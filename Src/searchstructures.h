#ifndef ASEARCH_SEARCHSTRUCTURES_H
#define ASEARCH_SEARCHSTRUCTURES_H

#include "basicstructures.h"
#include "environmentoptions.h"
#include "gl_const.h"
#include <algorithm>
#include<unordered_map>
#include<set>


bool operator==(const AgentAction &a, const AgentAction &b);

struct ActionHasher
{
    std::size_t operator()(const AgentAction &e) const;
};


// State
struct State
{
    Point pos;
    SafeInterval interval;
};

bool operator==(const State &a, const State &b);

struct StateHasher
{
    std::size_t operator()(const State &state) const;
};


// SafeInterval
bool operator==(const SafeInterval &a, const SafeInterval &b);

bool operator<(const SafeInterval &a, const SafeInterval &b);

struct SafeIntervalHasher
{
    std::size_t operator()(const SafeInterval &interval) const;
};


// Point
struct PointHasher
{
    std::size_t operator()(const Point &p) const;
};

struct Node
{
    State state;
    double F;
    int g;
    double h;
    Node *parent; //backpointer to the predecessor node (e.g. the node which g-value was used to set the g-velue of the current node)
};


class SafeIntervalsContainer : public std::set<SafeInterval>
{
public:
    using std::set<SafeInterval>::set;

    SafeIntervalsContainer();
    void eraseIntervals(int startTime, int endTime);
};


struct Comp
{
    Comp(bool breakingties);

    bool breakingties;

    bool operator()(const Node *a, const Node *b) const;
};


class OpenContainer
{
public:
    OpenContainer(bool breakingties);


    void updateNode(const Node &node);

    Node extractMinNode();

    size_t size() const;

private:
    std::set<Node *, Comp> sorted_nodes;
    std::unordered_map<State, Node, StateHasher> state_to_node;
};


#endif //ASEARCH_SEARCHSTRUCTURES_H
