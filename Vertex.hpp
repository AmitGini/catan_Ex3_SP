#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Edge.hpp"
#include "Player.hpp"
#include "Tile.hpp"

namespace catan_game {
    class Edge;
    class Player;
    enum class TileType;

    class Vertex {
    private:
        Player* owner;
        bool settled;
        bool city;
        int row;
        int col;

        std::vector<Edge*> mySurroundingEdges;
        std::vector<Vertex*> myVertexNeighbors;

    public:
        // Constructor to initialize the vertex
        Vertex(int rowCoord, int columnCoord);

        // Get the x coordinate of the vertex
        int getRow() const;

        // Get the y coordinate of the vertex
        int getColumn() const;

        // Get the owner of the vertex
        Player* getOwner() const;

        // Get the neighbors of the vertex
        const std::vector<Vertex*>& getMyVertexNeighbors() const;

        // Get the surround edges of the vertex
        const std::vector<Edge*>& getMySurroundingEdges() const;

        // Check if the vertex is buildable
        bool isSettlementBuildable(bool isCity) const;

        // Check if the vertex is a settlement
        bool isSettled() const;

        // Check if the vertex is a city
        bool isCity() const;

        // Set the owner of the vertex
        void setOwner(Player* player);

        // Set the neighbors of the vertex
        void setMyVertexNeighbors(std::vector<Vertex*>& vertex);
        
        // Set the edges of the vertex
        void setMyEdges(std::vector<Edge*>& mySurroundingEdges);

        // Set the settlement status of the vertex
        void setSettlement();

        // Set the city status of the vertex
        void setCity();

        // Add the resources to the player
        void addResources(const TileType& tileType);

        // operator == to check if two vertices are equal
        bool operator==(const Vertex& vertex) const;

        // default printing of vertex using cout
        friend std::ostream& operator<<(std::ostream &stream, const Vertex& vertex);

    };
}
#endif // VERTEX_HPP
