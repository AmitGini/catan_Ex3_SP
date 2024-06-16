#include <algorithm>
#include <iomanip>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include <memory>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Board.hpp"

constexpr auto NUM_MATRIX_ROWS = 6;
constexpr auto NUM_MATRIX_COLS = 11;

namespace catan_game {

    // Static member initialization
    Board* Board::boardInstance = nullptr;

    // Constructor - private to prevent instantiation
    Board::Board()      
    { 
        initializeVertices(); // Initialize All the vertices of the board
        deleteAndUpdateInvalidVertex(); // Update the Invalid Verteics and delete the memory allocation
        updateVertexNeighbors(); // Update the neighbors of the vertices(Valid ones)
        initializeEdges(); // Initialize the edges of the board
        initializeTiles(); // Initialize the Tiles of the board and the type of the tiles with random selections
        assignNumbers(); // Assign the numbers to the tiles on the board - for dice rolls
        assignVertexToTiles(); // Assign the vertex to the tiles
    }

    Board::~Board() {
    for (auto& vertexRow : boardVertices) {
        for (auto vertex : vertexRow) {
            if(vertex != nullptr) {
                delete vertex;
                vertex = nullptr;
            }
        }
    }

    for (auto edge : boardEdges) {
        if(edge != nullptr)
        {
            delete edge;
            edge = nullptr;
        }
    }

    for (auto tile : boardTiles) {
        if(tile != nullptr)
        {
            delete tile;
            tile = nullptr;
        }
    }
}

    // static method - Singleton instance creation/return
    Board *Board::getBoardInstance()
    {
        if(boardInstance == nullptr){
            boardInstance = new Board();
        }
        return boardInstance;
    }

    const std::vector<Tile *> &Board::getTiles() const
    {
        return this->boardTiles;
    }

    // Initialize the verteics of the board
    void Board::initializeVertices()
    {
        boardVertices.resize(NUM_MATRIX_ROWS, std::vector<Vertex*>(NUM_MATRIX_COLS,nullptr));

        for(int row = 0; row < NUM_MATRIX_ROWS; ++row)
        {
            for(int col = 0; col < NUM_MATRIX_COLS; ++col)
            {
                    boardVertices[row][col] = new Vertex(row, col);
            }
        }
    }       
    
    // Update the Invalid Verteics and delete the memory allocation
    void Board::deleteAndUpdateInvalidVertex()
    {
        /* Set the invalid positions */
        //First row of the matrix
        int idxMin = 0;  // index of the first column and rows
        delete boardVertices[idxMin][idxMin];
        boardVertices[idxMin][idxMin] = nullptr;  // [0][0]
        delete boardVertices[idxMin][idxMin+1];
        boardVertices[idxMin][idxMin+1] = nullptr;  // [0][1]
        delete boardVertices[idxMin][NUM_MATRIX_COLS-2];
        boardVertices[idxMin][NUM_MATRIX_COLS-2] = nullptr;  // [0][9]
        delete boardVertices[idxMin][NUM_MATRIX_COLS-1];
        boardVertices[idxMin][NUM_MATRIX_COLS-1] = nullptr; // [0][10]

        //Second row of the matrix
        delete boardVertices[idxMin+1][idxMin];
        boardVertices[idxMin+1][idxMin] = nullptr;  //[1][0]
        delete boardVertices[idxMin+1][NUM_MATRIX_COLS-1];
        boardVertices[idxMin+1][NUM_MATRIX_COLS-1] = nullptr; //[1][10]

        //Fifth row of the matrix
        delete boardVertices[NUM_MATRIX_ROWS-2][idxMin];  //[5][1
        boardVertices[NUM_MATRIX_ROWS-2][idxMin] = nullptr;  //[5][0]
        delete boardVertices[NUM_MATRIX_ROWS-2][NUM_MATRIX_COLS-1];  //[5][1
        boardVertices[NUM_MATRIX_ROWS-2][NUM_MATRIX_COLS-1] = nullptr; //[5][10]

        //Sixth row of the matrix
        delete boardVertices[NUM_MATRIX_ROWS-1][idxMin];  //[10][0]
        boardVertices[NUM_MATRIX_ROWS-1][idxMin] = nullptr; //[10][0]
        delete boardVertices[NUM_MATRIX_ROWS-1][idxMin+1];  //[10][1]
        boardVertices[NUM_MATRIX_ROWS-1][idxMin+1] = nullptr; //[10][1]
        delete boardVertices[NUM_MATRIX_ROWS-1][NUM_MATRIX_COLS-2];  //[10][9]
        boardVertices[NUM_MATRIX_ROWS-1][NUM_MATRIX_COLS-2] = nullptr; //[10][9]
        delete boardVertices[NUM_MATRIX_ROWS-1][NUM_MATRIX_COLS-1];
        boardVertices[NUM_MATRIX_ROWS-1][NUM_MATRIX_COLS-1] = nullptr; //[10][10]
    }

    // Update the neighbors of the vertices
    void Board::updateVertexNeighbors()
    {
        for(int row = 0; row < NUM_MATRIX_ROWS; ++row)
        {
            for(int col = 0; col < NUM_MATRIX_COLS; ++col)
            {
                if(boardVertices[row][col] != nullptr)
                {
                    std::vector<Vertex*> myVertexNeighbors;
                    if(col + 1 < NUM_MATRIX_COLS && boardVertices[row][col + 1] != nullptr)
                    {
                        myVertexNeighbors.push_back(boardVertices[row][col + 1]); 
                    }
                    
                    if(col - 1 >= 0 && boardVertices[row][col - 1] != nullptr)
                    {
                        myVertexNeighbors.push_back(boardVertices[row][col - 1]); 
                    }
                    
                    if(((row + col) % 2) == 0) //even
                    {
                        if(row + 1 < NUM_MATRIX_ROWS && boardVertices[row + 1][col] != nullptr)
                        {
                            myVertexNeighbors.push_back(boardVertices[row + 1][col]); 
                        }
                    }
                    else //odd
                    {
                        if(row - 1 >= 0 && boardVertices[row - 1][col] != nullptr)
                        {
                            myVertexNeighbors.push_back(boardVertices[row - 1][col]); 
                        }
                    }

                    boardVertices[row][col]->setMyVertexNeighbors(myVertexNeighbors);
                }
            }
        }
    }

    // Initialize the edges of the board
    void Board::initializeEdges()
    {
        for(int row = 0; row < NUM_MATRIX_ROWS; ++row)
        {
            for(int col = 0; col < NUM_MATRIX_COLS; ++col)
            {
                if(boardVertices[row][col] != nullptr)
                {
                    std::vector<Edge*> mySurroundingEdges;
                    if(col + 1 < NUM_MATRIX_COLS && boardVertices[row][col + 1] != nullptr)
                    {
                        Edge* edge = addEdge(boardVertices[row][col], boardVertices[row][col + 1]);
                        if(edge != nullptr)
                        {
                            mySurroundingEdges.push_back(edge);
                        }
                    }
                    
                    if(col - 1 >= 0 && boardVertices[row][col - 1] != nullptr)
                    {
                        Edge* edge = addEdge(boardVertices[row][col], boardVertices[row][col - 1]);
                        if(edge != nullptr)
                        {
                            mySurroundingEdges.push_back(edge);
                        }
                    }
                    
                    if(((row + col) % 2) == 0) //even
                    {
                        if(row + 1 < NUM_MATRIX_ROWS && boardVertices[row + 1][col] != nullptr)
                        {
                            Edge* edge = addEdge(boardVertices[row][col], boardVertices[row + 1][col]);
                            if(edge != nullptr)
                            {
                                mySurroundingEdges.push_back(edge);
                            }
                        }
                    }
                    else //odd
                    {
                        if(row - 1 >= 0 && boardVertices[row - 1][col] != nullptr)
                        {
                            Edge* edge = addEdge(boardVertices[row][col], boardVertices[row - 1][col]);
                            if(edge != nullptr)
                            {
                                mySurroundingEdges.push_back(edge);
                            }
                        }
                    }
                    boardVertices[row][col]->setMyEdges(mySurroundingEdges);
                }
            }
        }
    }
    
    // Add edges to the board of vertices
    Edge* Board::addEdge(Vertex *vertex1, Vertex *vertex2)
    {       
        if(vertex1 == nullptr || vertex2 == nullptr)
        {
            return nullptr;
        }
        
        for(Edge* existingEdge: boardEdges)
        {
            if(existingEdge->getVertices().first == vertex1 && existingEdge->getVertices().second == vertex2)
            {
                return existingEdge;
            }
            else if(existingEdge->getVertices().first == vertex2 && existingEdge->getVertices().second == vertex1)
            {
                return existingEdge;
            }
        }
        Edge* tempEdge = new Edge(vertex1, vertex2);
        
        boardEdges.push_back(tempEdge);
        return tempEdge;
    }

    // Initialize the Tiles of the board and the type of the tiles with random selections
    void Board::initializeTiles() 
    {
        // Create a vector with the exact number of each Tile::Type
        std::vector<TileType> tileTypes = 
        {
                TileType::Tree, TileType::Tree, TileType::Tree, TileType::Tree,
                TileType::Clay, TileType::Clay, TileType::Clay,
                TileType::Wool, TileType::Wool, TileType::Wool, TileType::Wool,
                TileType::Crop, TileType::Crop, TileType::Crop, TileType::Crop,
                TileType::Iron, TileType::Iron, TileType::Iron,
                TileType::Sand
        };

        // Shuffle the tileTypes vector
        std::shuffle(tileTypes.begin(), tileTypes.end(),
                    std::default_random_engine(static_cast<unsigned>(std::time(0))));

        // Create a Tile for each type in tileTypes and add it to the tiles vector
        for (size_t tileAtIndex = 0; tileAtIndex < tileTypes.size(); ++tileAtIndex) 
        {
            boardTiles.push_back(new Tile(tileTypes[tileAtIndex]));
        }
    }

    // Assign the numbers to the tiles on the board - for dice rolls
    void Board::assignNumbers() 
    {   
        std::vector<int> numbers = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
        int atIndex = 0;

        // Shuffle the numbers vector
        std::shuffle(numbers.begin(), numbers.end(),
                    std::default_random_engine(static_cast<unsigned>(std::time(0))));

        for (Tile* tile: boardTiles) 
        {
            if (tile->getType() != TileType::Sand) 
            {
                tile->setValue(numbers[atIndex]);
                ++atIndex;
            }
        }
    }

    // Assign the vertex to the tiles
    void Board::assignVertexToTiles()
    {
        int numTile = 0;
        int row = 0;
        for(int col = 2; col <= 8; col+= 2)
        {
            boardTiles[numTile]->setVertex(boardVertices[row][col]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+2]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+2]);
            if(numTile == 2) break;
            numTile++;
        }

        row++;
        numTile++;
        for(int col = 1; col <= 9; col+= 2)
        {
            boardTiles[numTile]->setVertex(boardVertices[row][col]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+2]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+2]);
            if(numTile == 6) break;
            numTile++;
        }

        row++;
        numTile++;
        for(int col = 0; col <= 10; col+= 2)
        {
            boardTiles[numTile]->setVertex(boardVertices[row][col]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+2]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+2]);
            if(numTile == 11) break;
            numTile++;
        }

        row++;
        numTile++;
        for(int col = 1; col <= 9; col+= 2)
        {
            boardTiles[numTile]->setVertex(boardVertices[row][col]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+2]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+2]);
            if(numTile == 15) break;
            numTile++;
        }

        row++;
        numTile++;
        for(int col = 2; col <= 8; col+= 2)
        {
            boardTiles[numTile]->setVertex(boardVertices[row][col]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+1]);
            boardTiles[numTile]->setVertex(boardVertices[row][col+2]);
            boardTiles[numTile]->setVertex(boardVertices[row+1][col+2]);
            if(numTile == 18) break;
            numTile++;
        }
    }

    // Place a settlement on the board
    Vertex* Board::placeSettlement(int row, int col, Player *player, bool isCity, bool isResouceCheckRequire)
    {
        if(isOutOfBound(row,col))
        {
            std::cout<<"Coordination Out-of-bound"<<std::endl;
            return nullptr;
        }
        
        if(boardVertices[row][col] != nullptr)
        {
            if(!isCity){
                if((boardVertices[row][col]->getOwner()) == nullptr && (boardVertices[row][col]->isSettlementBuildable(isCity)))
                {
                    if((player->addBuilding(boardVertices[row][col], isCity, isResouceCheckRequire)))
                    {
                        return boardVertices[row][col];
                    }
                }
            }
            else{
                if((boardVertices[row][col]->getOwner() == player) && (boardVertices[row][col]->isSettlementBuildable(isCity)))
                {
                    if((player->addBuilding(boardVertices[row][col], isCity, false)))
                    {
                        return boardVertices[row][col];
                    }
                }
            }
        }
        return nullptr;
    }

    // Place a road on the board
    Edge *Board::placeRoad(int fromRow, int fromCol, int toRow, int toCol, Player *player, bool freeFromResource)
    {
        if(isOutOfBound(fromRow, fromCol) || isOutOfBound(toRow, toCol) || (fromRow == toRow && fromCol == toCol))
        {
            std::cout<<"Out of bound Or Invliad arguments"<<std::endl;
            return nullptr;
        }

        //Looking for the edge in the boardEdges
        auto it = std::find_if(boardEdges.begin(), boardEdges.end(), [&](Edge* edge)
        {
            return ( edge->getVertices().first->getRow() == fromRow && edge->getVertices().first->getColumn() == fromCol 
                     && edge->getVertices().second->getRow() == toRow && edge->getVertices().second->getColumn() == toCol)
                || ( edge->getVertices().first->getRow() == toRow && edge->getVertices().first->getColumn() == toCol
                     && edge->getVertices().second->getRow() == fromRow && edge->getVertices().second->getColumn() == fromCol);
        });
                
        Edge* edge = (it != boardEdges.end()) ? *it : nullptr; 

        //If the edge exist
        if(edge != nullptr)
        {
            //if the edge already has a road
            if(edge->hasRoad())
            {
                std::cout<<"Edge already has road"<<std::endl;
                return nullptr;
            }

            //If there is a settlement or city in the vertex, is connected to the road of player
            else if(boardVertices[fromRow][fromCol]->getOwner() == player || boardVertices[toRow][toCol]->getOwner() == player)
            {
                if(player->addRoad(edge, freeFromResource))
                {
                    return edge;
                }
            }

            //If there is no settelemnt or city in the vertex, is connected to other road of player from vertex 1
            std::vector<Edge*> firstVertexSurrondEdges = edge->getVertices().first->getMySurroundingEdges();
            for(Edge* surroundingEdge : firstVertexSurrondEdges)
            {
                if(surroundingEdge->hasRoad() && surroundingEdge->getRoadOwner() == player)
                {
                    if(player->addRoad(edge, freeFromResource))
                    {
                    return edge;
                    }
                }
            }

            //If there is no settelemnt or city in the vertex, is connected to other road of player from vertex 2
            std::vector<Edge*> secondVertexSurrondEdges = edge->getVertices().second->getMySurroundingEdges();
            for(Edge* surroundingEdge : secondVertexSurrondEdges)
            {
                if(surroundingEdge->hasRoad() && (surroundingEdge->getRoadOwner() == player))
                {
                    if(player->addRoad(edge, freeFromResource))
                    {
                        return edge;
                    }
                }
            }
        }
        std::cout<<"Road Must be next to City or Road"<<std::endl;
        return nullptr;
    }

    // Send the starting resources to the players
    void Board::sendStartingResources()
    {
        for(Tile* tile: boardTiles)
        {
            for(Vertex* vertex: tile->getVertices())
            {
                vertex->addResources(tile->getType());
            }
        }
    }

    // Check if the coordinates are out of bound
    bool Board::isOutOfBound(int row, int col) const
    {
        if((row < 0 || row > 5 || col < 0 || col > 11)  
            ||((row == 0 || row == 5) && (col == 0 || col == 1 || col == 9 || col == 10))
            || ((row == 1 || row == 4) && (col == 0 || col == 10)) )
            {
                return true;
            }
        return false;
    }

    // After rolling the dices the method distribute the resources
    void Board::distrbuteResources(int diceRoll)
    {
        for(Tile* tile: boardTiles)
        {
            if(tile->getValue() == diceRoll)
            {
                for(Vertex* vertex: tile->getVertices())
                {
                    vertex->addResources(tile->getType());
                }
            }
        }
    }

    // Print the board coordinates and structure
    void Board::printBoard() const 
    {
        int dynSize = -1;
        int counter = -1;
        std::vector<std::string> tileVectorType;
        std::vector<int> tileVectorValue;

        for(int row = 0; row < NUM_MATRIX_ROWS-1; ++row)
        {
            if(row == 0 || row == 4) {
                dynSize = 2;
                counter = (row == 0) ? 16 : 0;
            }
            else if(row == 1 || row == 3)
            {
                dynSize = 3;
                counter = (row == 1) ? 12 : 3;
            }
            else if(row == 2)
            {
                dynSize = 4;
                counter = 7;        
            }

            
            for(int col = 0; col <= dynSize; ++col)
            {
                tileVectorValue.push_back(boardTiles[counter]->getValue());

                switch(boardTiles[counter]->getType())
                {
                    case TileType::Tree:
                        tileVectorType.push_back("Tree");
                        break;
                    case TileType::Clay:
                        tileVectorType.push_back("Clay");
                        break;
                    case TileType::Crop:
                        tileVectorType.push_back("Crop");
                        break;
                    case TileType::Wool:
                        tileVectorType.push_back("Wool");
                        break;
                    case TileType::Iron:
                        tileVectorType.push_back("Iron");
                        break;
                    case TileType::Sand:
                        tileVectorType.push_back("Sand");
                        break;
                }
                ++counter;
            }
            if(counter == 18) break;
        }
        
        std::cout << "******************************* CATAN BOARD *******************************" << std::endl;
        int typeIndex = 0;
        int valueIndex = 0;
        
        // Row 5
        std::cout << "          ";
        for (int col = 0; col < 10; col++) {
            if (boardVertices[5][col] != nullptr && ((col % 2) != 0)) {
                std::cout << (*boardVertices[5][col]) << "___  ";
            }else if(col == 0 || col == 1 || col == 8 || col == 9){
                std::cout << "     ";
            } else {
                std::cout << "___";
            }
        }
        std::cout << std::endl;

        // Row 3
        std::cout << "                 ";
        for (int col = 2; col <= 8; col++) {
            if(col % 2 == 0){
                switch(col){
                    case 2:
                        std::cout << "  /   ";
                        break;
                    case 8:
                        std::cout << "  \\ ";
                        break;
                    default:
                        std::cout << "  \\/   ";
                        break;
                }
            }
            else{
                std::cout << "  .   ";
            }
        }
        std::cout << std::endl;

        // Row 5
        std::cout << "   ";
        for (int col = 0; col < 10; col++) {
            if (boardVertices[5][col] != nullptr && ((col % 2) == 0)) {
                std::cout << (*boardVertices[5][col])<<"   ";
            } else if(col > 1 && col < 9){
                if(tileVectorValue[valueIndex] > 9 || tileVectorValue[valueIndex] < 0)
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"   ";
                }else{
                    std::cout<<tileVectorValue[valueIndex++]<<"    ";
                }
                
            }else{
                std::cout << "       ";
            }
        }
        std::cout << std::endl;

        // Row 4
        std::cout << "           ";
        for (int col = 0; col < 10; col++) {
            if(col == 0|| col == 9){
                std::cout << "       ";
            }else if(col == 1){
                std::cout << "| ";
            }else if(col % 2 != 0){
                std::cout << "|";
            }else if(col > 1 && col < 8){
                std::cout << "    "<<tileVectorType[typeIndex++]<<"    ";
            }else{
                std::cout << "       ";
            }
        }
        std::cout << std::endl;

        // Row 4
        std::cout << "        ";
        for (int col = 0; col < 10; col++) {
            if (boardVertices[4][col] != nullptr && ((col % 2) == 0)) {
                std::cout << (*boardVertices[4][col]) << "___. ";
            }else if(col == 0 || col == 9){
                std::cout << "      ";
            } 
            else {
                std::cout << "___";
            }
        }
        std::cout << std::endl;

        // Row 4
        std::cout << "     ";
        for (int col = 0; col <= 10; col++) {
            if(col % 2 != 0){
                switch(col){
                    case 1:
                        std::cout << "  /   ";
                        break;
                    case 9:
                        std::cout << "  \\ ";
                        break;
                    default:
                        std::cout << "  \\/   ";
                        break;
                }
            }
            else if(col > 1 && col < 9)
            {
                std::cout << "  .   ";
            }
            else
            {
                std::cout << "      ";
            }
        }
        std::cout << std::endl;

        // Row 4
        std::cout << "    ";
        for (int col = 0; col <= 10; col++) {
            if (boardVertices[4][col] != nullptr && ((col % 2) != 0)) 
            {
                std::cout << (*boardVertices[4][col])<<"   ";
            } 
            else if(col > 1 && col < 9)
            {
                if(tileVectorValue[valueIndex] > 9 || tileVectorValue[valueIndex] < 0)
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"   ";
                }
                else
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"    ";
                }
            }
            else{
                std::cout << "       ";
            }
        }
        std::cout << std::endl;

        // Row 3
        std::cout << "           ";
        for (int col = 0; col < 10; col++) 
        {
            if(col == 0)
            {
                std::cout << " |    ";
            }
            else if(col % 2 == 0)
            {
                std::cout << "  |   ";
            }
            else if(col > 0 && col < 9){
                std::cout<<" "<<tileVectorType[typeIndex++]<<"  ";
            }
            else
            {
                std::cout << "       ";
            }
        }
        std::cout << std::endl;

        // Row 3
        std::cout << "        ";
        for (int col = 0; col < 10; col++) {
            if (boardVertices[3][col] != nullptr && ((col % 2) != 0) ) {
                std::cout << (*boardVertices[3][col]) << "___  ";
            } else {
                std::cout << "___";
            }
        }
        std::cout << std::endl;

        // Row 3
        std::cout << "     ";
        for (int col = 0; col <= 10; col++) {
            if(col % 2 == 0){
                switch(col){
                    case 0:
                        std::cout << "  /   ";
                        break;
                    case 10:
                        std::cout << "  \\ ";
                        break;
                    default:
                        std::cout << "  \\/   ";
                        break;
                }
            }
            else{
                std::cout << "  .   ";
            }
        }
        std::cout << std::endl;
        
        // Row 3
        std::cout << "     ";
        for (int col = 0; col <= 10; col++) 
        {
            if (boardVertices[3][col] != nullptr && ((col % 2) == 0)) 
            {
                std::cout << (*boardVertices[3][col])<<"   ";
            } 
            else
            {
                if(tileVectorValue[valueIndex] > 9 || tileVectorValue[valueIndex] < 0)
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"   ";
                }
                else
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"    ";
                }

            }
        }
        std::cout << std::endl;
        
        // Row 2
        std::cout << "      ";
        for (int col = 0; col <= 10; col++) 
        {
            if(col == 0)
            {
                std::cout << "|";
            }
            else if(col % 2 == 0)
            {
                std::cout << "|";
            }
            else
            {
                std::cout<<"    "<<tileVectorType[typeIndex++]<<"    ";
            }
        }
        std::cout << std::endl;

        // Row 2
        std::cout << "     ";
        for (int col = 0; col <= 10; col++) 
        {
            if (boardVertices[2][col] != nullptr && ((col % 2) == 0)) 
            {
                std::cout << (*boardVertices[2][col]) << " ";
            } 
            else 
            {
                std::cout << "  .    ";
            }
        }
        std::cout << std::endl;

        // Row 2
        std::cout << "     ";
        for (int col = 0; col <= 10; col++) {
            if(col % 2 == 0){
                switch(col){
                    case 0:
                        std::cout << "  \\___";
                        break;
                    case 10:
                        std::cout << "___/  ";
                        break;
                    default:
                        std::cout << "___/\\___";
                        break;
                }
            }
            else{
                std::cout << "  .  ";
            }
        }
        std::cout << std::endl;

        // Row 1
        std::cout << "           ";
        for (int col = 1; col <= 10; col++) {
            if (boardVertices[2][col] != nullptr && ((col % 2) != 0)) {
                std::cout << (*boardVertices[2][col]) << "   ";
            } else if(col > 1&& col < 10){
                std::cout << ".    ";
            }else {
                std::cout << "     ";
            }
        }
        std::cout << std::endl;

        // Row 1
        std::cout << "            ";
        for (int col = 0; col < 10; col++) {
            if(col == 0)
            {
                std::cout << "| ";
            }
            else if(col % 2 == 0)
            {
                std::cout << "|";
            }
            else if(col > 0 && col < 9)
            {
                std::cout << "     ";
                if(tileVectorValue[valueIndex] > 9 || tileVectorValue[valueIndex] < 0)
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"     ";
                }
                else
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"      ";
                }
            }
            else
            {
                std::cout << "       ";
            }
        }
        std::cout << std::endl;

        // Row 1 
        std::cout << "           ";
        for (int col = 1; col <= 10; col++) {
            if (boardVertices[1][col] != nullptr && ((col % 2) != 0)) {
                std::cout << (*boardVertices[1][col]);
            } else if(col > 1 && col < 9) {
                std::cout<<"  "<<tileVectorType[typeIndex++]<<"  ";
            }else{
                std::cout << "     ";
            }
        }
        std::cout << std::endl;
        
        //row 1
        std::cout << "          ";
        for (int col = 1; col <= 9; col++) {
            if(col % 2 != 0){
                switch(col){
                    case 1:
                        std::cout << "   \\___";
                        break;
                    case 9:
                        std::cout << "___/ ";
                        break;
                    default:
                        std::cout << "___/\\___";
                        break;
                }
            }
            else{
                std::cout << "  .  ";
            }
        }
        std::cout << std::endl;

        // Row 1
        std::cout << "       ";
        for (int col = 0; col <= 10; col++) {
            if (boardVertices[1][col] != nullptr && ((col % 2) == 0)) {
                std::cout << (*boardVertices[1][col]) << "   ";
            } else if(col > 1 && col < 9){
                std::cout << " .   ";
            }else{
                std::cout << "     ";
            }
        }
        std::cout << std::endl;


        // Row 4
        std::cout << "                   ";
        for (int col = 0; col < 9; col++) 
        {

            if(col == 1)
            {
                std::cout << "|";
            }
            else if(col % 2 != 0)
            {
                std::cout << "|";
            }
            else if(col > 1 && col < 8)
            {
                std::cout << "     ";
                if(tileVectorValue[valueIndex] > 9 || tileVectorValue[valueIndex] < 0)
                {
                    std::cout<<tileVectorValue[valueIndex++]<<"     ";
                }
                else
                {
                    std::cout<<" "<<tileVectorValue[valueIndex++]<<"     ";
                }
            }

        }
        std::cout << std::endl;

        // Row 0
        std::cout << "       ";
        for (int col = 0; col <= 10; col++) {
            if (boardVertices[0][col] != nullptr && ((col % 2) == 0)) 
            {
                std::cout << (*boardVertices[0][col]);
            } 
            else if (col > 2 && col < 8)
            {
                std::cout<<"  "<<tileVectorType[typeIndex++]<< "  ";
                //std::cout << " .   ";
            }
            else
            {
                std::cout << "     ";
            }
        }
        std::cout << std::endl;

        std::cout << "            ";
        for (int col = 0; col < 8; col++) 
        {
            if(col % 2 != 0)
            {
                switch(col)
                {
                    case 1:
                        std::cout << "   \\___";
                        break;
                    case 7:
                        std::cout << "___/ ";
                        break;
                    default:
                        std::cout << "___/\\___";
                        break;
                }
            }
            else if(col > 1)
            {
                std::cout << "  .  ";
            }
            else 
            {
                std::cout << "     ";
            }
        }
        std::cout << std::endl;

        // Row 0
        std::cout << "            ";
        for (int col = 0; col <= 8 ; col++) 
        {
            if (boardVertices[0][col] != nullptr && ((col % 2) != 0)) 
            {
                std::cout << (*boardVertices[0][col]) << "    ";
            } 
            else 
            {
                std::cout << "    ";
            }
        }
        std::cout << std::endl;

        std::cout << "***************************************************************************" << std::endl;
    }
}
