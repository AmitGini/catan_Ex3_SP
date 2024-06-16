#ifndef EDGE_HPP
#define EDGE_HPP
#include <array>
#include <vector>
#include "Vertex.hpp"
#include "Player.hpp"

namespace catan_game{
    class Vertex;
    class Player;
    class Edge {
    
    private:
    std::pair<Vertex*,Vertex*> edgeVertices;
    std::vector<Edge*> edgeNeighbours;
    Player* roadOwner;
    
    public:
        // Constructor
        Edge(Vertex* pt_ver1, Vertex* pt_ver2);
        // edge vertex getter
        const std::pair<Vertex*,Vertex*>& getVertices() const;
        const std::vector<Edge*>& getNeighbours() const;
        Player* getRoadOwner() const;
        bool hasRoad();
        void setMyNeighbors(std::vector<Edge *> neighbors);
        void setRoad(Player* player);
        bool operator==(const Edge& edge) const;
        friend std::ostream& operator<<(std::ostream &stream, const catan_game::Edge& edg);

    };

}
#endif