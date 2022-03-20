#pragma once

#include <memory>

#include "CEffect.h"
#include "CStats.h"

/**
 * The class represents the healing effect of a playing card
 */
class CEffectHeal : public CEffect {
public:
    explicit CEffectHeal(size_t PowerOfEffect);

    std::unique_ptr<CEffect> Clone() const override;

    EEffectType Type() const override;

    std::string ToString() const override;

    CEffect &ApplyEffect(CStats &owner, CStats &opponent) override;
};