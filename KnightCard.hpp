#ifndef KNIGHTCARD_HPP
#define KNIGHTCARD_HPP

#include <string>
#include "Card.hpp"
#include "Player.hpp"

namespace catan_game
{
    class KnightCard : public Card {
        std::string cardName;
        int points;
    public:
        KnightCard();
        std::string getName() const override;
        int getPoints() const override;
    };
}
#endif 
