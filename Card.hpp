#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include "Player.hpp"

namespace catan_game
{
    class Player; // Forward declaration
        
    class Card {
    public:
        
        virtual ~Card() {}  // Virtual destructor
        virtual int getPoints() const = 0; 
        virtual std::string getName() const = 0;

    };
}
#endif 
