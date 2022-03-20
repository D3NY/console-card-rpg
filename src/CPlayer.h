#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>

#include "CStats.h"
#include "CCard.h"

/**
 * The parent class representing the player
 */
class CPlayer {
public:
    explicit CPlayer(std::string PlayerNickname);

    CPlayer(std::string PlayerNickname, CStats PlayerStats);

    CPlayer(const CPlayer &) = default;

    CPlayer &operator=(const CPlayer &) = delete;

    virtual ~CPlayer() = default;

    virtual std::unique_ptr<CPlayer> Clone() const = 0;

    /**
     * In each round requests next move from the player
     * @param opponent Opponent
     * @return The card the player is playing with
     */
    virtual CCard Move(const std::shared_ptr<CPlayer> &opponent) = 0;

    /**
    * Returns a string with the type of the player
    * @return Player type
    */
    virtual std::string Type() const = 0;

    /**
     * Adds the card to the player's deck
     * @param card Card to add
     * @return Reference to itself, so one can call this method in chain
     */
    CPlayer &AddCard(const CCard &card);

    /**
     * Gets the total endurance of the player
     * @return Endurance value
     */
    size_t GetEndurance() const;

    /**
     * Gets the player stats
     * @return Player stats
     */
    CStats &GetPlayerStats();

    /**
     * Gets the player's nickname
     * @return Player nickname
     */
    const std::string &GetPlayerNickname() const;

    /**
     * Gets all cards the player has in his hand
     * @return The player's cards
     */
    const std::vector<CCard> &GetPlayerCards() const;

    /**
     * Finds out if the player is still alive
     * @return true if the player is alive, otherwise false
     */
    bool IsAlive() const;

    /**
     * Finds out if the player is able to move
     * @return true if the player can move, otherwise false
     */
    bool IsPossibleMove() const;

    /**
     * Finds if the player has a card in the deck with the required effect, it can also find out if the card effect can kill an opponent
     * @param effectType Required card effect
     * @param opponentEndurance Opponent's endurance
     * @return Card position in the deck
     */
    std::vector<CCard>::iterator SearchCard(EEffectType effectType, size_t opponentEndurance = 0);

    /**
     * Checks whether the player has a card at a given position in the deck
     * @param it Card position in the deck
     * @return true if the card is in the deck, otherwise false
     */
    bool CardFound(std::vector<CCard>::iterator it) const;


protected:
    std::string m_Nickname;
    CStats m_Stats;
    std::vector<CCard> m_Cards;

    /**
     * Returns the card the player is playing with and removes it from his deck
     * @param it Card position in the deck
     * @return The card the player is playing with
     */
    CCard SelectCard(std::vector<CCard>::iterator it);
};