#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "customWindow.h"
#include "2dArray.h"
#include "astar.h"

const int size = 30;  // size x size field
const int size_of_pixel = 20;
const float offset = 0.04;
const int sleepMilliSecondsPerStep = 10;

int main()
{
	Pathfinding::CustomWindow window(size, size_of_pixel, offset, Grid::CustomGrid(size));

	std::cout << "Please enter start: " << std::endl;
	Node start = window.getMouseClick(true);
	std::cout << "Got x: " << start.x << " y: " << start.y << std::endl;
	window.startGrid.SetStartNode(start);
	window.markPixel(start.x, start.y, sf::Color(0, 131, 204), true);

	std::cout << "Please enter target: " << std::endl;
	Node target = window.getMouseClick(true);
	std::cout << "Got x: " << target.x << " y: " << target.y << std::endl;
	window.startGrid.SetTargetNode(target);
	window.markPixel(target.x, target.y, sf::Color(185, 36, 0), true);

	std::cout << "Please mark the border: (cancel by pressing space)" << std::endl;
	while (!window.checkForKeyboardInterrupt())
	{
		Node borderNode = window.getMouseClick();
		std::cout << "Got x: " << borderNode.x << " y: " << borderNode.y << std::endl;
		window.markPixel(borderNode.x, borderNode.y, sf::Color(2, 49, 87), true);
		window.startGrid.addBorder(borderNode);
	}

	Astar::AstarPathfinding pathfinder(size, window.startGrid);

	pathfinder.FindPath(start, target, window, sleepMilliSecondsPerStep);

	window.handleWindowEevents();
	return 0;
}
