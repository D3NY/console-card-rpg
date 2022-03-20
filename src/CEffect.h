#pragma once

#include <memory>

#include "CStats.h"
#include "EEffectType.h"

/**
 * The parent class for playing card effect
 */
class CEffect {
public:
    explicit CEffect(size_t PowerOfEffect);

    CEffect(const CEffect &) = default;

    CEffect &operator=(const CEffect &) = delete;

    virtual ~CEffect() = default;

    virtual std::unique_ptr<CEffect> Clone() const = 0;

    /**
     * Returns a string representation of the object
     * @return Object represented in a string
     */
    virtual std::string ToString() const = 0;

    /**
     * Returns the effect type
     * @return Effect type
     */
    virtual EEffectType Type() const = 0;

    /**
     * Applies effect to the card owner or opponent depending on the effect
     * @param owner Owner of the card
     * @param opponent Opponent
     * @return Reference to itself, so one can call this method in chain
     */
    virtual CEffect &ApplyEffect(CStats &owner, CStats &opponent) = 0;

    /**
     * Gets the power of the effect
     * @return Effect power value
     */
    size_t GetPowerOfEffect() const;


protected:
    size_t m_PowerOfEffect;
};