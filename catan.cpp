#ifndef RUN_TESTS

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_map>
#include <stdlib.h>

#include "Player.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Board.hpp"
#include "Tile.hpp"
#include "Card.hpp"
#include "KnightCard.hpp"
#include "VictoryPointCard.hpp"
#include "YearOfPlentyCard.hpp"
#include "RoadCard.hpp"
#include "MonopolyCard.hpp"

#ifdef _WIN32
    #define CLEAR "cls"
#elif __APPLE__ || __unix__
    #define CLEAR "clear"
#else
    #error "OS not supported!"
#endif

using catan_game::Vertex;
using catan_game::Edge;
using catan_game::Player;
using catan_game::Tile;
using catan_game::Board;
using catan_game::TileType;
using catan_game::Card;
using catan_game::KnightCard;
using catan_game::VictoryPointCard;
using catan_game::YearOfPlentyCard;
using catan_game::RoadCard;
using catan_game::MonopolyCard;
using std::vector;

Board *board = catan_game::Board::getBoardInstance(); // Create a board
vector<Player*> players;  // Create a vector of players
vector<Card*> deckCards; // Create a vector of development cards
vector<Card*> usedCards; // Create a vector of used development cards
void initPlayers();
void initPlayersSettlements();
void initPlayersRoads();
void initCardsDeck();

void playerOptions(Player* player);
bool buildSettlementCityOption(Player* player);
void buildRoadOption(Player *player);
bool cardsOptions(Player *player);
void openTrade(Player *player);
void yearOfPlentyCardOptions(Player *player);

Card* drawCard(Player* player);
bool tryBuildSettelment(Player *player, bool isCity, bool startGame);
bool tryBuildRoad(Player *player, bool startGame);
bool playRoadBuildingCard(Player* player);
bool playMonopolyCard(Player* player);

void startGame();
bool isGameOver();

void printCurrentGameData();
void printMyGameData(Player* player);

void deletePlayers();
void deleteCards();

int main() {
    
    initPlayers(); // Create the players
    printCurrentGameData(); // print the board and the players
    initPlayersSettlements(); // ask for the first settlements from the players
    initPlayersSettlements();
    initPlayersRoads(); // ask for the first roads from the players
    initPlayersRoads();
    

    initCardsDeck(); // initialize the development cards
    board->sendStartingResources(); // send the resources to the players according to the first settlements
    printCurrentGameData(); // print the board and the players
    startGame(); // start the game, end if someone wins

    deletePlayers(); // free the players
    deleteCards(); // free the development cards
    delete board; // delete the board

    return 0;
}

void initPlayers() {
    
    std::cout<<"Note! empty name inserted, player will be assign with default name! "<<std::endl;
    std::string playerOneName;
    std::cout<<"Enter Player 1 name: ";
    std::cin>> playerOneName;

    if(playerOneName.empty())
    {
        playerOneName = "Player 1";
    }

    Player* playerOne = new Player(playerOneName);
    size_t oneRoll = playerOne->rollDice();
    std::cout<<"Player 1 rolled: "<<oneRoll<<std::endl;

    std::string playerTwoName;
    std::cout<<"Enter Player 2 name: ";
    std::cin>>playerTwoName;

    if(playerTwoName.empty() || playerTwoName.compare(playerOneName) == 0)
    {
        playerTwoName = "Player 2";
    }

    Player* playerTwo = new Player(playerTwoName);
    size_t twoRoll = playerTwo->rollDice();
    std::cout<<"Player 2 rolled: "<<twoRoll<<std::endl;
    
    std::string playerThreeName;
    std::cout<<"Enter Player 3 name: ";
    std::cin>>playerThreeName;

    if(playerThreeName.empty() || playerThreeName.compare(playerOneName) == 0 || playerThreeName.compare(playerTwoName) == 0)
    {
        playerThreeName = "Player 3";
    }

    Player* playerThree = new Player(playerThreeName);
    size_t threeRoll = playerThree->rollDice();
    std::cout<<"Player 3 rolled: "<<threeRoll<<std::endl;

    if(oneRoll >= twoRoll && oneRoll >= threeRoll)
    {
        players.push_back(playerOne);
        if(twoRoll >= threeRoll)
        {
            players.push_back(playerTwo);
            players.push_back(playerThree);
        }
        else
        {
            players.push_back(playerThree);
            players.push_back(playerTwo);
        }
    }
    else if(twoRoll >= oneRoll && twoRoll >= threeRoll)
    {
        players.push_back(playerTwo);
        if(oneRoll >= threeRoll)
        {
            players.push_back(playerOne);
            players.push_back(playerThree);
        }
        else
        {
            players.push_back(playerThree);
            players.push_back(playerOne);
        }
    }
    else
    {
        players.push_back(playerThree);
        if(oneRoll >= twoRoll)
        {
            players.push_back(playerOne);
            players.push_back(playerTwo);
        }
        else
        {
            players.push_back(playerTwo);
            players.push_back(playerOne);
        }
    }

    std::cout<<"Play order: "<<std::endl;
    for(Player* player: players)
    {
        std::cout<<player->getUsername()<<", ";
    }
    std::cout<<std::endl;
}

void initPlayersSettlements()
{
    for(Player* player: players){
        for(;;)
        {
            if(tryBuildSettelment(player, false, true))
            {
                break;
            }else{
                std::cout<<"Invalid settlement, try again"<<std::endl;
            }
        }
    }
}

void initPlayersRoads()
{
    for(Player* player: players)
    {
        for(;;)
        {
            if(tryBuildRoad(player, true))
            {
                break;
            }else{
                std::cout<<"Invalid road, try again"<<std::endl;
            }
        }
    }
}

void initCardsDeck()
{
    // Add 3 Knight cards
    for (int index = 0; index < 3; ++index) {
        deckCards.push_back(new KnightCard());
    }
    // Add 4 Victory Point cards
    for (int index = 0; index < 4; ++index) {
        deckCards.push_back(new VictoryPointCard());
    }
    // Add 3 each of Year Of Plenty, Road Building, and Monopoly cards
    for (int index = 0; index < 3; ++index) { // Assuming 3 can be adjustem
        deckCards.push_back(new YearOfPlentyCard());
        deckCards.push_back(new RoadCard());
        deckCards.push_back(new MonopolyCard());
    }

    // Shuffle the deck
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deckCards.begin(), deckCards.end(), std::default_random_engine(seed));
}

Card* drawCard(Player* player) {
    if (deckCards.empty()) return nullptr;
    if(!(player->removeResourceForDevCard())) return nullptr;
    Card* card = deckCards.back();
    deckCards.pop_back();
    usedCards.push_back(card);
    return card;
}

void printMyGameData(Player* player)
{
    std::cout<<*player<<std::endl;
}

void printCurrentGameData()
{
    board->printBoard();
    for(Player* player: players)
    {
        std::cout<<*player<<std::endl;
    }
}

void startGame()
{
    for(;;)
    {
        for(Player* player: players)
        {
            playerOptions(player);
            if(isGameOver()) 
            {
                return;
            }
        }
    }
}

void playerOptions(Player* player)
{

    bool hasRolled = false;
    system(CLEAR);
    board->printBoard();
    printMyGameData(player);
    for(;;){
        std::cout<<player->getUsername()<<" Turn's"<<std::endl;
        std::cout<<"Options:"<<std::endl;
        std::cout<<"0. End Turn"<<std::endl;
        std::cout<<"1. Roll Dice"<<std::endl;
        std::cout<<"2. Build Road"<<std::endl;
        std::cout<<"3. Build Settlement Or City"<<std::endl;
        std::cout<<"4. Buy Development Card"<<std::endl;
        std::cout<<"5. Play Development Card"<<std::endl;
        std::cout<<"6. Trade"<<std::endl;
        std::cout<<"7. Print Map and My Current Game-Data"<<std::endl;
        std::vector<Card*> playerCards, removedCards;
        size_t diff;
        int choice;
        try
        {
            std::cout<<"Choice: ";
            std::cin>>choice;
            std::cout<<std::endl;
        }
        catch(std::invalid_argument& e)
        {
            std::cout<<"Invalid input"<<std::endl;
            continue;
        }

        switch(choice)
        {
            case 0:
                return;

            case 1:
                if(!hasRolled)
                {
                    size_t diceSumResult = player->rollDice();
                    hasRolled = true;
                    std::cout<<"***** Rolled: "<<diceSumResult<<" *****\n"<<std::endl;
                    if(diceSumResult == 7)
                    {
                        for(Player* player: players)
                        {
                            if(player->getNumOfResources() >= 7)
                            {
                                player->sevenPenalty();
                            }
                        }

                    }
                    else 
                        board->distrbuteResources(diceSumResult);
                }
                else 
                    std::cout<<"Already rolled the dice"<<std::endl;
                break;

            case 2:
                if(player->hasResourcesForRoad()) 
                    buildRoadOption(player);
                else 
                    std::cout<<"Not enough resources to build road"<<std::endl;
                break;

            case 3:
                if(buildSettlementCityOption(player))
                {
                    std::cout<<"Build Succefully"<<std::endl;
                }
                break;

            case 4:
                //implement player methods
                if(player->hasResourcesForDevelopmentCard())
                {
                    Card* card = drawCard(player);
                    if(card == nullptr)
                    {
                        std::cout<<"No more cards in the deck"<<std::endl;
                        break;
                    }
                    else
                    {
                        
                        diff = player->getMyDevelopmentCards().size();
                        playerCards = player->addDevelopmentCard(card);
                        if(playerCards.size() == diff)
                        {
                            throw std::invalid_argument("Error in adding card to player - card not added");

                        }else if(playerCards.size() - diff > 1)
                        {
                            for(Card* card: playerCards)
                            {
                                if(card->getName().compare("Largest Army") == 0)
                                {
                                    std::cout<<"\n**** Development Card: "<<card->getName()<<" ****"<<std::endl;
                                    std::cout<<"\n**** Congratulation you have gained the Largest Army Card ****\n"<<std::endl;
                                }
                            }
                        }else{
                            std::cout<<"\n**** Development Card: "<<card->getName()<<" ****"<<std::endl;
                        }
                    }
                }
                else
                    std::cout<<"Not enough resources to buy development card"<<std::endl;
                break;

            case 5:
                if(player->getMyDevelopmentCards().empty())
                {
                    std::cout<<"No development cards to play"<<std::endl;
                    break;
                }
                else
                {
                    if(cardsOptions(player)) return;
                }
                break;

            case 6:
                openTrade(player);
                break;

            case 7:
                board->printBoard();
                printMyGameData(player);
                break;


            default:
                break;
        }
    }
}

void buildRoadOption(Player *player)
{
    for(;;)
    {
        std::cout<<"Road Options:"<<std::endl;
        std::cout<<"1. Place Road"<<std::endl;
        std::cout<<"2. Cancel"<<std::endl;
        
        int choice;
        try
        {
            std::cin>>choice;
        }
        catch(std::invalid_argument& e)
        {
            std::cout<<"Invalid input"<<std::endl;
            continue;
        }

        switch(choice)
        {
            case 1:
                if(tryBuildRoad(player, false)) return;
                break;

            case 2:
                return;

            default:
                break;
        }
    }
}

bool tryBuildRoad(Player *player, bool freeFromResource)
{
    std::string fromRowStr, fromColStr, toRowStr, toColStr;
    std::cout<<"\n"<<player->getUsername()<<" Turn's"<<std::endl;
    std::cout<<"From row: ";
    std::cin>>fromRowStr;
    std::cout<<"From column: ";
    std::cin>>fromColStr;
    std::cout<<"To row: ";
    std::cin>>toRowStr;
    std::cout<<"To column: ";
    std::cin>>toColStr;

    // try to parse to int
    int fromRow, fromCol, toRow, toCol;
    try{
        fromRow = std::stoi(fromRowStr);
        fromCol = std::stoi(fromColStr);
        toRow = std::stoi(toRowStr);
        toCol = std::stoi(toColStr);
    }catch(std::invalid_argument& e){
        std::cout<<"Invalid input"<<std::endl;
        return false;
    }

    // check if the vertex is buildable
    Edge* edge = board->placeRoad(fromRow,fromCol,toRow, toCol, player, freeFromResource);
    if(edge != nullptr)
    {
        printCurrentGameData();
        std::cout<<player->getUsername()<<": Road placed in: "<<*edge<<std::endl;
        return true;
    }

    return false;
}

bool playRoadBuildingCard(Player* player)
{
    std::cout<<"**** Note! Once you start building you cant cancel ****"<<std::endl;
    std::cout<<"**** Note! You will have 3 chances to place your roads! ****"<<std::endl;
    std::cout<<"1. Start Build"<<std::endl;
    std::cout<<"2. Cancel"<<std::endl;
    std::cout<<"Enter your choice: ";
    int choice;
    try
    {
        std::cin>>choice;
    }
    catch(std::invalid_argument& e)
    {
        std::cout<<"Invalid input"<<std::endl;
        return false;
    }
    
    int count = 0;
    for(;;)
    {
        switch(choice)
        {
            case 1:
                if(tryBuildRoad(player, true))
                {
                    board->printBoard();
                    count++;
                    if(count == 2) return true;
                }
                break;

            case 2:
                return false;

            default:
                return false;
        }
    }
    return false;
}

bool playMonopolyCard(Player* player)
{
    for(;;)
    {
        std::cout<<"Monopoly Card Options:"<<std::endl;
        std::cout<<"1. Ask for Resource"<<std::endl;
        std::cout<<"2. Ask for Card"<<std::endl;
        std::cout<<"3. Cancel"<<std::endl;
        std::cout<<"Invalid argument will lead to lost of the card"<<std::endl;
        std::cout<<"Note! If the other player does not have the resource or the card you want, you will get nothing"<<std::endl;
        std::vector<Card*> addedCards, removedCards;
        std::string demand;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int choice;
        try
        {
            std::cin>>choice;
        }
        catch(std::invalid_argument& e)
        {
            std::cout<<"Invalid input"<<std::endl;
            continue;
        }

        TileType type;
        std::unordered_map<TileType,int> resource;
        switch(choice)
        {
            case 1:
                std::cout<<"Enter the Resource you want to ask for: ";
                std::cout<<"Resources Options: Tree, Clay, Crop, Wool, Iron"<<std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                std::cin>>demand;
                type = catan_game::stringToTileType(demand);
                for(Player* otherPlayer: players)
                {
                    if(player != otherPlayer){
                        resource = otherPlayer->getMyResources();
                        if(resource[type] > 0)
                        {
                            player->addResources(type ,1);
                            otherPlayer->removeResources(type,1);
                        }else{
                            std::cout<<"\n"<<otherPlayer->getUsername()<<" Does not habe the Resource you asked for"<<std::endl;
                        }
                    }
                }
                return true;

            case 2:
                std::cout<<"Enter the Card you want to ask for: ";
                std::cout<<"Cards Options: Year of Plenty, Road Building, Monopoly, Knight"<<std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::cin>>demand;
                for(Player* otherPlayer: players)
                {
                    if(player != otherPlayer){
                        for(auto card : otherPlayer->getMyDevelopmentCards())
                        {
                            if(card->getName().compare(demand) == 0)
                            {
                                removedCards = otherPlayer->removeDevelopmentCard(card);
                                if(removedCards.empty())
                                {
                                    throw std::invalid_argument("Error in removing card from player - card not removed");
                                }else if(removedCards.size() > 1){
                                    for(Card* card: removedCards)
                                    {
                                        if(card->getName().compare("Largest Army") == 0)
                                        {
                                            std::cout<<"\n**** "<<otherPlayer->getUsername()<<" has lost the Largest Army Card ****\n"<<std::endl;
                                        }
                                    }
                                }

                                addedCards = player->addDevelopmentCard(card);
                                if(addedCards.size() > 1)
                                {
                                    for(Card* card: addedCards)
                                    {
                                        if(card->getName().compare("Largest Army") == 0)
                                        {
                                            usedCards.push_back(card);
                                            std::cout<<"\n**** Congratulation you have gained the Largest Army Card ****\n"<<std::endl;
                                        }
                                    }
                                }
                                
                            }else{
                                std::cout<<"\n"<<otherPlayer->getUsername()<<" Does not have the Card you asked for"<<std::endl;
                            }
                        }
                    }
                }
                return true;

            case 3:
                return false;
            
            default:
                    break;
        }
    }
    return false;
}

bool buildSettlementCityOption(Player *player)
{
    for(;;){
        std::cout<<"Settlement Options:"<<std::endl;
        std::cout<<"1. Place Settlement"<<std::endl;
        std::cout<<"2. Place City"<<std::endl;
        std::cout<<"3. Cancel"<<std::endl;
        int choice;
        try
        {
            std::cin>>choice;
        }
        catch(std::invalid_argument& e)
        {
            std::cout<<"Invalid input"<<std::endl;
            continue;
        }

        switch(choice)
        {
            case 1:
                if(player->hasResourcesForSettlement())
                {   
                    if(tryBuildSettelment(player, false, false)) 
                    {
                        return true;
                    }
                }

                std::cout<<"Not enough resources to build settlement"<<std::endl;
                break;

            case 2:
                if(player->hasResourcesForCity())
                {
                    if(tryBuildSettelment(player, true, false))
                    {
                        return true;
                    }
                }
                std::cout<<"Not enough resources to build city"<<std::endl;
                break;

            case 3:
                return false;

            default:
                break;
        }
    }
    return false;
}

bool tryBuildSettelment(Player* player, bool isCity, bool startGame)
{
    std::string rowStr, colStr;
    std::cout<<"Enter row: ";
    std::cin>>rowStr;
    std::cout<<"Enter col: ";
    std::cin>>colStr;

    // try to parse to int
    int row, col;
    try{
        row = std::stoi(rowStr);
        col = std::stoi(colStr);
    }catch(std::invalid_argument& e){
        std::cout<<"Invalid input"<<std::endl;
        return false;
    }

    // check if the vertex is buildable
    Vertex* ver = board->placeSettlement(row, col, player, isCity, startGame);
    if(ver != nullptr)
    {
        std::cout<<player->getUsername()<<": Settlement placed in: "<<*ver<<std::endl;
        return true;
    }
    return false;
}

bool cardsOptions(Player *player)
{

    std::cout<<"Development Cards Options:"<<std::endl;
    player->printMyCards();
    std::cout<<"1. Play Road Building Card"<<std::endl;
    std::cout<<"2. Play Year of Plenty Card"<<std::endl;
    std::cout<<"3. Play Monopoly Card"<<std::endl;
    std::cout<<"4. Cancel"<<std::endl;

    int choice;
    try
    {
        std::cin>>choice;
    }
    catch(std::invalid_argument& e)
    {
        std::cout<<"Invalid input"<<std::endl;
        return false;
    }

    switch(choice)
    {
        case 1:
            for(auto playerCard: player->getMyDevelopmentCards())
            {
                if(playerCard->getName().compare("Road Building") == 0)
                {
                    if(playRoadBuildingCard(player))
                    {
                        player->removeDevelopmentCard(playerCard);
                        return true; // road building card get 2 roads and finish its turn
                    }
                    break;
                }
            }
            std::cout<<"No Road Building Card"<<std::endl;
            break;

        case 2:
            for(auto playerCard: player->getMyDevelopmentCards())
            {
                if(playerCard->getName().compare("Year of Plenty") == 0)
                {                   
                    yearOfPlentyCardOptions(player);
                    player->removeDevelopmentCard(playerCard);
                    return false; // year of plenty get resources and continue its turn
                }
                break;
            }

            std::cout<<"No Year of Plenty Card"<<std::endl;
            break;

        case 3:
            for(auto playerCard: player->getMyDevelopmentCards())
            {
                if(playerCard->getName().compare("Monopoly") == 0)
                {
                    playMonopolyCard(player);
                    player->removeDevelopmentCard(playerCard);
                    return true; // Monopoly get resource or card from player and finish its turn
                }
            }
            std::cout<<"No Monopoly Card"<<std::endl;
            break;

        case 4:
            return false;

        default:
            break;
    }
    return false;
}


void openTrade(Player* player)
{
    std::cout<<"Trade:"<<std::endl;
    std::cout<<"**** Note! You can only offer or demand one resource at a time ****"<<std::endl;
    std::cout<<"**** Note! Invalid input will lead to cancel the trade ****"<<std::endl;
    std::string offer, demand, trade;
    int numResOffer, numResDemand, accept;
    std::unordered_map<TileType, int> playerRes, otherRes;

    std::cout<<"\nEnter the Resource you want to offer: ";
    std::cout<<"Resources Options: Tree, Clay, Crop, Wool, Iron"<<std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin>>offer;
    std::cout<<"Enter amount: ";
    try
    {
        std::cin>>numResOffer;
    }
    catch(const std::exception& e)
    {
        std::cout<<"Invalid number"<<std::endl;
        return;
    }

    TileType typeOffer = catan_game::stringToTileType(offer);
    if(typeOffer == TileType::Sand)
    {
        std::cout<<"Invalid Resource Offer"<<std::endl;
        return;
    }

    std::cout<<"Enter the Resource you want to demand: ";
    std::cout<<"Resources Options: Tree, Clay, Crop, Wool, Iron"<<std::endl;
    std::cin>>demand;
    std::cout<<"Enter amount: ";
    try
    {
        std::cin>>numResDemand;
    }
    catch(const std::exception& e)
    {
        std::cout<<"Invalid number"<<std::endl;
        return;
    }

    TileType typeDemand = catan_game::stringToTileType(demand);
    if(TileType::Sand == typeDemand)
    {
        std::cout<<"Invalid Resource Demand"<<std::endl;
        return;
    }

    else if(numResDemand <= 0 || numResOffer <= 0)
    {
        std::cout<<"Invalid Number of resources"<<std::endl;
        return;
    }

    playerRes = player->getMyResources();

    auto itPlayer = playerRes.find(typeOffer);
    if(itPlayer == playerRes.end())
    {
            std::cout<<"Invalid Resource Demand"<<std::endl;
            return;
    }
    if(itPlayer->second < numResOffer)
    {
        std::cout<<"Not enough resources to offer"<<std::endl;
        return;
    }

    std::cout<<"\n****Offer: "<<numResOffer<<" "<<offer<<" For: "<<numResDemand<<" "<<demand<<"****"<<std::endl;
    for(Player* otherPlayer: players)
    {
        otherRes = otherPlayer->getMyResources();
        auto itOther = otherRes.find(typeDemand);
        if(itOther == otherRes.end())
        {
            std::cout<<"Invalid Resource Demand"<<std::endl;
            return;
        }
        if(itOther->second < numResDemand)
        {
            continue;
        }
        if(!(otherPlayer == player))
        {
            std::cout<<"Hey, "<<otherPlayer->getUsername()<<" Trade request arrived from"<<player->getUsername()<<" To accept the offer enter '1' or anything else to cancel: ";
            try
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin>>accept;
            }
            catch(const std::exception& e)
            {
                std::cout<<"Invalid number"<<std::endl;
                continue;
            }

            if(accept == 1)
            {
                std::cout<<"Test offer: "<<tileTypeToString(typeOffer)<<" Test Demand: "<<tileTypeToString(typeDemand)<<" Test num Offer: "<<numResOffer<<" Test num Demand: "<<numResDemand<<std::endl;
                otherPlayer->addResources(typeOffer, numResOffer);
                player->addResources(typeDemand, numResDemand);
                player->removeResources(typeOffer, numResOffer);
                otherPlayer->removeResources(typeDemand, numResDemand);
                
                std::cout<<"Trade Succeeded"<<std::endl;
                return;
            }
        }
        std::cout<<"Player has not accepted or dont have the resources you demand!"<<std::endl;
            
    }

   std::cout<<"\n Other Players might not have the resources you asked for.\n"<<std::endl;
}

void yearOfPlentyCardOptions(Player *player)
{
    for(;;)
    {
        std::cout << "Choose two resources to take: Clay, Tree, Wool, Crop, Iron\n";
        std::string resource1;
        std::string resource2;
        std::cout<<"Enter the first resource: ";
        std::cin >> resource1;
        std::cout<<"Enter the second resource: ";
        std::cin >> resource2;
        TileType type1;
        TileType type2;
        try
        {
            type1 = catan_game::stringToTileType(resource1); 
            type2 = catan_game::stringToTileType(resource2);
            if(type1 == TileType::Sand || type2 == TileType::Sand)
            {
                throw std::invalid_argument("Invalid resource type.");
            }
        }
        catch(const std::invalid_argument& e)
        {
            std::cout << "Invalid resource type." << std::endl;
            continue;
        }
        
        player->addResources(type1, 1);
        player->addResources(type2, 1);
        std::cout<<resource1<<", "<<resource2<<" added successfully.\n";
        break;
    }
}

bool isGameOver()
{
    //Todo: implement counter for the players that have 10 points - 
    //consider: buildings, longest road, largest army, etc.
    for(Player* player: players)
    {
        if(player->getMyPoints() >= 10)
        {
            std::cout<<player->getUsername()<<" wins!"<<std::endl;
            return true;
        }
    }
    return false;
}

// Free the players
void deletePlayers()
{
    for(Player* player: players)
    {
        if(player != nullptr){
            delete player;
            player = nullptr;
        }
    }
}

// Free the development cards
void deleteCards()
{
    for(Card* deckCard: deckCards)
    {
        if(deckCard != nullptr)
        {
            delete deckCard;
            deckCard = nullptr;
        }
    }

    for(Card* usedCard: usedCards)
    {
        if(usedCard != nullptr){
            delete usedCard;
            usedCard = nullptr;
        }
    }
}
#endif