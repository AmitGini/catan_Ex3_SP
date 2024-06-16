#include "LargestArmyCard.hpp"
#include "Player.hpp"

namespace catan_game
{
    LargestArmyCard::LargestArmyCard()
    {
        points = 2;
    }

    std::string LargestArmyCard::getName() const
    {
        return "Largest Army";
    }

    int LargestArmyCard::getPoints() const
    {
        return this->points;
    }
}