#ifndef ROADCARD_HPP
#define ROADCARD_HPP

#include "Card.hpp"
#include "Player.hpp"
#include <iostream>

namespace catan_game
{
    class RoadCard : public Card {
        int points;
    public:
        RoadCard();
        std::string getName() const override;
        int getPoints() const override;
    };
}
#endif 
