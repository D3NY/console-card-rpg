#include "CPlayerHuman.h"

using namespace std;

CPlayerHuman::CPlayerHuman(string PlayerNickname, const shared_ptr<CInterface> &Interface)
        : CPlayer(move(PlayerNickname)), m_Interface(Interface) {}

CPlayerHuman::CPlayerHuman(string PlayerNickname, CStats PlayerStats, const shared_ptr<CInterface> &Interface)
        : CPlayer(move(PlayerNickname), PlayerStats), m_Interface(Interface) {}

unique_ptr<CPlayer> CPlayerHuman::Clone() const {
    return make_unique<CPlayerHuman>(*this);
}

CCard CPlayerHuman::Move(const shared_ptr<CPlayer> &opponent) {
    // Ignoring param (only a robot uses the opponent's attributes)
    static_cast<void>(opponent);

    size_t playerCard = m_Interface->PromptPlayerCard();
    while (true) {
        // Player is picking 1-X, but vector has indexes 0-X
        playerCard--;
        if (playerCard >= m_Cards.size()) {
            m_Interface->InvalidCardSelected();
            playerCard = m_Interface->PromptPlayerCard();
            continue;
        }

        if (m_Stats.GetMana() < m_Cards.at(playerCard).GetCardCost()) {
            m_Interface->LackOfMana();
            playerCard = m_Interface->PromptPlayerCard();
            continue;
        }

        break;
    }

    auto it = m_Cards.begin();
    advance(it, playerCard);

    return SelectCard(it);
}

string CPlayerHuman::Type() const {
    return "Human";
}