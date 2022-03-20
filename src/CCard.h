#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iterator>

#include "CEffect.h"
#include "CStats.h"

/**
 * The class represents a playing card
 */
class CCard {
public:
    CCard(size_t CardID, std::string CardName, size_t CardCostMana);

    CCard(const CCard &other);

    CCard &operator=(CCard other);

    ~CCard() = default;

    /**
     * Adds an effect to the card
     * @param effect Added effect
     * @return Reference to itself, so one can call this method in chain
     */
    CCard &AddEffect(std::unique_ptr<CEffect> effect);

    /**
     * Applies all effects to the card owner or opponent depending on the effects of the card
     * @param owner Owner of the card
     * @param opponent Opponent
     * @return Reference to itself, so one can call this method in chain
     */
    CCard &UseCard(CStats &owner, CStats &opponent);

    /**
     * Gets all the effects of the card
     * @return Card effects
     */
    const std::vector<std::shared_ptr<CEffect>> &GetEffects() const;

    /**
     * Gets the card name
     * @return Card name
     */
    const std::string &GetCardName() const;

    /**
     * Gets the card cost
     * @return Card cost
     */
    size_t GetCardCost() const;

    /**
     * Gets the card ID
     * @return Card ID
     */
    size_t GetCardID() const;


private:
    size_t m_ID;
    std::string m_Name;
    size_t m_Cost;
    std::vector<std::shared_ptr<CEffect>> m_Effects;
};