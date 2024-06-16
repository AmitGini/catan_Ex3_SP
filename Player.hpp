#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "Edge.hpp"
#include "Vertex.hpp"
#include "Tile.hpp"
#include "Card.hpp"

namespace catan_game {
    class Edge;
    class Vertex;
    class Tile;
    class Card;

    enum class TileType;
    
    class Player {
    private:
        std::string username;
        std::vector<Edge*> myRoads;
        std::vector<Vertex*> myBuildings;
        std::unordered_map<TileType, int> myResources;
        std::vector<Card*> myCards;
        int myPoints;



    public:
        // Constructor to initialize the username
        Player(const std::string& name);

        // Destructor 
        ~Player();

        // Method to get the player's username
        std::string getUsername() const;

        // return vector of all player roads
        const std::vector<Edge*>& getMyRoads() const;

        // return vector of all player buildings(settlemnt/city)
        const std::vector<Vertex*>& getMyBuildings() const;

        // return map of the player resources
        const std::unordered_map<TileType, int>& getMyResources() const;

        // return vector of all player development cards
        const std::vector<Card*>& getMyDevelopmentCards() const;

        // return the number of points the player has
        int getMyPoints() const;

        // return the number of resources the player has
        int getNumOfResources() const;

        // add pointer to vector roads, road is edge of player
        bool addRoad(Edge* road, bool isStartGame);

        //add pointer tovector buildings, building is vertex of player
        bool addBuilding(Vertex* building, bool isCity, bool isStartGame);

        //add resources to player
        void addResources(TileType type, int amount);

        //add development card to player
        const std::vector<Card*>& addDevelopmentCard(Card* card);

        //remove the resources needed to buy development card
        bool removeResourceForDevCard();

        //remove resources from player - case of card or trade
        void removeResources(TileType type, int amount);

        //remove development card to player
        const std::vector<Card*>& removeDevelopmentCard(Card* card);
        
        //If player rolled the sum of 7 in the dice, all player with more then 7 resources will lose half of their resources
        void sevenPenalty();

        //roll 2 dices and return the sum of the result
        size_t rollDice();
        
        //check if player has resources to build road
        bool hasResourcesForRoad() const;

        //check if player has resources to build settlement
        bool hasResourcesForSettlement() const;

        //check if player has resources to build city
        bool hasResourcesForCity() const;

        // Method to check if the player has enough resources to build a development card
        bool hasResourcesForDevelopmentCard() const;

        //operator ==, check equaliztion between 2 players
        bool operator==(const Player &other) const;

        //print the player resources and the number of each resource
        void printMyResources() const;

        //print the player cards and the number of each card
        void printMyCards() const;

        // how would like default printing of player using cout
        friend std::ostream& operator<<(std::ostream &stream, const Player& player);
    };

} 
#endif // PLAYER_HPP