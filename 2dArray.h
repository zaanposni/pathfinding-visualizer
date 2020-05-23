#ifndef Grid
#define Grid

#include <iostream>
#include "node.h"


class CustomGrid
{   
    private:
        int size;
        Node *values;
    
    public:
        Node startNode;
        Node targetNode;
        std::vector<Node> border;
        CustomGrid() { }
        CustomGrid(int _size)
        {
            size = _size;
            values = new Node[size*size];
            for (int x=0; x<_size; x++)
            {
                for (int y=0; y<_size; y++)
                {
                    Node generate = Node(x, y, true);
                    bool isBorder = (std::find(border.begin(), border.end(), generate) != border.end());
                    values[x * size + y] = Node(x, y, !isBorder);  // default
                    
                }
            }
        }
        
        std::vector<Node> GetNeighbours(Node node)
        {
            std::vector<Node> neighbours;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    if (x == 0 && y == 0)
                    {
                        continue;
                    }

                    int checkX = node.x + x;
                    int checkY = node.y + y;

                    if (checkX >= 0 && checkX < size && checkY >= 0 && checkY < size)
                    {
                        neighbours.push_back(GetNode(checkX, checkY));
                    }
                }
            }
            return neighbours;
        }

        void SetValue(Node node) 
        {
            values[node.x * size + node.y] = node;
        }

        void SetUnusable(int posX, int posY)
        {
            values[posX * size + posY].useable = false;
        }

        Node GetNode(int posX, int posY)
        {
            return values[posX * size + posY];
        }

        void SetParent(Node child, Node parent)
        {
            values[child.x * size + child.y].SetParent(parent);
        }

        void SetStartNode(Node _startNode)
        {
            startNode = _startNode;
        }

        void SetTargetNode(Node _targetNode)
        {
            targetNode = _targetNode;
        }

        bool isSpecial(int posX, int posY)  // is start/end or border
        {
            Node node = GetNode(posX, posY);
            return node == startNode || node == targetNode || (std::find(border.begin(), border.end(), node)!=border.end());
        }

        void addBorder(Node borderNode)
        {
            if (std::find(border.begin(), border.end(), borderNode)==border.end())
            {
                border.push_back(borderNode);
                SetUnusable(borderNode.x, borderNode.y);
            }
        }
};

#endif