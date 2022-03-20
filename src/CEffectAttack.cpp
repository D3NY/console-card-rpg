#include "CEffectAttack.h"

using namespace std;

CEffectAttack::CEffectAttack(size_t PowerOfEffect) : CEffect(PowerOfEffect) {}

unique_ptr<CEffect> CEffectAttack::Clone() const {
    return make_unique<CEffectAttack>(*this);
}

EEffectType CEffectAttack::Type() const {
    return EEffectType::OFFENSIVE;
}

string CEffectAttack::ToString() const {
    return "Attack: " + to_string(m_PowerOfEffect);
}

CEffect &CEffectAttack::ApplyEffect(CStats &owner, CStats &opponent) {
    // Ignoring param
    static_cast<void>(owner);

    opponent.TakeDamage(m_PowerOfEffect);
    return *this;
}