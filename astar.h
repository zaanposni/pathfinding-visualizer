#ifndef Astar
#define Astar

#include <algorithm>
#include "vector"
#include <unistd.h>

#include "2dArray.h"
#include "customWindow.h"

class AstarPathfinding
{
    private:
        int size;
        Grid::CustomGrid grid;
    
    public:

        AstarPathfinding(int _size, Grid::CustomGrid _grid)
        {
            size = _size;
            grid = _grid;
        }

        int GetDistance(Node a, Node b)
        {
            int distanceX = std::abs(a.x - b.x);
            int distanceY = std::abs(a.y - b.y);

            if (distanceX > distanceY)
            {
                return 14 * distanceY + 10 * (distanceX - distanceY);
            }
            return 14 * distanceX + 10 * (distanceY - distanceX);
        }

        void FindPath(Node startPos, Node targetPos, CustomWindow window, int sleepMilliSecondsPerStep=0)
        {
            std::vector< Node > openSet;
            std::vector< Node > closedSet;
            
            openSet.push_back(startPos);
            while (openSet.size() > 0)
            {
                if (window.checkForKeyboardInterrupt()) { return; }  // window still is responsive and space can cancel path finding
                Node currentNode = openSet[0];
                for (int i=1; i < openSet.size(); i++)
                {
                    if (openSet[i].GetFCost() < currentNode.GetFCost() || openSet[i].GetFCost() == currentNode.GetFCost() && openSet[i].hCost < currentNode.hCost) 
                    {
                        currentNode = openSet[i];
                    }
                }

                openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());  // remove by value
                closedSet.push_back(currentNode);
                window.markPixel(currentNode.x, currentNode.y, sf::Color(17, 128, 36));

                if (currentNode == targetPos)  // path found
                {
                    std::vector<Node> path;
                    currentNode = grid.GetNode(targetPos.x, targetPos.y);
                    while(currentNode != startPos)
                    {
                        path.push_back(currentNode);
                        currentNode = grid.GetNode(currentNode.parentX, currentNode.parentY);
                    }

                    std::reverse(path.begin(), path.end());
                    for (Node node : path)
                    {
                        window.markPixel(node.x, node.y, sf::Color(210, 138, 0));
                        std::cout << "Move to x: " << node.x << " y: " << node.y << std::endl;
                    }
                    return;
                }

                for (Node neighbourNode : grid.GetNeighbours(currentNode))
                {
                    if (! neighbourNode.useable || (std::find(closedSet.begin(), closedSet.end(), neighbourNode)!=closedSet.end()))  // contains
                    {
                        continue;
                    }

                    int newMovCost = currentNode.gCost + GetDistance(currentNode, neighbourNode);
                    if (newMovCost < neighbourNode.gCost || (std::find(openSet.begin(), openSet.end(), neighbourNode)==openSet.end()))  // not contain
                    {
                        neighbourNode.gCost = newMovCost;
                        neighbourNode.hCost = GetDistance(neighbourNode, targetPos);
                        grid.SetParent(neighbourNode, currentNode);

                        if (std::find(openSet.begin(), openSet.end(), neighbourNode)==openSet.end())  // not contain
                        {
                            openSet.push_back(neighbourNode);
                            window.markPixel(neighbourNode.x, neighbourNode.y, sf::Color(11, 181, 36));
                        }
                    }
                }
                // sleep
                if (sleepMilliSecondsPerStep > 0) 
                {
                    usleep(sleepMilliSecondsPerStep*1000);
                }
            }
        }
};

#endif
