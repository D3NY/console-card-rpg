#pragma once

#include <memory>

/**
 * The class represents the stats of the player
 */
class CStats {
public:
    CStats() = default;

    CStats(size_t PlayerHealth, size_t PlayerShield, size_t PlayerMana);

    /**
     * Deals damage
     * @param powerOfCard The power of the card effect
     * @return Reference to itself, so one can call this method in chain
     */
    CStats &TakeDamage(size_t powerOfCard);

    /**
     * Increases a shield
     * @param powerOfCard The power of the card effect
     * @return Reference to itself, so one can call this method in chain
     */
    CStats &IncreaseShield(size_t powerOfCard);

    /**
     * Increases a health
     * @param powerOfCard The power of the card effect
     * @return Reference to itself, so one can call this method in chain
     */
    CStats &Heal(size_t powerOfCard);

    /**
     * Increases a mana
     * @param powerOfCard The power of the card effect
     * @return Reference to itself, so one can call this method in chain
     */
    CStats &IncreaseMana(size_t powerOfCard);

    /**
     * Decreases a mana
     * @param powerOfCard The power of the card effect
     * @return Reference to itself, so one can call this method in chain
     */
    CStats & DecreaseMana(size_t powerOfCard);

    /**
     * Gets the health
     * @return Health value
     */
    size_t GetHealth() const;

    /**
     * Gets the shield
     * @return Shield value
     */
    size_t GetShield() const;

    /**
     * Gets the mana
     * @return Mana value
     */
    size_t GetMana() const;

private:
    size_t m_Health = 15;
    size_t m_Shield = 5;
    size_t m_Mana = 10;
};