#pragma once

#include <memory>

#include "CEffect.h"
#include "CStats.h"

/**
 * The class represents the magic effect of a playing card
 */
class CEffectMana : public CEffect {
public:
    explicit CEffectMana(size_t PowerOfEffect);

    std::unique_ptr<CEffect> Clone() const override;

    EEffectType Type() const override;

    std::string ToString() const override;

    CEffect &ApplyEffect(CStats &owner, CStats &opponent) override;
};