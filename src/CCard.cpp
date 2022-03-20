#include "CCard.h"

using namespace std;

CCard::CCard(size_t CardID, string CardName, size_t CardCostMana)
        : m_ID(CardID), m_Name(move(CardName)), m_Cost(CardCostMana) {}

CCard::CCard(const CCard &other)
        : m_ID(other.m_ID), m_Name(other.m_Name), m_Cost(other.m_Cost) {

    for (const auto &effect : other.m_Effects) {
        m_Effects.push_back(effect->Clone());
    }
}

CCard &CCard::operator=(CCard other) {
    swap(m_ID, other.m_ID);
    swap(m_Name, other.m_Name);
    swap(m_Cost, other.m_Cost);
    swap(m_Effects, other.m_Effects);
    return *this;
}

CCard &CCard::AddEffect(unique_ptr<CEffect> effect) {
    m_Effects.push_back(move(effect));
    return *this;
}

CCard &CCard::UseCard(CStats &owner, CStats &opponent) {
    // The card has no effect
    if (m_Effects.empty()) {
        return *this;
    }

    // Applies all effects to the player or opponent depending on the card effect
    for (const auto &it : m_Effects) {
        it->ApplyEffect(owner, opponent);
    }

    // Depending on the price of the card reduce mana to the owner for using the card
    owner.DecreaseMana(m_Cost);
    return *this;
}

const vector<shared_ptr<CEffect>> &CCard::GetEffects() const {
    return m_Effects;
}

const string &CCard::GetCardName() const {
    return m_Name;
}

size_t CCard::GetCardCost() const {
    return m_Cost;
}

size_t CCard::GetCardID() const {
    return m_ID;
}