#include "CEffectDefend.h"

using namespace std;

CEffectDefend::CEffectDefend(size_t PowerOfEffect) : CEffect(PowerOfEffect) {}

unique_ptr<CEffect> CEffectDefend::Clone() const {
    return make_unique<CEffectDefend>(*this);
}

EEffectType CEffectDefend::Type() const {
    return EEffectType::DEFENSIVE;
}

string CEffectDefend::ToString() const {
    return "Defense: " + to_string(m_PowerOfEffect);
}

CEffect &CEffectDefend::ApplyEffect(CStats &owner, CStats &opponent) {
    // Ignoring param
    static_cast<void>(opponent);

    owner.IncreaseShield(m_PowerOfEffect);
    return *this;
}