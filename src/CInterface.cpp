#include "CInterface.h"

using namespace std;

CInterface::CInterface(istream &In, ostream &Out) : m_In(In), m_Out(Out) {
    m_In.exceptions(ios::badbit | ios::eofbit);
    m_Out.exceptions(ios::failbit | ios::badbit | ios::eofbit);
}

CInterface &CInterface::ClearInput() {
    m_In.clear();
    m_In.ignore(numeric_limits<streamsize>::max(), '\n');
    return *this;
}

CInterface &CInterface::ClearTerminal() {
    // "\x1b[2J" clears the screen and sets cursor to home, "\x1b[H" returns the cursor to the home position
    m_Out << "\x1B[2J\x1B[H";
    return *this;
}

CInterface &CInterface::DrawPlayerBar(const shared_ptr<CPlayer> &player) {
    int playerBarWidth = m_NumberOfCards * (m_CardWidth + m_HorizontalGap);

    // Prints the border of the player status bar
    m_Out << setfill('-') << setw(playerBarWidth - 1) << "-" << endl;

    m_Out << player->GetPlayerNickname() << ": " << player->GetPlayerStats().GetHealth() << " Health | "
          << player->GetPlayerStats().GetShield() << " Shield | " << player->GetPlayerStats().GetMana() << " Mana" << endl;

    m_Out << setfill('-') << setw(playerBarWidth - 1) << "-" << endl;
    return *this;
}

CInterface &CInterface::DrawCardBorder(size_t x, size_t y) {
    // ANSI escape sequence "\033[y;xH", where ESC[y;xH moves courser to row y and col x

    // Prints the top border of the card
    m_Out << "\033[" << y << ";" << x << "H " << setfill('_') << setw(m_CardWidth - 1) << "_" << endl;

    // Prints the left and right border of the card
    for (size_t i = 1; i < m_CardHeight - 1; i++) {
        m_Out << "\033[" << (y + i) << ";" << x << "H|" << setfill(' ') << setw(m_CardWidth - 1) << " |" << endl;
    }

    // Prints the bottom border of the card
    m_Out << "\033[" << (y + m_CardHeight - 1) << ";" << x << "H|" << setfill('_')
          << setw(m_CardWidth - 1) << "_|" << endl;
    return *this;
}

CInterface &CInterface::DrawCardAttributes(const CCard &card, size_t x, size_t y) {
    // Moves the cursor two columns to the right so that the attribute does not start at the left border of the card
    x += 2;

    // Each increment of y moves the cursor one line down so that the attributes are printed below each other
    y++;
    m_Out << "\033[" << y << ";" << x << "H" << card.GetCardName();
    y++;
    m_Out << "\033[" << y << ";" << x << "HCost: " << card.GetCardCost();
    y++;
    m_Out << "\033[" << y << ";" << x << "H ";

    if (card.GetEffects().empty()) {
        return *this;
    }

    for (const auto &it : card.GetEffects()) {
        y++;
        m_Out << "\033[" << y << ";" << x << "H" << it->ToString();
    }

    return *this;
}

CInterface &CInterface::DrawCard(const CCard &card, size_t x, size_t y) {
    DrawCardBorder(x, y).DrawCardAttributes(card, x, y);
    return *this;
}

CInterface &CInterface::DrawPlayerDeck(const shared_ptr<CPlayer> &player, size_t y) {
    size_t x = 1;

    for (const auto &it : player->GetPlayerCards()) {
        DrawCard(it, x, y);

        // Moves the cursor x columns to the right to print the next card
        x += (m_CardWidth + m_HorizontalGap);
    }

    // After printing the deck of cards, it normalizes the cursor position so that the deck is not overwritten
    m_Out << "\033[" << (y + m_CardHeight + 1) << ";" << 1 << "H";
    return *this;
}

bool CInterface::ValidateUserInput(const string &userInput) const {
    for (const auto &it : userInput) {
        // ASCII table character codes, only A-Z, a-z and 0-9 characters allowed
        if ((it > 47 && it < 58) || (it > 64 && it < 91) || (it > 96 && it < 123)) {
            continue;
        }

        return false;
    }

    return true;
}

CInterface &CInterface::BeforeGameStart(const string &startingPlayerName) {
    m_Out << "The dice have been rolled and the starting player is "
          << "\u001b[1m" + startingPlayerName + "\u001b[0m" << "." << endl;
    return *this;
}

CInterface &CInterface::AfterGameEnd(const std::string &winnerName) {
    m_Out << "\u001b[1m" << winnerName << "\u001b[0m" << " just won the game! Honor to the losers.";
    return *this;
}

CInterface &CInterface::Redraw(const vector<shared_ptr<CPlayer>> &players, const CCard &card) {
    ClearTerminal();

    // Sets the required dimensions of the terminal before drawing the playing area
    SetTerminalDimensions();

    // Prints the status bar of the first player
    DrawPlayerBar(players.at(0));
    m_Out << " " << endl;

    size_t playerDeckY = m_PlayerBarHeight + m_VerticalGap + 1;
    // Prints the deck of cards of the first player
    DrawPlayerDeck(players.at(0), playerDeckY);

    size_t centerCardX = (m_NumberOfCards * (m_CardWidth + m_HorizontalGap) / 2) - (m_CardWidth / 2) - 1;
    size_t centerCardY = m_PlayerBarHeight + m_CardHeight + 3 * m_VerticalGap;

    // Prints the center card
    DrawCard(card, centerCardX, centerCardY);

    playerDeckY = centerCardY + m_CardHeight + 2 * m_VerticalGap;
    // Prints the status bar of the second player
    DrawPlayerDeck(players.at(1), playerDeckY);
    m_Out << " " << endl;

    // Prints the deck of cards of the first player
    DrawPlayerBar(players.at(1));
    return *this;
}

CInterface &CInterface::DrawMenu() {
    m_Out << "Card RPG menu:" << endl;
    m_Out << "\t1) New game (Player VS Player)" << endl;
    m_Out << "\t2) New game (Player VS Robot)" << endl;
    m_Out << "\t3) New game (Robot VS Robot) [EXPERIMENTAL]" << endl;
    m_Out << "\t4) Load game" << endl;
    m_Out << "\t5) Exit game" << endl;
    m_Out << "\n\u001b[7m" << "Please note that the minimum dimensions of the terminal are " << m_TerminalWidth
          << " columns and " << m_TerminalHeight << " rows. Otherwise, the game interface may be rendered incorrectly."
          << "\u001b[0m" << endl;
    return *this;
}

CInterface &CInterface::PrintLine(const string &line) {
    m_Out << line << endl;
    return *this;
}

string CInterface::PromptCardsDefinitionFilename() {
    return ReadUserInput<string>("Enter the path to the card definitions file:");
}

string CInterface::PromptGameName() {
    string gameName = ReadUserInput<string>("Enter a name for the game:");

    while (true) {
        if(gameName.length() > m_MaximumFilenameLength) {
            m_Out << "The game name cannot be longer than 255 characters." << endl;
            gameName = ReadUserInput<string>("Please enter a name for the game again:");
            continue;
        }

        if (!ValidateUserInput(gameName)) {
            m_Out << "Allowed characters are only A-Z, a-z, 0-9." << endl;
            gameName = ReadUserInput<string>("Please enter a name for the game again:");
            continue;
        }

        break;
    }

    return gameName;
}

string CInterface::PromptPlayerNickname(size_t playerInOrder) {
    string message;

    switch (playerInOrder) {
        case 1:
            message = "Enter the nickname of the first player:";
            break;
        case 2:
            message = "Enter the nickname of the second player:";
            break;
        default:
            message = "Enter your nickname:";
    }

    string playerNickname = ReadUserInput<string>(message);
    while (true) {
        if(playerNickname.length() > m_MaximumNicknameLength) {
            m_Out << "The player's nickname cannot be longer than 25 characters." << endl;
            playerNickname = ReadUserInput<string>("Please enter your nickname again:");
            continue;
        }

        if (!ValidateUserInput(playerNickname)) {
            m_Out << "Allowed characters are only A-Z, a-z, 0-9." << endl;
            playerNickname = ReadUserInput<string>("Please enter your nickname again:");
            continue;
        }

        break;
    }

    return playerNickname;
}

size_t CInterface::PromptMenuItem() {
    auto playerChoice = ReadUserInput<size_t>("Select an item from the menu:");

    while (true) {
        if (playerChoice < 1 || playerChoice > m_NumberOfMenuItems) {
            m_Out << "Selected menu item does not exist!" << endl;
            playerChoice = ReadUserInput<size_t>("Select again an item from the menu:");
            continue;
        }

        break;
    }

    return playerChoice;
}

size_t CInterface::PromptPlayerCard() {
    return ReadUserInput<size_t>("Select the card (1-" + to_string(m_NumberOfCards) + ") you want to play with:");
}

CInterface &CInterface::InvalidCardSelected() {
    m_Out << "You do not have such a card!" << endl;
    return *this;
}

CInterface &CInterface::PlayerOnMove(const string &playerName) {
    m_Out << "\u001b[1m" << playerName << "\u001b[0m" << " is now on the move." << endl;
    return *this;
}

CInterface &CInterface::CantMove(const string &playerName) {
    m_Out << "\u001b[1m" << playerName << "\u001b[0m" << " cannot play in this round because he lacks mana." << endl;
    return *this;
}

CInterface &CInterface::LackOfMana() {
    m_Out << "You do not have enough mana for this card." << endl;
    return *this;
}

CInterface &CInterface::SetTerminalDimensions() {
    // Resizes the terminal window to the required dimensions using ANSI escape sequences "\033[8;h;wt"
    m_Out << "\033[8;" << m_TerminalHeight << ";" << m_TerminalWidth << "t" << endl;
    return *this;
}

CInterface &CInterface::Exit() {
    m_Out << "Thanks for playing. I hope you will come back soon!" << endl;
    return *this;
}