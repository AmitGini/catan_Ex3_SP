#include <iostream>
#include <string>

#include "Tile.hpp"

using std::string;
static int tilesIndex = 0;

namespace catan_game {

    Tile::Tile(TileType typeName) : type(typeName), index(tilesIndex++), value(-1) {}

    int Tile::getIndex() const { return this->index; }

    TileType Tile::getType() const
    {
        return this->type;
    }

    const std::vector<Vertex *>& Tile::getVertices() const
    {
        return this->myVertices;
    }

    int Tile::getValue() const { return this->value; }

    void Tile::setValue(int numeValue)
    {
        if(value == -1 && (this->getType() != TileType::Sand)){
            value = numeValue;
        }
    }

    void Tile::setVertex(Vertex *vertex)
    {
        myVertices.push_back(vertex);
    }

    void Tile::sendResources()
    {
        for(Vertex* vertex: myVertices)
        {
            vertex->addResources(this->type);
        }
    }

    std::string tileTypeToString(const TileType& tileType) {
        switch (tileType) {
            case TileType::Tree:
                return "Tree";
            case TileType::Clay:
                return "Clay";
            case TileType::Crop:
                return "Crop";
            case TileType::Wool:
                return "Wool";
            case TileType::Iron:
                return "Iron";
            case TileType::Sand:
                return "Sand";
            default:
                return "Unknown";
        }
    }

    TileType stringToTileType(const std::string& tileType) {
        if(tileType.compare("Tree")== 0)
        {
            return TileType::Tree;
        }
        else if(tileType.compare("Clay") == 0)
        {
            return TileType::Clay;
        }
        else if(tileType.compare("Crop")== 0)
        {
            return TileType::Crop;
        }
        else if(tileType.compare("Wool")== 0)
        {
            return TileType::Wool;
        }
        else if(tileType.compare("Iron")== 0)
        {
            return TileType::Iron;
        }
        else 
        {
            return TileType::Sand;
        }
    }

    std::ostream& operator<<(std::ostream &stream, Tile const &tile){
        stream << tile.getType() << " " << tile.getValue();
        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const TileType &tileType)
    {
        stream << tileTypeToString(tileType);
        return stream;
    }

}