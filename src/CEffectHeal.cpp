#include "CEffectHeal.h"

using namespace std;

CEffectHeal::CEffectHeal(size_t PowerOfEffect) : CEffect(PowerOfEffect) {}

unique_ptr<CEffect> CEffectHeal::Clone() const {
    return make_unique<CEffectHeal>(*this);
}

EEffectType CEffectHeal::Type() const {
    return EEffectType::DEFENSIVE;
}

string CEffectHeal::ToString() const {
    return "Heal: " + to_string(m_PowerOfEffect);
}

CEffect &CEffectHeal::ApplyEffect(CStats &owner, CStats &opponent) {
    // Ignoring param
    static_cast<void>(opponent);

    owner.Heal(m_PowerOfEffect);
    return *this;
}