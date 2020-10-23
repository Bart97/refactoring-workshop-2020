#pragma once

#include <list>

#include "SnakeInterface.hpp"
#include "IEventHandler.hpp"

class IPort;


namespace Snake
{
class Controller;

class SnakeBody
{
public:

    SnakeBody(Controller &ctrl);
    struct Segment
    {
        int x;
        int y;
    };

    void addSegment(const Segment& seg);
    bool isSegmentAtPosition(int x, int y) const;
    Segment calculateNewHead() const;
    void updateSegmentsIfSuccessfullMove(Segment const& newHead);
    void addHeadSegment(Segment const& newHead);
    void removeTailSegmentIfNotScored(Segment const& newHead);
    void removeTailSegment();

private:
    std::list<Segment> m_segments;

    Controller& m_controller;
};

} // namespace Snake