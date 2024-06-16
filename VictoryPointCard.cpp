#include "VictoryPointCard.hpp"
#include "Player.hpp"
namespace catan_game
{
    VictoryPointCard::VictoryPointCard() {
        points = 1;
    }

    std::string VictoryPointCard::getName() const
    {
        return "Victory Point"; 
    }
    
    int VictoryPointCard::getPoints() const
    {
        return this->points;
    }
}