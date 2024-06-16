#ifndef VICTORYPOINTCARD_HPP
#define VICTORYPOINTCARD_HPP

#include "Card.hpp"
namespace catan_game
{
    
    class VictoryPointCard : public Card {
        int points;
    public:
        VictoryPointCard();
        std::string getName() const override;
        int getPoints() const override;
    };
}
#endif 
