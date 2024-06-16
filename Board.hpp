
#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Tile.hpp"
#include "Card.hpp"

namespace catan_game {
    class Board {
    private:
        static Board* boardInstance;
        std::vector<std::vector<Vertex*>> boardVertices;
        std::vector<Edge*> boardEdges;
        std::vector<Tile*> boardTiles;
        
        Board();
        void initializeVertices();
        void deleteAndUpdateInvalidVertex();
        void updateVertexNeighbors();
        void initializeEdges();
        Edge* addEdge(Vertex* vertex1, Vertex* vertex2);
        void initializeTiles();
        void assignNumbers();
        void assignVertexToTiles();
        
    public:
        ~Board();
        static Board* getBoardInstance();
        const std::vector<Tile*>& getTiles() const;
        Vertex* placeSettlement(int row, int col, Player *player, bool isCity, bool freeFromResource);
        Edge* placeRoad(int fromRow, int fromCol, int toRow, int toCol, Player *player, bool freeFromResource);
        void sendStartingResources();
        void printBoard() const;
        bool isOutOfBound(int row, int col) const;
        void distrbuteResources(int diceRoll);
    };
}

#endif