#include "CEffect.h"

using namespace std;

CEffect::CEffect(size_t PowerOfEffect) : m_PowerOfEffect(PowerOfEffect) {}

size_t CEffect::GetPowerOfEffect() const {
    return m_PowerOfEffect;
}