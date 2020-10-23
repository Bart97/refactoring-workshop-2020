#pragma once
struct Coordinate
{
    int x;
    int y;

    Coordinate(int _x, int _y) : x(_x), y(_y) { }
};

class Map
{
public:
    Map(int x, int y);

    bool isPositionOutsideMap(int x, int y) const;

private:
    Coordinate _dimension;
};