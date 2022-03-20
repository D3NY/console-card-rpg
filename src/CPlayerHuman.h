#pragma once

#include <string>
#include <memory>
#include <utility>

#include "CPlayer.h"
#include "CInterface.h"

/**
 * The class representing a human player
 */
class CPlayerHuman : public CPlayer {
public:
    CPlayerHuman(std::string PlayerNickname, const std::shared_ptr<CInterface> &Interface);

    CPlayerHuman(std::string PlayerNickname, CStats PlayerStats, const std::shared_ptr<CInterface> &Interface);

    std::unique_ptr<CPlayer> Clone() const override;

    CCard Move(const std::shared_ptr<CPlayer> &opponent) override;

    std::string Type() const override;


private:
    std::shared_ptr<CInterface> m_Interface;
};