#ifndef LARGESTARMYCARD_HPP
#define LARGESTARMYCARD_HPP

#include "Card.hpp"

namespace catan_game
{
    class LargestArmyCard : public Card {
        int points;
    public:
        LargestArmyCard();
        std::string getName() const override;
        int getPoints() const override;
    };
}
#endif 
