#include "CApplication.h"
#include "CPlayerHuman.h"
#include "CPlayerRobot.h"
#include "CIOHandler.h"

using namespace std;

CApplication::CApplication(const shared_ptr<CInterface> &Interface) : m_Interface(Interface) {}

CApplication &CApplication::NewGame(EGameType gameType) {
    string gameName = m_Interface->PromptGameName();
    m_Game = make_unique<CGame>(CGame(gameName, m_Interface));

    unique_ptr<CPlayer> playerOne, playerTwo;
    string firstPlayerName, secondPlayerName;

    switch (gameType) {
        case EGameType::PVP:
            firstPlayerName = m_Interface->PromptPlayerNickname(1);
            secondPlayerName = m_Interface->PromptPlayerNickname(2);

            // Ensures that players do not have the same name
            if (firstPlayerName == secondPlayerName) {
                secondPlayerName += "1";
            }

            playerOne = make_unique<CPlayerHuman>(firstPlayerName, m_Interface);
            playerTwo = make_unique<CPlayerHuman>(secondPlayerName, m_Interface);
            break;

        case EGameType::PVR:
            firstPlayerName = m_Interface->PromptPlayerNickname();

            playerOne = make_unique<CPlayerHuman>(firstPlayerName, m_Interface);
            playerTwo = make_unique<CPlayerRobot>();
            break;

        case EGameType::RVR:
            playerOne = make_unique<CPlayerRobot>();
            playerTwo = make_unique<CPlayerRobot>("Steven Segfault");
            break;

        default:
            return *this;
    }

    m_Game->AddPlayer(move(playerOne)).AddPlayer(move(playerTwo)).Init().Run();

    return *this;
}

CApplication &CApplication::LoadGame() {
    string gameName = m_Interface->PromptGameName();
    m_Game = make_unique<CGame>(CGame(gameName, m_Interface));

    CIOHandler io(gameName, m_Interface);

    // In case of any error during reading, it will print an error and return to the menu
    if (!io.LoadGame(m_Game)) {
        return *this;
    }

    m_Game->Load();
    return *this;
}

CApplication &CApplication::Running(bool running) {
    m_Running = running;
    return *this;
}

void CApplication::Run() {
    while (m_Running) {
        // Sets the appropriate dimensions of the terminal before drawing the user interface
        m_Interface->SetTerminalDimensions();
        m_Interface->DrawMenu();
        size_t menuItem = m_Interface->PromptMenuItem();

        switch (menuItem) {
            case 1:
                NewGame(EGameType::PVP);
                break;
            case 2:
                NewGame(EGameType::PVR);
                break;
            case 3:
                NewGame(EGameType::RVR);
                break;
            case 4:
                LoadGame();
                break;
            default:
                m_Interface->Exit();
                Running(false);
        }
    }
}