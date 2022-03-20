#pragma once

#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <string>

#include "CPlayer.h"

/**
 * The class provides communication between the application / game and the user
 */
class CInterface {
public:
    CInterface(std::istream &In, std::ostream &Out);

    /**
     * Prints the starting player before the game starts
     * @param startingPlayerName The name of the starting player
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &BeforeGameStart(const std::string &startingPlayerName);

    /**
     * Prints the winner at the end of the game
     * @param winnerName The name of the winner
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &AfterGameEnd(const std::string &winnerName);

    /**
     * Prints the status bar and decks of both players' cards, as well as the active card
     * @param players Players
     * @param card Active card
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &Redraw(const std::vector<std::shared_ptr<CPlayer>> &players, const CCard &card);

    /**
     * Draws menus to the console
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &DrawMenu();

    /**
     * Prints the string on a line to the console
     * @param line String to print
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &PrintLine(const std::string &line);

    /**
     * Prints the name of the player on the turn to the console
     * @param playerName Name of the player
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &PlayerOnMove(const std::string &playerName);

    /**
     * Prints information if a player on a turn cannot participate in the round
     * @param playerName Name of the player
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &CantMove(const std::string &playerName);

    /**
     * Prints information about the selected card that does not exist
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &InvalidCardSelected();

    /**
     * Prints information about not enough mana
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &LackOfMana();

    /**
     * Prints a thank you note before closing the application
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &Exit();

    /**
     * Prints a thank you note before closing the application
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &SetTerminalDimensions();

    /**
     * Prompts the user to select an item from the menu
     * @return Menu item number
     */
    size_t PromptMenuItem();

    /**
     * Prompts the user on a turn to select a playing card
     * @return The order of the cards in the deck
     */
    size_t PromptPlayerCard();

    /**
     * Prompts the user to enter the path to the card definitions file
     * @return Path to file
     */
    std::string PromptCardsDefinitionFilename();

    /**
     * Prompts the user to enter the name of the game
     * @return The name of the game
     */
    std::string PromptGameName();

    /**
     * Prompts the user to enter the nickname
     * @param playerInOrder Player in order
     * @return The player nickname
     */
    std::string PromptPlayerNickname(size_t playerInOrder = 0);


private:
    std::istream &m_In;
    std::ostream &m_Out;

    static constexpr size_t m_TerminalHeight = 50;
    static constexpr size_t m_TerminalWidth = 120;
    /** The maximum number of cards that are drawn in the player's hand */
    static constexpr size_t m_NumberOfCards = 5;
    static constexpr size_t m_CardWidth = 15;
    static constexpr size_t m_CardHeight = 9;
    static constexpr size_t m_PlayerBarHeight = 3;
    static constexpr size_t m_VerticalGap = 2;
    static constexpr size_t m_HorizontalGap = 4;
    static constexpr size_t m_NumberOfMenuItems = 5;
    static constexpr size_t m_MaximumNicknameLength = 25;
    static constexpr size_t m_MaximumFilenameLength = 255;

    /**
     * Clears the input
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &ClearInput();

    /**
     * Clears the screen and sets cursor to home position
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &ClearTerminal();

    /**
     * Prints the player status bar to the console
     * @param player The player
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &DrawPlayerBar(const std::shared_ptr<CPlayer> &player);

    /**
     * Prints a card border to the console
     * @param x X coordinates of the upper left corner
     * @param y Y coordinates of the upper left corner
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &DrawCardBorder(size_t x, size_t y);

    /**
     * Prints card attributes to the terminal
     * @param card Card to print
     * @param x X coordinates of the upper left corner
     * @param y Y coordinates of the upper left corner
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &DrawCardAttributes(const CCard &card, size_t x, size_t y);

    /**
     * Prints the entire card to the terminal
     * @param card Card to print
     * @param x X coordinates of the upper left corner
     * @param y Y coordinates of the upper left corner
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &DrawCard(const CCard &card, size_t x, size_t y);

    /**
     * Prints the cards the player has in his hand to the terminal
     * @param player Player to print
     * @param y Y coordinates of the upper left corner
     * @return Reference to itself, so one can call this method in chain
     */
    CInterface &DrawPlayerDeck(const std::shared_ptr<CPlayer> &player, size_t y);

    /**
     * Validates user input
     * @param userInput User input
     * @return true if the validation was successful, otherwise false
     */
    bool ValidateUserInput(const std::string &userInput) const;

    /**
     * Reads user input from the terminal
     * @tparam T_ A data type that can be read from the input stream using the << operator
     * @param message Message to enter input
     * @return Loaded value
     */
    template<typename T_>
    T_ ReadUserInput(const std::string &message) {
        T_ userInput;

        while (true) {
            m_Out << message << std::endl;

            if (!(m_In >> userInput)) {
                m_Out << "An error occurred while reading!" << std::endl;
                ClearInput();
                continue;
            }

            break;
        }

        ClearInput();

        return userInput;
    }
};