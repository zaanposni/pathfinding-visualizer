#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "customWindow.h"
#include "2dArray.h"
#include "astar.h"

const int size = 30;  // size x size field
const int size_of_pixel = 20;
const float offset = 0.04;
const int sleepMilliSecondsPerStep = 10;

int main(int argc, char* argv[])
{
	Pathfinding::CustomWindow window(size, size_of_pixel, offset, Grid::CustomGrid(size));

	if (argc < 2)
	{
		std::cout << "Specify a file to open at startup to load a saved grid\n./main <filepath>" << std::endl;

		std::cout << "Please enter start: " << std::endl;
		Node start = window.getMouseClick(true);
		std::cout << "StartNode x: " << start.x << " y: " << start.y << std::endl;
		window.startGrid.SetStartNode(start);
		window.markPixel(start.x, start.y, sf::Color(0, 131, 204), true);

		std::cout << "Please enter target: " << std::endl;
		Node target = window.getMouseClick(true);
		std::cout << "TargetNode x: " << target.x << " y: " << target.y << std::endl;
		window.startGrid.SetTargetNode(target);
		window.markPixel(target.x, target.y, sf::Color(185, 36, 0), true);

		std::cout << "Please mark the border: (cancel by pressing space)" << std::endl;
		while (!window.checkForKeyboardInterrupt())
		{
			Node borderNode = window.getMouseClick();
			if (borderNode == start || borderNode == target) { continue; }
			std::cout << "BorderNode x: " << borderNode.x << " y: " << borderNode.y << std::endl;
			window.markPixel(borderNode.x, borderNode.y, sf::Color(2, 49, 87), true);
			window.startGrid.addBorder(borderNode);
		}

		// if saved grid does not exists save new grid to file
		std::ifstream f("newgrid.txt");
		if (!f.good())
		{
			f.close();
			std::ofstream outfile;
			std::cout << "Saving grid to \"newgrid.txt\"." << std::endl;
			outfile.open("newgrid.txt");
			outfile << start.x << " " << start.y << "\n";
			outfile << target.x << " " << target.y << "\n";
			for (Node borderNode : window.startGrid.border)
			{
				outfile << borderNode.x << " " << borderNode.y << "\n";
			}
			outfile.close();
		} else
		{
			f.close();
			std::cout << "\"newgrid.txt\" already exists. Move it to save your own grid." << std::endl;
		}
	} else
	{
		std::cout << "Opening configuration from file: " << argv[1] << std::endl;
		std::ifstream infile(argv[1]);
		
		std::string line;
		int counter = 0;
		while (std::getline(infile, line))
		{
			std::istringstream iss(line);
			int x, y;
			if (!(iss >> x >> y))
			{
				std::cout << "This seems like an invalid file." << std::endl;
				return 1;
			}
			switch(counter)
			{
				case 0:
					std::cout << "StartNode x: " << x << " y: " << y << std::endl;
					window.startGrid.SetStartNode(Node(x, y, true));
					window.markPixel(x, y, sf::Color(0, 131, 204), true);
					break;
				case 1:
					std::cout << "TargetNode x: " << x << " y: " << y << std::endl;
					window.startGrid.SetTargetNode(Node(x, y, true));
					window.markPixel(x, y, sf::Color(185, 36, 0), true);
					break;
				default:
					std::cout << "BorderNode x: " << x << " y: " << y << std::endl;
					window.startGrid.addBorder(Node(x, y, false));
					window.markPixel(x, y, sf::Color(2, 49, 87), true);
			}
			counter++;
		}
	}
	
	Astar::AstarPathfinding pathfinder(size, window.startGrid);

	pathfinder.FindPath(window.startGrid.startNode, window.startGrid.targetNode, window, sleepMilliSecondsPerStep);

	window.handleWindowEevents();
	return 0;
}
