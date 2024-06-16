
#include "RoadCard.hpp"
#include "Player.hpp"

namespace catan_game
{
    RoadCard::RoadCard() {
        points = 0;
    }


    std::string RoadCard::getName() const {
        return "Road Building";
    }
    int RoadCard::getPoints() const
    {
        return this->points;
    }
}