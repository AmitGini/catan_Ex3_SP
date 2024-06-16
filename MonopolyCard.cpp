#include "MonopolyCard.hpp"
#include "Player.hpp"

namespace catan_game
{
    MonopolyCard::MonopolyCard()
    {
        points = 0;
    }

    std::string MonopolyCard::getName() const {
        return "Monopoly";
    }

    int MonopolyCard::getPoints() const {
        return this->points;
    }
}