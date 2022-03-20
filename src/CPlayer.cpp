#include "CPlayer.h"

using namespace std;

CPlayer::CPlayer(string PlayerNickname) : m_Nickname(move(PlayerNickname)) {}

CPlayer::CPlayer(string PlayerNickname, CStats PlayerStats)
        : m_Nickname(move(PlayerNickname)), m_Stats(PlayerStats) {}

CCard CPlayer::SelectCard(vector<CCard>::iterator it) {
    CCard card = *it;
    m_Cards.erase(it);
    return card;
}

CPlayer &CPlayer::AddCard(const CCard &card) {
    m_Cards.push_back(card);
    return *this;
}

size_t CPlayer::GetEndurance() const {
    return m_Stats.GetHealth() + m_Stats.GetShield();
}

CStats &CPlayer::GetPlayerStats() {
    return m_Stats;
}

const string &CPlayer::GetPlayerNickname() const {
    return m_Nickname;
}

const vector<CCard> &CPlayer::GetPlayerCards() const {
    return m_Cards;
}

bool CPlayer::IsAlive() const {
    return m_Stats.GetHealth() > 0;
}

bool CPlayer::IsPossibleMove() const {
    // Checks if the player has enough mana for any card in the deck
    for (const auto &it : m_Cards) {
        if (it.GetCardCost() <= m_Stats.GetMana()) {
            return true;
        }
    }

    return false;
}

vector<CCard>::iterator CPlayer::SearchCard(EEffectType effectType, size_t opponentEndurance) {
    // Card position in the deck, by default set to the end
    auto cardPosition = m_Cards.end();

    // The strongest effect found so far
    size_t bestEffect = 0;

    for (auto itCard = m_Cards.begin(); itCard < m_Cards.end(); itCard++) {
        for (const auto &itEffect : itCard->GetEffects()) {
            if (itCard->GetCardCost() > m_Stats.GetMana() || itEffect->Type() != effectType) {
                continue;
            }

            if (itEffect->GetPowerOfEffect() >= opponentEndurance && itEffect->GetPowerOfEffect() > bestEffect) {
                bestEffect = itEffect->GetPowerOfEffect();
                cardPosition = itCard;
            }
        }
    }

    return cardPosition;
}

bool CPlayer::CardFound(std::vector<CCard>::iterator it) const {
    return it != m_Cards.end();
}