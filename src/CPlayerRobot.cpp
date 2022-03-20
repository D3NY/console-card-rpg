#include "CPlayerRobot.h"

#include <chrono>
#include <thread>
#include <random>

using namespace std;

CPlayerRobot::CPlayerRobot() : CPlayer("Saltibart Fast") {}

CPlayerRobot::CPlayerRobot(CStats PlayerStats)
        : CPlayer("Saltibart Fast", PlayerStats) {}

CPlayerRobot::CPlayerRobot(string PlayerName) : CPlayer(move(PlayerName)) {}

unique_ptr<CPlayer> CPlayerRobot::Clone() const {
    return make_unique<CPlayerRobot>(*this);
}

size_t CPlayerRobot::CalculateAvgCardCost() const {
    size_t sum = 0;

    for (const auto &card : m_Cards) {
        sum += card.GetCardCost();
    }

    return sum / m_Cards.size();
}

CCard CPlayerRobot::Move(const shared_ptr<CPlayer> &opponent) {
    // Reference: en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    mt19937 randomGenerator(random_device{}());

    // Sleeps for x milliseconds to simulate a real player's move
    this_thread::sleep_for(chrono::milliseconds(uniform_int_distribution<>{m_MinDelay, m_MaxDelay}(randomGenerator)));

    // Find a card in the deck that can kill an opponent
    auto it = SearchCard(EEffectType::OFFENSIVE, opponent->GetEndurance());
    if (CardFound(it)) {
        return SelectCard(it);
    }

    // Find a card in the opponent's deck that can kill me
    it = opponent->SearchCard(EEffectType::OFFENSIVE, GetEndurance());
    if (opponent->CardFound(it)) {
        // An opponent can kill me, start defending
        it = SearchCard(EEffectType::DEFENSIVE);
        if (CardFound(it)) {
            return SelectCard(it);
        }
    }

    // Lack of mana for the next rounds, find a card that can restore mana
    if (m_Stats.GetMana() < CalculateAvgCardCost()) {
        it = SearchCard(EEffectType::UTILITY);
        if (CardFound(it)) {
            return SelectCard(it);
        }
    }

    // Enough HP and shields, attack an opponent
    it = SearchCard(EEffectType::OFFENSIVE);
    if (CardFound(it)) {
        return SelectCard(it);
    }

    // No suitable card was found, use the first card on which i have enough mana
    it = m_Cards.begin();
    while (it < m_Cards.end()) {
        if (it->GetCardCost() <= m_Stats.GetMana()) {
            break;
        }

        it++;
    }

    return SelectCard(it);
}

string CPlayerRobot::Type() const {
    return "Robot";
}