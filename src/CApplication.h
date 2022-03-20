#pragma once

#include "CGame.h"
#include "CInterface.h"
#include "EGameType.h"

/**
 * The class obtains the necessary attributes from the user and then initializes the game
 */
class CApplication {
public:
    explicit CApplication(const std::shared_ptr<CInterface> &Interface);

    CApplication(const CApplication &) = delete;

    CApplication &operator=(const CApplication &) = delete;

    ~CApplication() = default;

    /**
     * Sets the status of the application
     * @param running true the application is running, false the application is stopped
     * @return Reference to itself, so one can call this method in chain
     */
    CApplication &Running(bool running);

    /**
     * Starts the application
     */
    void Run();


private:
    std::shared_ptr<CInterface> m_Interface;
    std::unique_ptr<CGame> m_Game = nullptr;
    bool m_Running = true;

    /**
     * Initializes a new game
     * @param gameType Specifies the game type
     * @return Reference to itself, so one can call this method in chain
     */
    CApplication &NewGame(EGameType gameType);

    /**
     * Initializes the saved game
     * @return Reference to itself, so one can call this method in chain
     */
    CApplication &LoadGame();
};