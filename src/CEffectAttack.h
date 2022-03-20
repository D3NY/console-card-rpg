#pragma once

#include <memory>

#include "CEffect.h"

/**
 * The class represents the attacking effect of a playing card
 */
class CEffectAttack : public CEffect {
public:
    explicit CEffectAttack(size_t PowerOfEffect);

    std::unique_ptr<CEffect> Clone() const override;

    EEffectType Type() const override;

    std::string ToString() const override;

    CEffect &ApplyEffect(CStats &owner, CStats &opponent) override;
};