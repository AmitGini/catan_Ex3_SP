
#ifndef TILE_HPP
#define TILE_HPP
#include <vector>
#include <string>
#include <iostream>
#include "Vertex.hpp"


namespace catan_game {
    class Vertex;
    enum class TileType;

        class Tile {
        private:
            TileType type;
            int index;
            int value;
            std::vector<Vertex*> myVertices;

        public:
            Tile(TileType type);
            int getIndex() const;
            int getValue() const;
            TileType getType() const;
            const std::vector<Vertex*>& getVertices() const;
            void setValue(int value);
            void setVertex(Vertex* vertex);
            void sendResources();
            friend std::ostream& operator<<(std::ostream &stream, Tile const &tile);
    };


    enum class TileType {
        Tree,
        Clay, 
        Crop, 
        Wool, 
        Iron,
        Sand
    };

    std::string tileTypeToString(const TileType& tileType);
    TileType stringToTileType(const std::string& tileType);
    std::ostream& operator<<(std::ostream& stream, const TileType& tileType);
}
#endif