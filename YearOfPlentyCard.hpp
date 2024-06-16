#ifndef YEAROFPLENTYCARD_HPP
#define YEAROFPLENTYCARD_HPP

#include <iostream>
#include "Card.hpp"
#include "Player.hpp"

namespace catan_game
{
    class YearOfPlentyCard : public Card {
        int points;
    public:
        YearOfPlentyCard();
        std::string getName() const override;
        int getPoints() const override;
        void play(Player* player);
    };
}
#endif 
