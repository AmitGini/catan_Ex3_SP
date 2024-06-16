#include "KnightCard.hpp"
#include "Player.hpp"

namespace catan_game
{

    KnightCard::KnightCard() {
        cardName = "Knight";
        points = 0;
    }

    std::string KnightCard::getName() const {
        return this->cardName;
    }
    
    int KnightCard::getPoints() const
    {
        return this->points;
    }
}