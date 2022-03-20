#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <algorithm>
#include <vector>

#include "CCard.h"
#include "CEffect.h"
#include "CPlayer.h"
#include "CInterface.h"
#include "CGame.h"

/**
 * The class provides an interface for saving and loading data from a file
 */
class CIOHandler {
public:
    explicit CIOHandler(std::string Filename, const std::shared_ptr<CInterface> &Interface);

    /**
     * Sets the value of a local variable to the new file name / path
     * @param filename New file name / path
     * @return Reference to itself, so one can call this method in chain
     */
    CIOHandler &SetFilename(const std::string &filename);

    /**
     * Loads card definitions into the card deck
     * @param cards Card deck
     * @return true if the loading was successful, otherwise false
     */
    bool LoadCards(std::vector<CCard> &cards);

    /**
     * Saves important game attributes to the file
     * @param activeCard Currently placed card
     * @param deckOfCards Current card deck
     * @param players Players
     * @param playerOnTurn Player on turn
     * @return true if the loading was successful, otherwise false
     */
    bool SaveGame(const CCard &activeCard, const std::vector<CCard> &deckOfCards,
                  const std::vector<std::shared_ptr<CPlayer>> &players, bool playerOnTurn);

    /**
     * Loads game attributes from a file and builds the game
     * @param game Current Game
     * @return true if the loading was successful, otherwise false
     */
    bool LoadGame(const std::unique_ptr<CGame> &game);

    /**
     * Removes the game save
     * @return true if the removing was successful, otherwise false
     */
    bool RemoveSave();


private:
    std::string m_Filename;
    std::shared_ptr<CInterface> m_Interface;
    /** The minimum amount of cards in the card deck */
    constexpr static size_t m_MinCards = 15;
    /** The maximum number of cards that are drawn in the player's hand */
    static constexpr size_t m_NumberOfCards = 5;
    constexpr static char m_CommentFlag = '#';
    constexpr static const char *const m_GameSavePath = "examples/saves/";

    /**
     * Verifies that the number in the string is positive and valid
     * @param number Entered number in the string
     * @return true if the string is valid number, otherwise false
     */
    bool IsNumber(const std::string &number) const;

    /**
     * Validates the card name and shortens it if necessary
     * @param cardName Card name
     * @return true if the card name was processed correctly, otherwise false
     */
    bool ProcessCardName(std::string &cardName);

    /**
     * Validates the card cost and returns only the value
     * @param cardCost Card cost
     * @return true if the card cost was processed correctly, otherwise false
     */
    bool ProcessCardCost(std::string &cardCost);

    /**
     * Validates the effect
     * @param cardEffectName Effect name
     * @param cardEffectPower Effect power
     * @return true if the effect was processed correctly, otherwise false
     */
    bool ProcessCardEffect(std::string &cardEffectName, std::string &cardEffectPower);

    /**
     * Creates an effect from the parameters
     * @param cardEffectName Card effect name
     * @param cardEffectPower Card effect power
     * @return Effect
     */
    std::unique_ptr<CEffect> MakeCardEffect(const std::string &cardEffectName, const std::string &cardEffectPower);

    /**
     * Loads a card from the file
     * @param fin File input stream
     * @param cardID Initial card ID
     * @return Loaded card
     */
    CCard LoadCard(std::ifstream &fin, size_t &cardID);

    /**
     * Loads a player from the file
     * @param fin File input stream
     * @param cardID Initial card ID
     * @return Loaded player
     */
    std::unique_ptr<CPlayer> LoadPlayer(std::ifstream &fin, size_t &cardID);

    /**
     * Saves the card to the file
     * @param fos File output stream
     * @param card Card to save
     * @return Reference to itself, so one can call this method in chain
     */
    CIOHandler &SaveCard(std::ofstream &fos, const CCard &card);

    /**
     * Saves the player to the file
     * @param fos File output stream
     * @param player Player to save
     * @return Reference to itself, so one can call this method in chain
     */
    CIOHandler &SavePlayer(std::ofstream &fos, const std::shared_ptr<CPlayer> &player);
};