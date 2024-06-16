#include "YearOfPlentyCard.hpp"
#include "Player.hpp"

namespace catan_game
{
    YearOfPlentyCard::YearOfPlentyCard()
    {
        points = 0;
    }

    std::string YearOfPlentyCard::getName() const 
    {
        return "Year of Plenty";
    }

    int YearOfPlentyCard::getPoints() const 
    {
        return this->points;
    }
}