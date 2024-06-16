#include <math.h>
#include <vector>
#include "Vertex.hpp"


namespace catan_game {
    // Constructor to initialize the vertex
    // Constructor to initialize the vertex
    Vertex::Vertex(int rowCoord, int columnCoord) : 
                    owner(nullptr),
                    settled(false),
                    city(false),
                    row(rowCoord),
                    col(columnCoord),
                    mySurroundingEdges(),
                    myVertexNeighbors() {}
                    
    // Get the x coordinate of the vertex
    int Vertex::getRow() const{
        return this->row;
    }

    // Get the y coordinate of the vertex
    int Vertex::getColumn() const{
        return this->col;
    }

    // Get the owner of the vertex
    Player* Vertex::getOwner() const
    {
        return this->owner;
    }

    // Get the surrounding edges of the vertex
    const std::vector<Edge*>& Vertex::getMySurroundingEdges() const
    {
        return this->mySurroundingEdges;
    }

    // Get the neighbors of the vertex
    const std::vector<Vertex*>& Vertex::getMyVertexNeighbors() const
    {
        return this->myVertexNeighbors;
    }
    
    // Check if the vertex is settled
    bool Vertex::isSettled() const
    {
        return this->settled;
    }
    
    // Check if the vertex is a city
    bool Vertex::isCity() const
    {
        return this->city;
    }

    // Check if the vertex is buildable
    bool Vertex::isSettlementBuildable(bool isCity) const
    {
        if(this->isSettled() && !isCity)
        {
            return false;
        }
        
        for(Vertex* neighbor: this->myVertexNeighbors)
        {
            if(neighbor->isSettled())
            {
                return false;
            }
        }

        return true;
    }
    
    // Set the owner of the vertex
    void Vertex::setOwner(Player *player)
    {
        if(player != nullptr )
            this->owner = player;
        else
            throw std::invalid_argument("Player cannot be null");
    }

    // Set the settlement status of the vertex
    void Vertex::setSettlement()
    {
        this->settled = true;
    }

    void Vertex::setCity()
    {
        this->city = true;
    }

    // add resources to the player
    void Vertex::addResources(const TileType& tileType)
    {
        if(this->owner != nullptr && tileType != TileType::Sand)
        {
            if(this->isCity())
            {
                this->owner->addResources(tileType, 2);
            }
            else if(this->isSettled())
            {
                this->owner->addResources(tileType, 1);
            }
        }
    }

    void Vertex::setMyEdges(std::vector<Edge*>& mySurrounding)
    {
        this->mySurroundingEdges = mySurrounding;
    }

    void Vertex::setMyVertexNeighbors(std::vector<Vertex *> &neighbors)
    {
        this->myVertexNeighbors = neighbors;
    }

    bool Vertex::operator==(const Vertex &other) const
    {
        bool isEqual = this->row == other.getRow() && this->col == other.getColumn();
        return isEqual;
    }

    std::ostream& operator<<(std::ostream &stream, const Vertex& vertex)
    {
        if(vertex.isSettled() && vertex.isCity())
        {
            stream <<"|" << vertex.getRow() << "," << vertex.getColumn() << "|";
        }
        else if(vertex.isSettled())
        {
            stream <<"[" << vertex.getRow() << "," << vertex.getColumn() << "]";
        }
        else{
            stream <<"(" << vertex.getRow() << "," << vertex.getColumn() << ")";
        }
        return stream;
    }

}
