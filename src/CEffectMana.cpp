#include "CEffectMana.h"

using namespace std;

CEffectMana::CEffectMana(size_t PowerOfEffect) : CEffect(PowerOfEffect) {}

unique_ptr<CEffect> CEffectMana::Clone() const {
    return make_unique<CEffectMana>(*this);
}

EEffectType CEffectMana::Type() const {
    return EEffectType::UTILITY;
}

string CEffectMana::ToString() const {
    return "Mana: " + to_string(m_PowerOfEffect);
}

CEffect &CEffectMana::ApplyEffect(CStats &owner, CStats &opponent) {
    // Ignoring param
    static_cast<void>(opponent);

    owner.IncreaseMana(m_PowerOfEffect);
    return *this;
}