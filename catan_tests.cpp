#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <map>
#include <string>
#include <vector>

#include "doctest.h"
#include "Player.hpp"
#include "Tile.hpp"
#include "KnightCard.hpp"
#include "VictoryPointCard.hpp"
#include "YearOfPlentyCard.hpp"
#include "RoadCard.hpp"
#include "MonopolyCard.hpp"
#include "Board.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"
#include "Card.hpp"

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
using std::map;
using std::string;

using namespace catan_game;

// Player basic functionalities
TEST_CASE("Player initialization") {
    Player player("TestPlayer");
    CHECK(player.getUsername() == "TestPlayer");
    CHECK(player.getMyPoints() == 0);
}

TEST_CASE("Player rolling dice") {
    Player player("TestPlayer");
    for(int i = 0; i < 100; i++){
        size_t roll = player.rollDice();
        CHECK(roll >= 2);
        CHECK(roll <= 12);
    }
}

TEST_CASE("Player resources management") {
    Player player("TestPlayer");
    player.addResources(TileType::Tree, 3);
    player.addResources(TileType::Clay, 2);
    auto resources = player.getMyResources();
    CHECK(resources[TileType::Tree] == 3);
    CHECK(resources[TileType::Clay] == 2);
    player.removeResources(TileType::Tree, 1);
    resources = player.getMyResources();
    CHECK(resources[TileType::Tree] == 2);
    CHECK_NOTHROW(player.removeResources(TileType::Tree, 2));
    resources = player.getMyResources();
    CHECK(resources[TileType::Tree] == 0);
    player.removeResources(TileType::Tree, 1);
    CHECK(resources[TileType::Tree] == 0);
}

TEST_CASE("Player development cards management") {
    Player* testPlayer = new Player("TestPlayer");
    Card* knightCard = new KnightCard();
    Card* victoryCard = new VictoryPointCard();
    testPlayer->addDevelopmentCard(knightCard);
    testPlayer->addDevelopmentCard(victoryCard);
    auto devCards = testPlayer->getMyDevelopmentCards();
    CHECK(devCards.size() == 2);
    testPlayer->removeDevelopmentCard(knightCard);
    devCards = testPlayer->getMyDevelopmentCards();
    CHECK(devCards.size() == 1);
    delete testPlayer;
    testPlayer = nullptr;
    delete knightCard;
    knightCard = nullptr;
    delete victoryCard;
    victoryCard = nullptr;
}

TEST_CASE("Player checking resources availability") {
    Player player("TestPlayer");
    player.addResources(TileType::Tree, 1);
    player.addResources(TileType::Clay, 1);
    player.addResources(TileType::Crop, 1);
    player.addResources(TileType::Wool, 1);
    player.addResources(TileType::Iron, 1);
    CHECK(player.hasResourcesForRoad() == true);
    CHECK(player.hasResourcesForSettlement() == true);
    CHECK(player.hasResourcesForCity() == false);
    player.addResources(TileType::Iron, 2);
    player.addResources(TileType::Crop, 2);
    CHECK(player.hasResourcesForCity() == true);
}

TEST_CASE("Player seven penalty") {
    Player player("TestPlayer");
    player.addResources(TileType::Tree, 10);
    player.addResources(TileType::Clay, 5);
    player.sevenPenalty();
    auto resources = player.getMyResources();
    CHECK(player.getNumOfResources() == 8);
}

TEST_CASE("Player equality operator") {
    Player player("TestPlayer");
    Player player2("TestPlayer2");
    CHECK(player == player);
    CHECK_FALSE(player == player2);
}

TEST_CASE("Player resource removal for dev card") {
    Player player("TestPlayer");
    player.addResources(TileType::Tree, 1);
    player.addResources(TileType::Clay, 1);
    player.addResources(TileType::Crop, 1);
    player.addResources(TileType::Wool, 1);
    player.addResources(TileType::Iron, 1);
    CHECK(player.removeResourceForDevCard() == true);
    auto resources = player.getMyResources();
    CHECK(resources[TileType::Tree] == 1);
    CHECK(resources[TileType::Clay] == 1);
    CHECK(resources[TileType::Crop] == 0);
    CHECK(resources[TileType::Wool] == 0);
    CHECK(resources[TileType::Iron] == 0);
}

TEST_CASE("Player adding and checking roads and buildings") {
    Player player("TestPlayer");
    Vertex* ver1 = new Vertex(1, 1);
    Vertex* ver2 = new Vertex(2, 2);
    Edge edge(ver1, ver2);
    Vertex settlement(2, 2);
    Vertex city(3, 3);
    player.addRoad(&edge, true);
    CHECK(player.getMyRoads().size() == 1);
    CHECK(player.getMyRoads()[0] == &edge);
    player.addBuilding(&settlement, false, true);
    CHECK(player.getMyBuildings().size() == 1);
    CHECK(player.getMyBuildings()[0] == &settlement);
    player.addBuilding(&city, true, true);
    CHECK(player.getMyBuildings().size() == 2);
    CHECK(player.getMyBuildings()[1] == &city);
    delete ver1;
    delete ver2;
}

// Vertex basic functionalities
TEST_CASE("Vertex initialization") {
    Vertex vertex(1, 2);
    CHECK(vertex.getRow() == 1);
    CHECK(vertex.getColumn() == 2);
    CHECK(vertex.isSettled() == false);
    CHECK(vertex.isCity() == false);
}

TEST_CASE("Vertex setting owner and building settlement") {
    Vertex vertex(1, 2);
    Player player("TestPlayer");
    vertex.setOwner(&player);
    vertex.setSettlement();
    CHECK(vertex.getOwner() == &player);
    CHECK(vertex.isSettled() == true);
    CHECK(vertex.isCity() == false);
}

TEST_CASE("Vertex upgrading to city") {
    Vertex vertex(1, 2);
    Player player("TestPlayer");
    vertex.setOwner(&player);
    vertex.setSettlement();
    vertex.setCity();
    CHECK(vertex.getOwner() == &player);
    CHECK(vertex.isSettled() == true);
    CHECK(vertex.isCity() == true);
}

TEST_CASE("Vertex neighbor vertices and edges") {
    Vertex vertex(1, 2);
    Vertex neighbor1(1, 3);
    Vertex neighbor2(2, 2);
    std::vector<Vertex*> neighbors = {&neighbor1, &neighbor2};
    vertex.setMyVertexNeighbors(neighbors);
    auto vertexNeighbors = vertex.getMyVertexNeighbors();
    CHECK(vertexNeighbors.size() == 2);
    CHECK(vertexNeighbors[0] == &neighbor1);
    CHECK(vertexNeighbors[1] == &neighbor2);
    Edge edge1(&vertex, &neighbor1);
    Edge edge2(&vertex, &neighbor2);
    std::vector<Edge*> edges = {&edge1, &edge2};
    vertex.setMyEdges(edges);
    auto surroundingEdges = vertex.getMySurroundingEdges();
    CHECK(surroundingEdges.size() == 2);
    CHECK(surroundingEdges[0] == &edge1);
    CHECK(surroundingEdges[1] == &edge2);
}

// Edge basic functionalities
TEST_CASE("Edge initialization") {
    Vertex vertex1(1, 1);
    Vertex vertex2(1, 2);
    Edge edge(&vertex1, &vertex2);
    auto vertices = edge.getVertices();
    CHECK(vertices.first == &vertex1);
    CHECK(vertices.second == &vertex2);
    CHECK(edge.getRoadOwner() == nullptr);
    CHECK(edge.hasRoad() == false);
}

TEST_CASE("Edge setting road owner") {
    Vertex vertex1(1, 1);
    Vertex vertex2(1, 2);
    Edge edge(&vertex1, &vertex2);
    Player player("TestPlayer");
    edge.setRoad(&player);
    CHECK(edge.getRoadOwner() == &player);
    CHECK(edge.hasRoad() == true);
}

TEST_CASE("Edge neighbors") {
    Vertex vertex1(1, 1);
    Vertex vertex2(1, 2);
    Edge edge(&vertex1, &vertex2);
    Vertex vertex3(2, 1);
    Edge neighbor1(&vertex1, &vertex3);
    std::vector<Edge*> neighbors = {&neighbor1};
    edge.setMyNeighbors(neighbors);
    auto edgeNeighbors = edge.getNeighbours();
    CHECK(edgeNeighbors.size() == 1);
    CHECK(edgeNeighbors[0] == &neighbor1);
}

// Board basic functionalities
TEST_CASE("Board placing settlement") {
    Board* board = Board::getBoardInstance();
    Player player("TestPlayer");
    Vertex* vertex = board->placeSettlement(2, 9, &player, false, true);
    CHECK(vertex != nullptr);
    CHECK(vertex->getOwner() == &player);
    CHECK(vertex->isSettled() == true);
    CHECK(vertex->isCity() == false);
}

TEST_CASE("Board placing road") {
    Board* board = Board::getBoardInstance();
    Player player("TestPlayer");
    board->placeSettlement(0, 3, &player, false, true);
    board->placeSettlement(1, 4, &player, false, true);
    Edge* edge = board->placeRoad(0, 3, 0, 4, &player, true);
    CHECK(edge != nullptr);
    CHECK(edge->getRoadOwner() == &player);
    CHECK(edge->hasRoad() == true);
}

TEST_CASE("Board resource distribution") {
    Board* board = Board::getBoardInstance();
    Player player1("Player1");
    Player player2("Player2");
    Player player3("Player3");
    Vertex* vertex1 = board->placeSettlement(1, 1, &player1, false, true);
    Vertex* vertex2 = board->placeSettlement(2, 2, &player2, false, true);
    Vertex* vertex3 = board->placeSettlement(3, 3, &player3, false, true);
    board->sendStartingResources();
    CHECK(vertex1->getOwner()->getNumOfResources() > 0);
    CHECK(vertex2->getOwner()->getNumOfResources() > 0);
    CHECK(vertex3->getOwner()->getNumOfResources() > 0);
}

// Card functionalities
TEST_CASE("Card victory point card") {
    Player* player = new Player("TestPlayer");
    Card* card = new VictoryPointCard();
    int points = player->getMyPoints();
    CHECK(card->getName() == "Victory Point");
    CHECK(card->getPoints() == 1);
    player->addDevelopmentCard(card);
    CHECK(player->getMyPoints() == points + 1);
    CHECK(player->getMyDevelopmentCards().size() == 1);
    player->removeDevelopmentCard(card);
    CHECK(player->getMyPoints() == points);
    CHECK(player->getMyDevelopmentCards().size() == 0);
    delete card;
    card = nullptr;
    delete player;
}

TEST_CASE("Card year of plenty card") {
    Player* player = new Player("TestPlayer");
    Card* card = new YearOfPlentyCard();
    int points = player->getMyPoints();
    CHECK(card->getName() == "Year of Plenty");
    CHECK(card->getPoints() == 0);
    player->addDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 1);
    CHECK(player->getMyPoints() == points);
    player->removeDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 0);
    delete card;
    card = nullptr;
    delete player;
}

TEST_CASE("Card road card") {
    Player* player = new Player("TestPlayer");
    Card* card = new RoadCard();
    int points = player->getMyPoints();
    CHECK(card->getName() == "Road Building");
    CHECK(card->getPoints() == 0);
    player->addDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 1);
    CHECK(player->getMyPoints() == points);
    player->removeDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 0);
    delete card;
    card = nullptr;
    delete player;
}

TEST_CASE("Card monopoly card") {
    Player* player = new Player("TestPlayer");
    Card* card = new MonopolyCard();
    int points = player->getMyPoints();
    CHECK(card->getName() == "Monopoly");
    CHECK(card->getPoints() == 0);
    player->addDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 1);
    CHECK(player->getMyPoints() == points);
    player->removeDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 0);
    delete card;
    card = nullptr;
    delete player;
}

TEST_CASE("Card knight card") {
    Player* player = new Player("TestPlayer");
    Card* card = new KnightCard();
    int points = player->getMyPoints();
    CHECK(card->getName() == "Knight");
    CHECK(card->getPoints() == 0);
    player->addDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 1);
    CHECK(player->getMyPoints() == points);
    player->removeDevelopmentCard(card);
    CHECK(player->getMyDevelopmentCards().size() == 0);
    delete card;
    card = nullptr;
    delete player;
}

TEST_CASE("Card largest army - 3 knight cards add") {
    Player* player = new Player("TestPlayer");
    Card* card1 = new KnightCard();
    Card* card2 = new KnightCard();
    Card* card3 = new KnightCard();
    int points = player->getMyPoints();
    int currPoints = player->getMyPoints();
    CHECK(card1->getName() == "Knight");
    CHECK(card1->getPoints() == 0);
    player->addDevelopmentCard(card1);
    currPoints = player->getMyPoints();
    CHECK(player->getMyDevelopmentCards().size() == 1);
    CHECK(points == currPoints);
    player->addDevelopmentCard(card2);
    currPoints = player->getMyPoints();
    CHECK(player->getMyDevelopmentCards().size() == 2);
    CHECK(points == currPoints);
    player->addDevelopmentCard(card3);
    currPoints = player->getMyPoints();
    CHECK(player->getMyDevelopmentCards().size() == 4);  // 3 Knights + Largest Army
    CHECK(points + 2 == currPoints);  // 2 points for Largest Army
    std::vector<Card*> devCards = player->getMyDevelopmentCards();
    CHECK(devCards.size() == 4);
    int countKnight = 0, countLargestArmy = 0;
    for (Card* card : devCards) {
        if (card->getName() == "Knight") countKnight++;
        if (card->getName() == "Largest Army") countLargestArmy++;
    }
    CHECK(countKnight == 3);
    CHECK(countLargestArmy == 1);
    delete player;
}

TEST_CASE("Card largest army - 3 knight cards add and then remove 1") {
    Player* player = new Player("TestPlayer");
    KnightCard* card1 = new KnightCard();
    KnightCard* card2 = new KnightCard();
    KnightCard* card3 = new KnightCard();
    player->addDevelopmentCard(card1);
    player->addDevelopmentCard(card2);
    player->addDevelopmentCard(card3);
    CHECK(player->getMyDevelopmentCards().size() == 4);  // 3 Knights + Largest Army
    CHECK(player->getMyPoints() == 2);  // 2 points for Largest Army
    player->removeDevelopmentCard(card1);
    CHECK(player->getMyDevelopmentCards().size() == 2);
    CHECK(player->getMyPoints() == 0);  // Lost Largest Army
    player->addDevelopmentCard(card1);
    CHECK(player->getMyDevelopmentCards().size() == 4);
    CHECK(player->getMyPoints() == 2);  // No points for Knights
    delete player;
}
