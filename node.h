#ifndef Grid
#define Grid
#endif

#include <iostream>

class Node
{
    public:
        int parentX;
        int parentY;
        int gCost;
        int hCost;
        int x;
        int y;
        bool useable;
        Node() { }
        Node(int _x, int _y, bool _useable) 
        {
            gCost = 0;
            hCost = 0;
            x = _x;
            y =_y;
            useable = _useable;
        }
        int GetFCost()
        {
            return gCost + hCost;
        }
        void SetParent(Node parent)
        {
            parentX = parent.x;
            parentY = parent.y;
        }
    
    bool operator==(const Node& other)
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Node& other)
    {
        return x != other.x || y != other.y;
    }
};


