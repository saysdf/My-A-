#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>

typedef std::tuple<int,int> Position;
//HASH For unordered map of tuple
struct key_hash : public std::unary_function<Position, std::size_t>
{
    std::size_t operator()(const Position& k) const
    {
        return std::get<0>(k) ^ std::get<1>(k);
    }
};
struct key_equal : public std::binary_function<Position, Position, bool>
{
    bool operator()(const Position& v0, const Position& v1) const
    {
        return (
                    std::get<0>(v0) == std::get<0>(v1) &&
                    std::get<1>(v0) == std::get<1>(v1)
                    );
    }
};


class Grid {
public:
    Grid (size_t w,size_t h,const std::vector<int> *map){
        if (map == nullptr)
            throw std::invalid_argument( "received null map" );
        if (w*h != map->size())
            throw std::invalid_argument( "map.size != w*h" );
        width=w;
        height=h;
        neighboursPos={Position{1,0},
                       Position{0,1},
                       Position{-1,0},
                       Position{0,-1}};
        this->map=map;
    }
    inline Position toPolar(const size_t i ) const {
        if (i>=map->size())
            throw std::out_of_range( "i>=map.size()" );
        auto r = std::make_tuple(i%width,i/width);
        return r;
    }
    inline size_t fromPolar(const Position& pos) const {
        return std::get<0>(pos)+std::get<1>(pos)*width;
    }
    inline bool inBounds(const Position& pos) const {
        int x, y;
        std::tie (x, y) = pos;
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    //Traversable locations of the grid are indicated by 1, and impassable locations are indicated by 0
    inline bool possible(Position pos) const {
        return (bool)map->at(fromPolar(pos));
    }
    std::vector<Position> neighbors(Position pos, std::vector<Position> &res) const {
        int x, y, dx, dy;
        std::tie (x, y) = pos;
        res.clear();
        for (auto dir : neighboursPos) {
            std::tie (dx, dy) = dir;
            Position next(x + dx, y + dy);
            if (inBounds(next) && possible(next)) {
                res.push_back(next);
            }
        }
        return res;
    }
    bool aStar(std::pair<int, int> Start,
               std::pair<int, int> Target,
               std::vector<int>& OutPath){
        //priority queue with cost and position
        typedef std::pair<double, Position> PriorityQueueElement;
        std::priority_queue<PriorityQueueElement,
                std::vector<PriorityQueueElement>,
                std::greater<PriorityQueueElement>> frontier;

        std::unordered_map<Position, double,key_hash,key_equal> cost;
        std::unordered_map<int, int> way;

        Position start = std::make_tuple(Start.first,Start.second);
        Position target = std::make_tuple(Target.first,Target.second);
        int polarStart = fromPolar(start);
        int polarTagret = fromPolar(target);

        frontier.emplace(0,start);
       // way[polarStart]=polarStart;
        cost[start] = 0;
        std::vector<Position> neighborsPos;
        neighborsPos.reserve(4);
        while (!frontier.empty()) {
            auto current =  frontier.top().second;
            frontier.pop();
            if (current == target) {
                //collect result from map to outvector
                int index = polarTagret;
                while (index!=polarStart &&
                       OutPath.size()<way.size()) {
                    OutPath.push_back(index);
                    index = way[index];
                }
                 std::reverse(OutPath.begin(), OutPath.end());
                return true;
            }
            for (auto& next : neighbors(current,neighborsPos)) {
                //+1 - cost of move to new position
                double new_cost = cost[current] + 1;
                if (!cost.count(next) || new_cost < cost[next]) {
                    cost[next] = new_cost;
                    double priority = new_cost + heuristic(next, target);
                    frontier.emplace(priority,next);
                    way[fromPolar(next)]=fromPolar(current);
                }
            }
        }
        return false;
    }
    size_t getWidth() const;
    void setWidth(const size_t &value);

    size_t getHeight() const;
    void setHeight(const size_t &value);

private:
    inline double heuristic(Position a, Position b) {
        int x1, y1, x2, y2;
        std::tie (x1, y1) = a;
        std::tie (x2, y2) = b;
        return abs(x1 - x2) + abs(y1 - y2);
    }

    size_t width, height;
    const std::vector<int> *map;
    std::array<Position, 4> neighboursPos;
};

inline size_t Grid::getWidth() const
{
    return width;
}

inline void Grid::setWidth(const size_t &value)
{
    width = value;
}

inline size_t Grid::getHeight() const
{
    return height;
}

inline void Grid::setHeight(const size_t &value)
{
    height = value;
}

#endif // GRID_HPP
