#include "CStats.h"

CStats::CStats(size_t Health, size_t Shield, size_t Mana) : m_Health(Health), m_Shield(Shield), m_Mana(Mana) {}

CStats &CStats::TakeDamage(size_t powerOfCard) {
    if (m_Shield == 0) {
        if (m_Health <= powerOfCard) {
            m_Health = 0;
            return *this;
        }

        m_Health -= powerOfCard;
        return *this;
    }

    if (m_Shield >= powerOfCard) {
        m_Shield -= powerOfCard;
        return *this;
    }

    powerOfCard -= m_Shield;
    m_Shield = 0;

    if (m_Health <= powerOfCard) {
        m_Health = 0;
        return *this;
    }

    m_Health -= powerOfCard;
    return *this;
}

CStats &CStats::IncreaseShield(size_t powerOfCard) {
    m_Shield += powerOfCard;
    return *this;
}

CStats &CStats::Heal(size_t powerOfCard) {
    m_Health += powerOfCard;
    return *this;
}

CStats &CStats::IncreaseMana(size_t powerOfCard) {
    m_Mana += powerOfCard;
    return *this;
}

CStats & CStats::DecreaseMana(size_t powerOfCard) {
    m_Mana -= powerOfCard;
    return *this;
}

size_t CStats::GetHealth() const {
    return m_Health;
}

size_t CStats::GetShield() const {
    return m_Shield;
}

size_t CStats::GetMana() const {
    return m_Mana;
}