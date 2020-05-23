#ifndef Pathfinding
#define Pathfinding

#include <SFML/Graphics.hpp>
#include "2dArray.h"

class CustomWindow
{
    private:
        void init()
        {
            sf::RectangleShape background(sf::Vector2f(size_of_pixel*size+offset, size_of_pixel*size+offset));
            background.setFillColor(sf::Color(59, 56, 56, 250));
            window->draw(background);
            shapes = new sf::RectangleShape*[size];
            for (int x=0; x<size; x++)
            {
                shapes[x] = new sf::RectangleShape[size];
                for (int y=0; y<size; y++)
                {
                    sf::RectangleShape pixel(sf::Vector2f((float)((1.0-2*offset)*size_of_pixel), (float)((1.0-2*offset)*size_of_pixel)));
                    pixel.setPosition(sf::Vector2f((float)(size_of_pixel * x + offset * size_of_pixel), (float)(size_of_pixel * y + offset * size_of_pixel)));
                    pixel.setFillColor(sf::Color(219, 211, 211));
                    shapes[x][y] = pixel;
                    window->draw(shapes[x][y]);
                }
            }
            window->display();
        }

    public:
        sf::RenderWindow* window;
        sf::RectangleShape **shapes;
        int size;
        int size_of_pixel;
        float offset;
        Grid::CustomGrid startGrid;

        CustomWindow() { }
        CustomWindow(int _size, int _size_of_pixel, float _offset, Grid::CustomGrid _startGrid) 
        {
            size = _size;
            size_of_pixel = _size_of_pixel;
            offset = _offset;
            startGrid = _startGrid;
            window = new sf::RenderWindow(sf::VideoMode(size_of_pixel*size+offset, size_of_pixel*size+offset), "Pathfinding", sf::Style::Close | sf::Style::Titlebar);
            init();
        }

        void render_shapes()
        {
            for (int x=0; x<size; x++)
            {
                for (int y=0; y<size; y++)
                {
                    window->draw(shapes[x][y]);
                }
            }
            window->display();
        }

        void markPixel(int posX, int posY, sf::Color color, bool ignoreCheck=false)
        {
            if (ignoreCheck)
            {
                shapes[posX][posY].setFillColor(color);
                render_shapes();
            } else {
                if (!startGrid.isSpecial(posX, posY))
                { 
                    shapes[posX][posY].setFillColor(color);
                    render_shapes();
                } 
            }
            
        }

        Node getMouseClick(bool waitForRelease=false)
        {
            while (window->isOpen())
            {
                sf::Event evnt;
                while (window->pollEvent(evnt))
                {
                    switch(evnt.type)
                    {
                        case sf::Event::Closed:
                            window->close();
                            break;
                    }
                }
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (waitForRelease)
                    {
                        while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { }
                    }
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
                    int nodeX = mousePosition.x / size_of_pixel;
                    int nodeY = mousePosition.y / size_of_pixel;
                    return startGrid.GetNode(nodeX, nodeY);
                }

                render_shapes();
                window->display();
            }
            return Node();
        }

        bool checkForKeyboardInterrupt()
        {
            while (window->isOpen())
            {
                sf::Event evnt;
                while (window->pollEvent(evnt))
                {
                    switch(evnt.type)
                    {
                        case sf::Event::Closed:
                            window->close();
                            break;
                    }
                }
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    return true;
                }
                return false;
            }
            return false;
        }

        void handleWindowEevents()
        {
            while (window->isOpen())
            {
                sf::Event evnt;
                while (window->pollEvent(evnt))
                {
                    switch(evnt.type)
                    {
                        case sf::Event::Closed:
                            window->close();
                            break;
                    }
                }
                
                render_shapes();
                window->display();
            }
        }
};

#endif