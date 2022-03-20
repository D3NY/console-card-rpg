#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "CPlayer.h"
#include "CCard.h"

/**
 * A class representing a robot player
 */
class CPlayerRobot : public CPlayer {
public:
    CPlayerRobot();

    explicit CPlayerRobot(CStats PlayerStats);

    explicit CPlayerRobot(std::string PlayerName);

    std::unique_ptr<CPlayer> Clone() const override;

    CCard Move(const std::shared_ptr<CPlayer> &opponent) override;

    std::string Type() const override;


private:
    /** Move delay in milliseconds */
    constexpr static size_t m_MinDelay = 1200;
    constexpr static size_t m_MaxDelay = 3000;

    /**
     * Calculates the average card price from the cards a player has in his hand
     * @return Average card price
     */
    size_t CalculateAvgCardCost() const;
};