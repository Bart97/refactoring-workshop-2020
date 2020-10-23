#include "Snake.hpp"

#include <algorithm>
#include <memory>

#include "EventT.hpp"
#include "IPort.hpp"

#include "SnakeController.hpp"

namespace Snake
{
SnakeBody::SnakeBody(Controller &ctrl) : m_controller(ctrl)
{

}

void SnakeBody::addSegment(const Segment& seg)
{
    m_segments.push_back(seg);
}

bool SnakeBody::isSegmentAtPosition(int x, int y) const
{
    return m_segments.end() !=  std::find_if(m_segments.cbegin(), m_segments.cend(),
                                             [x, y](auto const& segment){ return segment.x == x and segment.y == y; });
}


namespace
{
bool isHorizontal(Direction direction)
{
    return Direction_LEFT == direction or Direction_RIGHT == direction;
}

bool isVertical(Direction direction)
{
    return Direction_UP == direction or Direction_DOWN == direction;
}

bool isPositive(Direction direction)
{
    return (isVertical(direction) and Direction_DOWN == direction)
           or (isHorizontal(direction) and Direction_RIGHT == direction);
}

bool perpendicular(Direction dir1, Direction dir2)
{
    return isHorizontal(dir1) == isVertical(dir2);
}
} // namespace

SnakeBody::Segment SnakeBody::calculateNewHead() const
{
    Segment const& currentHead = m_segments.front();

    Segment newHead;
    newHead.x = currentHead.x + (isHorizontal(m_controller.m_currentDirection) ? isPositive(m_controller.m_currentDirection) ? 1 : -1 : 0);
    newHead.y = currentHead.y + (isVertical(m_controller.m_currentDirection) ? isPositive(m_controller.m_currentDirection) ? 1 : -1 : 0);

    return newHead;
}

void SnakeBody::removeTailSegment()
{
    auto tail = m_segments.back();

    DisplayInd l_evt;
    l_evt.x = tail.x;
    l_evt.y = tail.y;
    l_evt.value = Cell_FREE;
    m_controller.m_displayPort.send(std::make_unique<EventT<DisplayInd>>(l_evt));

    m_segments.pop_back();
}

void SnakeBody::addHeadSegment(Segment const& newHead)
{
    m_segments.push_front(newHead);

    DisplayInd placeNewHead;
    placeNewHead.x = newHead.x;
    placeNewHead.y = newHead.y;
    placeNewHead.value = Cell_SNAKE;

    m_controller.m_displayPort.send(std::make_unique<EventT<DisplayInd>>(placeNewHead));
}

void SnakeBody::removeTailSegmentIfNotScored(Segment const& newHead)
{
    if (std::make_pair(newHead.x, newHead.y) == m_controller.m_foodPosition) {
        m_controller.m_scorePort.send(std::make_unique<EventT<ScoreInd>>());
        m_controller.m_foodPort.send(std::make_unique<EventT<FoodReq>>());
    } else {
        removeTailSegment();
    }
}

void SnakeBody::updateSegmentsIfSuccessfullMove(Segment const& newHead)
{
    if (isSegmentAtPosition(newHead.x, newHead.y) or m_controller.map->isPositionOutsideMap(newHead.x, newHead.y)) {
        m_controller.m_scorePort.send(std::make_unique<EventT<LooseInd>>());
    } else {
        addHeadSegment(newHead);
        removeTailSegmentIfNotScored(newHead);
    }
}
}