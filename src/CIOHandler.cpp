#include "CIOHandler.h"
#include "CPlayerRobot.h"
#include "CPlayerHuman.h"
#include "CEffectAttack.h"
#include "CEffectDefend.h"
#include "CEffectHeal.h"
#include "CEffectMana.h"

using namespace std;

CIOHandler::CIOHandler(string Filename, const shared_ptr<CInterface> &Interface)
        : m_Filename(move(Filename)), m_Interface(Interface) {}

CIOHandler &CIOHandler::SetFilename(const string &filename) {
    m_Filename = filename;
    return *this;
}

bool CIOHandler::RemoveSave() {
    if (remove((m_GameSavePath + m_Filename).c_str()) != 0) {
        m_Interface->PrintLine("The saved game could not be deleted.");
        return false;
    }

    return true;
}

bool CIOHandler::IsNumber(const string &number) const {
    if (number.empty()) {
        return false;
    }

    return all_of(number.begin(), number.end(), ::isdigit);
}

bool CIOHandler::ProcessCardName(string &cardName) {
    if (cardName.empty()) {
        return false;
    }

    if (cardName.length() > 11) {
        cardName = cardName.substr(0, 12);
    }

    return true;
}

bool CIOHandler::ProcessCardCost(string &cardCost) {
    // Removes spaces from the entire string
    cardCost.erase(remove_if(cardCost.begin(), cardCost.end(), ::isspace), cardCost.end());

    size_t delimiterPosition = cardCost.find(':');
    string attribute = cardCost.substr(0, delimiterPosition);

    // Saves the cost of the card
    cardCost = cardCost.substr(delimiterPosition + 1);

    if (!IsNumber(cardCost) || stoull(cardCost) < 1 || attribute != "Cost") {
        return false;
    }

    return true;
}

bool CIOHandler::ProcessCardEffect(string &cardEffectName, string &cardEffectPower) {
    // Removes spaces from the entire string
    cardEffectName.erase(remove_if(cardEffectName.begin(), cardEffectName.end(), ::isspace), cardEffectName.end());

    size_t delimiterPosition = cardEffectName.find(':');

    // Saves the name and power of the effect
    cardEffectPower = cardEffectName.substr(delimiterPosition + 1);
    cardEffectName = cardEffectName.substr(0, delimiterPosition);


    if (!IsNumber(cardEffectPower) || stoull(cardEffectPower) < 1) {
        return false;
    }

    if (cardEffectName != "Attack" && cardEffectName != "Defense"
        && cardEffectName != "Heal" && cardEffectName != "Mana") {

        return false;
    }

    return true;
}

unique_ptr<CEffect> CIOHandler::MakeCardEffect(const string &cardEffectName, const string &cardEffectPower) {

    size_t effectPower = stoll(cardEffectPower);

    if (cardEffectName == "Attack") {
        return make_unique<CEffectAttack>(effectPower);
    }

    if (cardEffectName == "Defense") {
        return make_unique<CEffectDefend>(effectPower);
    }

    if (cardEffectName == "Heal") {
        return make_unique<CEffectHeal>(effectPower);
    }

    return make_unique<CEffectMana>(effectPower);
}

CCard CIOHandler::LoadCard(ifstream &fin, size_t &cardID) {
    CCard card(0, "Undefined", 0);
    string cardName, cardCost, cardEffect, cardEffectPower;
    size_t lineType = 1, numberOfEffect = 0;

    string line;
    while (getline(fin, line)) {

        // Skips empty lines
        if (line.empty()) {
            continue;
        }

        // Skips comments
        if (!line.empty() && line.at(0) == m_CommentFlag) {
            continue;
        }

        switch (lineType) {
            case 1:
                cardName = line;
                if (!ProcessCardName(cardName)) {
                    m_Interface->PrintLine("Bad formatting! Check the card names.");
                    return card;
                }

                lineType++;
                break;

            case 2:
                cardCost = line;
                if (!ProcessCardCost(cardCost)) {
                    m_Interface->PrintLine("Bad formatting! Check card prices.");
                    return card;
                }

                card = CCard(cardID, cardName, stoull(cardCost));
                lineType++;
                break;

            case 3:
                if (numberOfEffect >= 3) {
                    m_Interface->PrintLine("Bad formatting! Cards cannot have more than three effects.");
                    return card;
                }

                cardEffect = line;
                if (!ProcessCardEffect(cardEffect, cardEffectPower)) {
                    m_Interface->PrintLine("Bad formatting! Check card effects.");
                    return card;
                }

                card.AddEffect(MakeCardEffect(cardEffect, cardEffectPower));
                numberOfEffect++;
                break;

            default:
                m_Interface->PrintLine("Bad formatting!");
                return card;
        }

        // If the next line is empty, the whole card has been loaded
        if (fin.peek() == '\n') {
            break;
        }
    }

    cardID++;
    return card;
}

unique_ptr<CPlayer> CIOHandler::LoadPlayer(ifstream &fin, size_t &cardID) {
    unique_ptr<CPlayer> player;
    CCard card(0, "Undefined", 0);
    string type, nickname, health, shield, mana;

    getline(fin, type);
    getline(fin, nickname);
    getline(fin, health);
    getline(fin, shield);
    getline(fin, mana);

    if (!IsNumber(health) || !IsNumber(shield) || !IsNumber(mana)) {
        m_Interface->PrintLine("Bad formatting! The player failed to load.");
        return nullptr;
    }

    if (type == "Robot") {
        player = make_unique<CPlayerRobot>(CPlayerRobot(CStats(stoull(health), stoull(shield), stoull(mana))));
    } else {
        player = make_unique<CPlayerHuman>(CPlayerHuman(nickname, CStats(stoull(health), stoull(shield), stoull(mana)), m_Interface));
    }

    // Load a deck of player's cards
    for (size_t i = 0; i < m_NumberOfCards; i++) {
        card = LoadCard(fin, cardID);
        if (card.GetCardID() == 0) {
            m_Interface->PrintLine("Bad formatting! The player card deck failed to load.");
            return nullptr;
        }

        player->AddCard(card);
    }

    return player;
}

CIOHandler &CIOHandler::SaveCard(ofstream &fos, const CCard &card) {
    fos << card.GetCardName() << endl;
    fos << "Cost: " << card.GetCardCost() << endl;

    for (const auto &itEffect : card.GetEffects()) {
        fos << itEffect->ToString() << endl;
    }
    return *this;
}

CIOHandler &CIOHandler::SavePlayer(ofstream &fos, const shared_ptr<CPlayer> &player) {
    fos << player->Type() << endl;
    fos << player->GetPlayerNickname() << endl;
    fos << player->GetPlayerStats().GetHealth() << endl;
    fos << player->GetPlayerStats().GetShield() << endl;
    fos << player->GetPlayerStats().GetMana() << endl;

    fos << endl;
    for (const auto &itCards : player->GetPlayerCards()) {
        SaveCard(fos, itCards);
        fos << endl;
    }

    return *this;
}

bool CIOHandler::LoadCards(vector<CCard> &cards) {
    CCard card(0, "Undefined", 0);
    size_t cardID = 1;

    ifstream fin(m_Filename);
    if (fin.fail() || !fin.is_open() || !fin) {
        m_Interface->PrintLine("The file cannot be opened!");
        return false;
    }

    // Loads the entire deck of cards
    while (!fin.eof()) {
        card = LoadCard(fin, cardID);
        if (card.GetCardID() == 0) {
            m_Interface->PrintLine("Undefined card.");
            return false;
        }

        cards.push_back(card);
    }

    fin.close();

    if (cards.size() < m_MinCards) {
        m_Interface->PrintLine("The deck must contain at least " + to_string(m_MinCards) + " cards.");
        return false;
    }

    return true;
}

bool CIOHandler::SaveGame(const CCard &activeCard, const vector<CCard> &deckOfCards,
                          const vector<shared_ptr<CPlayer>> &players, bool playerOnTurn) {

    ofstream fos(m_GameSavePath + m_Filename, ofstream::trunc);
    if (!fos.is_open()) {
        m_Interface->PrintLine("Failed to create game file.");
        return false;
    }

    fos << "PLAYER ON TURN" << endl;
    fos << playerOnTurn << endl;
    fos << endl;

    fos << "PLAYER 1" << endl;
    SavePlayer(fos, players.at(0));

    fos << "PLAYER 2" << endl;
    SavePlayer(fos, players.at(1));

    fos << "ACTIVE CARD" << endl;
    SaveCard(fos, activeCard);
    fos << "" << endl;

    fos << "CARD DECK" << endl;
    for (size_t i = 0; i < deckOfCards.size() - 1; i++) {
        SaveCard(fos, deckOfCards.at(i));
        fos << endl;
    }

    SaveCard(fos, deckOfCards.at(deckOfCards.size() - 1));

    fos.close();
    return true;
}

bool CIOHandler::LoadGame(const unique_ptr<CGame> &game) {
    CCard card(0, "Undefined", 0);
    unique_ptr<CPlayer> player;
    size_t cardID = 1, cardsInDeck = 0;

    ifstream fin(m_GameSavePath + m_Filename);
    if (fin.fail() || !fin.is_open() || !fin) {
        m_Interface->PrintLine("Loading failed. The file cannot be opened!");
        return false;
    }

    string line;
    // Loads player on the turn
    getline(fin, line);
    if (line != "PLAYER ON TURN") {
        return false;
    }

    getline(fin, line);
    if (!IsNumber(line)) {
        return false;
    }
    game->SetPlayerOnTurn(stoull(line));

    // Make sure there is an empty line between attributes
    if (fin.get() != '\n') {
        return false;
    }

    // Loads first player
    getline(fin, line);
    if (line != "PLAYER 1") {
        return false;
    }
    game->AddPlayer(LoadPlayer(fin, cardID));

    if (fin.get() != '\n') {
        return false;
    }

    // Loads second player
    getline(fin, line);
    if (line != "PLAYER 2") {
        return false;
    }
    game->AddPlayer(LoadPlayer(fin, cardID));

    if (fin.get() != '\n') {
        return false;
    }

    // Loads an active card
    getline(fin, line);
    if (line != "ACTIVE CARD") {
        return false;
    }

    card = LoadCard(fin, cardID);
    if (card.GetCardID() == 0) {
        m_Interface->PrintLine("Undefined card.");
        return false;
    }
    game->SetActiveCard(card);

    if (fin.get() != '\n') {
        return false;
    }

    // Loads card deck
    getline(fin, line);
    if (line != "CARD DECK") {
        return false;
    }

    while (!fin.eof()) {
        card = LoadCard(fin, cardID);
        if (card.GetCardID() == 0) {
            m_Interface->PrintLine("Undefined card.");
            return false;
        }

        game->AddCard(card);
        cardsInDeck++;
    }

    if(cardsInDeck < (m_MinCards - (2 * m_NumberOfCards + 1))) {
        m_Interface->PrintLine("The deck must contain at least " + to_string(m_MinCards - (2 * m_NumberOfCards + 1)) + " cards.");
        return false;
    }

    fin.close();
    return true;
}