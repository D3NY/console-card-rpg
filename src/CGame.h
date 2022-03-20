#pragma once

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <random>
#include <ctime>

#include "CPlayer.h"
#include "CCard.h"
#include "CEffect.h"
#include "CInterface.h"

/**
 * The class initializes the game and further ensures its overall course
 */
class CGame {
public:
    explicit CGame(std::string GameName, const std::shared_ptr<CInterface> &Interface);

    CGame(const CGame &other);

    CGame &operator=(CGame other);

    ~CGame() = default;

    /**
     * Adds player to the game
     * @param player Player to add
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &AddPlayer(std::unique_ptr<CPlayer> player);

    /**
     * Adds card to the game deck
     * @param card Card to add
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &AddCard(const CCard& card);

    /**
     * Sets the card as currently placed
     * @param card Card to be set
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &SetActiveCard(const CCard& card);

    /**
     * Sets the player who is now on the turn
     * @param playerOnTurn Player on the turn
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &SetPlayerOnTurn(bool playerOnTurn);

    /**
     * Loads card definitions, selects starting player, licks cards to player hands and draw user interface
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &Init();

    /**
     * Ensures player move, applies card effects and redraws user interface
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &Update();

    /**
     * After loading the game redraws the user interface
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &Load();

    /**
     * Starts the game
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &Run();


private:
    std::string m_Name;
    std::shared_ptr<CInterface> m_Interface;
    CCard m_ActiveCard;
    std::vector<CCard> m_DeckOfCards;
    std::vector<std::shared_ptr<CPlayer>> m_Players;
    std::shared_ptr<CPlayer> m_Winner;
    bool m_PlayerOnTurn = false;

    /** The maximum number of cards that are drawn in the player's hand */
    static constexpr size_t m_NumberOfCards = 5;
    /** The amount of mana that will be given to the player after each turn */
    static constexpr size_t m_AmountOfMana = 2;

    /**
     * Randomly draws a starting player
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &SelectStartingPlayer();

    /**
     * Shuffles the deck of cards
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &ShuffleDeckOfCards();

    /**
     * Licks the card into the player's card deck
     * @param player The player who licks the card
     * @return Reference to itself, so one can call this method in chain
     */
    CGame &LickCard(const std::shared_ptr<CPlayer> &player);
};