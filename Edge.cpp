#include "Edge.hpp"
#include "Vertex.hpp"

namespace catan_game{

    Edge::Edge(Vertex *pt_ver1, Vertex *pt_ver2)
    {
        edgeVertices.first = pt_ver1;
        edgeVertices.second = pt_ver2;
        roadOwner = nullptr;
    }

    const std::pair<Vertex *, Vertex *>& Edge::getVertices() const
    {
        return edgeVertices;
    }

    const std::vector<Edge *>& Edge::getNeighbours() const
    {
        return edgeNeighbours;
    }

    Player* Edge::getRoadOwner() const
    {
        return this->roadOwner;
    }

    bool Edge::hasRoad()
    {
        return (this->roadOwner != nullptr);
    }

    void Edge::setMyNeighbors(std::vector<Edge *> neighbors)
    {
        this->edgeNeighbours = neighbors;
    }

    void Edge::setRoad(Player* player)
    {
        this->roadOwner = player;
    }

    bool Edge::operator==(const Edge &other) const
    {
        if (this->edgeVertices.first == nullptr ||
            this->edgeVertices.second == nullptr ||
            other.getVertices().first == nullptr ||
            other.getVertices().second == nullptr) 
        {
            return false;
        }
        // Check if the vertices are the same all the optiob that represend the same edge.
        bool isEqual =  (   (*this->edgeVertices.first == *other.getVertices().first) 
                            &&
                            (*this->edgeVertices.second == *other.getVertices().second)) 
                        || ((*this->edgeVertices.first == *other.getVertices().second) 
                            && 
                            (*this->edgeVertices.second == *other.getVertices().first)) 
                        ?
                        true : false;
        return isEqual;
    }

    std::ostream& operator<<(std::ostream &stream, const catan_game::Edge& edge){
        if(edge.getVertices().first == nullptr || edge.getVertices().second == nullptr){
            return stream;
        }
        stream << *(edge.getVertices().first) << "->" << *(edge.getVertices().second);
        return stream;
    }
}