
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "Tile.hpp"
#include "Player.hpp"
#include "LargestArmyCard.hpp"

namespace catan_game {
    // Constructor to initialize the username
    Player::Player(const std::string& name) : username(name) 
    {
        // Initialize the resources
        this->myResources[TileType::Tree] = 0;
        this->myResources[TileType::Clay] = 0;
        this->myResources[TileType::Crop] = 0;
        this->myResources[TileType::Wool] = 0;
        this->myResources[TileType::Iron] = 0;
        this->myPoints = 0;
    }

    // Destructor to free the memory
    Player::~Player() 
    {
        for (auto* card : this->myCards) {
            if(card != nullptr)
            {
                if(card->getName() == "Largest Army")
                {
                    delete card;
                    card = nullptr;
                }
            }
        }
    }

    // Method to get the player's username
    std::string Player::getUsername() const 
    {
        return this->username;
    }

    // return vector of all the player edges with roads
    const std::vector<Edge*>& Player::getMyRoads() const
    {
        return this->myRoads;
    }

    // return vector of all the player vertex with buildings(settlement/city) 
    const std::vector<Vertex*>& Player::getMyBuildings() const
    {
        return this->myBuildings;
    }

    // return map of all player resources and the number of each resource
    const std::unordered_map<TileType, int>& Player::getMyResources() const
    {
        return this->myResources;
    }

    // return vector of all player development cards
    const std::vector<Card*>& Player::getMyDevelopmentCards() const
    {
        return this->myCards;
    }

    // return the number of points the player has
    int Player::getMyPoints() const
    {
        return this->myPoints;
    }

    // return the number of resources the player has
    int Player::getNumOfResources() const
    {
        int sumOfResources = 0;
        for(const auto& res: this->myResources)
        {
            sumOfResources += res.second;
        }
        return sumOfResources;
    }

    // add pointer to vector roads, road is edge of player
    bool Player::addRoad(Edge *road, bool isStartGame)
    {
        if(isStartGame)
        {
        this->myRoads.push_back(road);
        road->setRoad(this);
        return true;
        }
        else
        {
            if(this->hasResourcesForRoad())
            {
                this->myRoads.push_back(road);
                road->setRoad(this);
                this->removeResources(TileType::Clay, 1);
                this->removeResources(TileType::Tree, 1);
                return true;
            }
        }
        return false;
    }

    bool Player::addBuilding(Vertex* ver, bool isCity, bool isResouceCheckRequire)
    {
        if(isResouceCheckRequire)
        {
            ver->setOwner(this);
            ver->setSettlement();
            this->myBuildings.push_back(ver);
            this->myPoints += 1;
            return true;
        }
        else
        {
            if(isCity)
            {
                if(this->hasResourcesForCity() && ver->isSettlementBuildable(isCity))
                {
                    ver->setCity();
                    this->removeResources(TileType::Crop, 2);
                    this->removeResources(TileType::Iron, 3);
                    this->myPoints += 1;
                    return true;
                }
            }
            else
            {
                if(this->hasResourcesForSettlement() && ver->isSettlementBuildable(isCity))
                {
                    ver->setOwner(this);
                    ver->setSettlement();
                    this->myBuildings.push_back(ver);
                    this->removeResources(TileType::Tree, 1);
                    this->removeResources(TileType::Clay, 1);
                    this->removeResources(TileType::Wool, 1);
                    this->removeResources(TileType::Crop, 1);
                    this->myPoints += 1;
                    return true;
                }
            }
        }
        return false;
    }

    void Player::addResources(TileType type, int amount)
    {
        this->myResources[type] += amount;
    }

    bool Player::removeResourceForDevCard()
    {
        if(this->hasResourcesForDevelopmentCard())
        {
            this->removeResources(TileType::Crop, 1);
            this->removeResources(TileType::Iron, 1);
            this->removeResources(TileType::Wool, 1);
            return true;
        }
        return false;
    }

    const std::vector<Card*>& Player::addDevelopmentCard(Card* card) {
        if (!card) return this->myCards;

        this->myCards.push_back(card);

        if (card->getName() == "Knight") {
            int counter = 0;
            for (Card* card : this->myCards) {
                if (card->getName() == "Knight") {
                    ++counter;
                }
            }

            if (counter == 3) {
                LargestArmyCard* largestArmy = new LargestArmyCard(); // Dynamically allocate
                this->myCards.push_back(largestArmy);
                this->myPoints += largestArmy->getPoints();
            }
        }

        this->myPoints += card->getPoints();
        return this->myCards;
    }


    const std::vector<Card*>& Player::removeDevelopmentCard(Card* card) {
        if (!card) return this->myCards;

        // If the specific card wasn't found in the usual cards, handle special cases
        if (card->getName() == "Knight") {
            auto itSpecial = std::find_if(this->myCards.begin(), this->myCards.end(),
                                          [](Card* tempCard) { return tempCard->getName() == "Largest Army"; });

            if (itSpecial != this->myCards.end()) {
                this->myPoints -= (*itSpecial)->getPoints(); // Deduct points
                delete *itSpecial; // Free the memory
                this->myCards.erase(itSpecial); // Remove card from the list
            }
        }

        auto it = std::find_if(this->myCards.begin(), this->myCards.end(),
                               [&card](Card* tempCard) { return card->getName() == tempCard->getName(); });

        if (it != this->myCards.end()) {
            this->myPoints -= (*it)->getPoints(); // Deduct points
            this->myCards.erase(it); // Remove card from the list
        }

        return this->myCards;
    }


    void Player::removeResources(TileType type, int amount)
    {
        if(this->myResources[type] > 0){
            this->myResources[type] -= amount;
        }else{
            std::cout<<"You don't have enough resources of this type\n";
        }
    }

    void Player::sevenPenalty()
    {
        int numOfResources = this->getNumOfResources();
        int numOfResourcesToRemove = numOfResources / 2;
        for(;;)
        {
            if(numOfResourcesToRemove == 0)
            {
                break;
            }

            this->printMyResources();
            std::cout << "You have " << numOfResourcesToRemove << " resources to remove\n";
            std::cout << "Enter the resource you want to remove: ";
            std::string type;
            std::cin >> type;
            std::cout << "Enter the amount of resources you want to remove: ";
            int amount;
            TileType tileType;

            try{
                std::cin >> amount;
                
                if(type == "Tree"){
                    tileType = TileType::Tree;
                }else if(type == "Clay"){
                    tileType = TileType::Clay;
                }else if(type == "Crop"){
                    tileType = TileType::Crop; 
                }else if(type == "Wool"){
                    tileType = TileType::Wool;
                }else if(type == "Iron"){
                    tileType = TileType::Iron;
                }else{
                    throw std::invalid_argument("Invalid resource type");
                }
            }
            catch(const std::invalid_argument& e)
            {
                continue;
            }

            if(this->myResources[tileType] - amount >= 0 && numOfResourcesToRemove - amount >= 0)
            {
                this->removeResources(tileType, amount);
                numOfResourcesToRemove -= amount;
            }
            else
            {
                std::cout << "You don't have enough resources of this type\n";
            }
        }
    }

    // Method to check if the player has enough resources to build a road
    bool Player::hasResourcesForRoad() const
    {
        int numClay = this->myResources.find(TileType::Clay)->second;
        int numTree = this->myResources.find(TileType::Tree)->second;
        return (numClay >= 1 && numTree >= 1);
    }
    
    // Method to check if the player has enough resources to build a settlement
    bool Player::hasResourcesForSettlement() const
    {
        int numTree = this->myResources.find(TileType::Tree)->second;
        int numClay = this->myResources.find(TileType::Clay)->second;
        int numWool = this->myResources.find(TileType::Wool)->second;
        int numCrop = this->myResources.find(TileType::Crop)->second;
        return (numTree >= 1 && numClay >= 1 && numWool >= 1 && numCrop >= 1);
    }
    // Method to check if the player has enough resources to build a city
    bool Player::hasResourcesForCity() const
    {
        int numCrop = this->myResources.find(TileType::Crop)->second;
        int numIron = this->myResources.find(TileType::Iron)->second;
        return (numCrop >= 2 && numIron >= 3);
    }

    // Method to check if the player has enough resources to build a development card
    bool Player::hasResourcesForDevelopmentCard() const
    {
        int numCrop = this->myResources.find(TileType::Crop)->second;
        int numIron = this->myResources.find(TileType::Iron)->second;
        int numWool = this->myResources.find(TileType::Wool)->second;
        return (numCrop >= 1 && numIron >= 1 && numWool >= 1);
    }

    size_t Player::rollDice()
    {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(1, 6);
        int num1 = distrib(gen);
        int num2 = distrib(gen);

        return num1 + num2;
    }



    void Player::printMyResources() const
    {
        std::cout <<(this->username)<<"'s Resources: ";
        for(const auto& res: this->myResources)
        {
            std::cout <<res.first<<"-"<<res.second<<", ";
        }
        std::cout<<"\n";
    }

    void Player::printMyCards() const
    {
        for(Card* card: this->myCards)
        {
            std::cout<<"\nDevelopment Cards: "<<(*card).getName()<<", ";
        }
        std::cout<<"\n";
    }

    bool Player::operator==(const Player &other) const
    {
        return this->username.compare(other.getUsername()) == 0;
    }

    std::ostream &operator<<(std::ostream &stream, const Player &player)
    {
        stream <<player.getUsername()<<"'s Information"<<std::endl;
        stream<<"Points: "<<player.getMyPoints()<<std::endl;   
        stream<<"Roads: ";
        for(Edge* road: player.getMyRoads())
        {
            stream<<*road<<", ";
        }
     
        stream<<"\nBuildings: ";
        for(Vertex* building: player.getMyBuildings())
        {
            stream<<*building<<", ";
        }

        stream<<"\nResources: ";
        for(const auto& res: player.getMyResources())
        {
            stream<<res.first<<"-"<<res.second<<", ";
        }

        stream<<"\nDevelopment Cards: ";
        for(Card* card: player.getMyDevelopmentCards())
        {
            stream<<card->getName()<<", ";
        }
        stream<<"\n";
        return stream;
    }
}