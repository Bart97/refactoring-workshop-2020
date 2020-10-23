#include "Map.hpp"

Map::Map(int x, int y) : _dimension(x, y)
{

}

bool Map::isPositionOutsideMap(int x, int y) const
{
    return x < 0 or y < 0 or x >= _dimension.x or y >= _dimension.y;
}