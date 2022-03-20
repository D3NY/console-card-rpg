#include "CGame.h"
#include "CIOHandler.h"

using namespace std;

CGame::CGame(string GameName, const shared_ptr<CInterface> &Interface)
        : m_Name(move(GameName)), m_Interface(Interface), m_ActiveCard(0, "Welcome", 0) {}

CGame::CGame(const CGame &other)
        : m_Name(other.m_Name), m_Interface(other.m_Interface), m_ActiveCard(other.m_ActiveCard) {

    for (const auto &card : other.m_DeckOfCards) {
        m_DeckOfCards.push_back(card);
    }

    for (const auto &player : other.m_Players) {
        m_Players.push_back(player->Clone());

        if (other.m_Winner == player) {
            m_Winner = player->Clone();
        }
    }

    m_PlayerOnTurn = other.m_PlayerOnTurn;
}

CGame &CGame::operator=(CGame other) {
    swap(m_Name, other.m_Name);
    swap(m_Interface, other.m_Interface);
    swap(m_ActiveCard, other.m_ActiveCard);
    swap(m_DeckOfCards, other.m_DeckOfCards);
    swap(m_Players, other.m_Players);
    swap(m_Winner, other.m_Winner);
    swap(m_PlayerOnTurn, other.m_PlayerOnTurn);
    return *this;
}

CGame &CGame::AddPlayer(unique_ptr<CPlayer> player) {
    m_Players.push_back(move(player));
    return *this;
}

CGame &CGame::AddCard(const CCard &card) {
    m_DeckOfCards.push_back(card);
    return *this;
}

CGame &CGame::SetActiveCard(const CCard &card) {
    m_ActiveCard = card;
    return *this;
}

CGame &CGame::SetPlayerOnTurn(bool playerOnTurn) {
    m_PlayerOnTurn = playerOnTurn;
    return *this;
}

CGame &CGame::Init() {
    // Gets from the user the path to the card definitions file
    string filename = m_Interface->PromptCardsDefinitionFilename();
    CIOHandler io(filename, m_Interface);

    // Loads card definitions
    while (true) {
        if (!io.LoadCards(m_DeckOfCards)) {
            m_DeckOfCards.clear();
            filename = m_Interface->PromptCardsDefinitionFilename();
            io.SetFilename(filename);
            continue;
        }

        break;
    }

    // Before the game starts, it shuffle the deck of cards and select the starting player
    ShuffleDeckOfCards().SelectStartingPlayer();

    // Each player licks four cards from the deck
    for (size_t i = 0; i < m_NumberOfCards; i++) {
        LickCard(m_Players.at(m_PlayerOnTurn));
        LickCard(m_Players.at(!m_PlayerOnTurn));
    }

    // The player who is not on a turn gets mana extra
    m_Players.at(!m_PlayerOnTurn)->GetPlayerStats().IncreaseMana(m_AmountOfMana + 1);

    // Redraws the user interface and prints an introductory message
    m_Interface->Redraw(m_Players, m_ActiveCard).BeforeGameStart(m_Players.at(m_PlayerOnTurn)->GetPlayerNickname());
    return *this;
}

CGame &CGame::Load() {
    // After loading the game from the file redraws the user interface and prints who is on the turn now
    m_Interface->Redraw(m_Players, m_ActiveCard).PlayerOnMove(m_Players.at(m_PlayerOnTurn)->GetPlayerNickname());
    Run();
    return *this;
}

CGame &CGame::Update() {
    // Finds out if the player on a turn has enough mana to make another move
    if (!m_Players.at(m_PlayerOnTurn)->IsPossibleMove()) {
        // Prints a message about not enough mana for the next move
        m_Interface->CantMove(m_Players.at(m_PlayerOnTurn)->GetPlayerNickname());

        m_Players.at(m_PlayerOnTurn)->GetPlayerStats().IncreaseMana(m_AmountOfMana);

        m_Interface->Redraw(m_Players, m_ActiveCard).PlayerOnMove(m_Players.at(!m_PlayerOnTurn)->GetPlayerNickname());
        m_PlayerOnTurn = !m_PlayerOnTurn;
        return *this;
    }

    // Gets the card the player used on this turn
    m_ActiveCard = m_Players.at(m_PlayerOnTurn)->Move(m_Players.at(!m_PlayerOnTurn));

    // Applies the card effect to the player or opponent, depending on the card effect
    m_ActiveCard.UseCard(m_Players.at(m_PlayerOnTurn)->GetPlayerStats(),
                         m_Players.at(!m_PlayerOnTurn)->GetPlayerStats());
    m_DeckOfCards.push_back(m_ActiveCard);

    // After the move, checks if the opponent is still alive
    if (!m_Players.at(!m_PlayerOnTurn)->IsAlive()) {
        // The opponent is not alive, set the winner
        m_Winner = m_Players.at(m_PlayerOnTurn);
        return *this;
    }

    // After the turn, the player licks another card and gets mana
    LickCard(m_Players.at(m_PlayerOnTurn));
    m_Players.at(m_PlayerOnTurn)->GetPlayerStats().IncreaseMana(m_AmountOfMana);

    // Redraws the user interface and prints who is on the turn now
    m_Interface->Redraw(m_Players, m_ActiveCard).PlayerOnMove(m_Players.at(!m_PlayerOnTurn)->GetPlayerNickname());

    m_PlayerOnTurn = !m_PlayerOnTurn;

    CIOHandler io(m_Name, m_Interface);
    io.SaveGame(m_ActiveCard, m_DeckOfCards, m_Players, m_PlayerOnTurn);
    return *this;
}

CGame &CGame::Run() {
    while (m_Winner == nullptr) {
        Update();
    }

    // At the end of the game, set the winner
    m_Interface->AfterGameEnd(m_Winner->GetPlayerNickname());

    CIOHandler io(m_Name, m_Interface);
    io.RemoveSave();
    return *this;
}

CGame &CGame::SelectStartingPlayer() {
    mt19937 randomGenerator(random_device{}());
    // Reference: en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    m_PlayerOnTurn = uniform_int_distribution<>{0, 1}(randomGenerator);
    return *this;
}

CGame &CGame::ShuffleDeckOfCards() {
    // Reference: en.cppreference.com/w/cpp/algorithm/random_shuffle
    shuffle(m_DeckOfCards.begin(), m_DeckOfCards.end(), mt19937(random_device{}()));
    return *this;
}

CGame &CGame::LickCard(const shared_ptr<CPlayer> &player) {
    player->AddCard(m_DeckOfCards.at(0));
    m_DeckOfCards.erase(m_DeckOfCards.begin());
    return *this;
}