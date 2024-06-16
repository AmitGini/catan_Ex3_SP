#ifndef MONOPOLYCARD_HPP
#define MONOPOLYCARD_HPP

#include "Card.hpp"

namespace catan_game
{
    class MonopolyCard : public Card {
        int points;
    public:
        MonopolyCard();
        std::string getName() const override;
        int getPoints() const override;
    };
}
#endif 
