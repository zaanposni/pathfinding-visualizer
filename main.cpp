#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "customWindow.h"
#include "2dArray.h"
#include "astar.h"

int size = 30; // size x size field
const int size_of_pixel = 20;
const float offset = 0.04;
const int sleepMilliSecondsPerStep = 0;

bool is_number(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

int main(int argc, char *argv[])
{
	char *filePath = nullptr;
	// handle args
	if (argc < 2)
	{
		std::cout << "Usage:\n./main <size> or <filepath>\n<size> sets the size of your grid.\n<filepath> lets the program load a defined save file." << std::endl;
		std::cout << "Setting size to default value 30." << std::endl;
	}
	else
	{
		char *input = argv[1];
		if (is_number(input))  // user passed a size
		{
			size = atoi(input);
			std::cout << "Set size to " << size << std::endl;
		}
		else  // user passed a save file
		{
			filePath = argv[1];
			std::ifstream infile(filePath);
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
				if (counter == 0)
				{
					std::cout << "Size: " << x << std::endl;  // loading size of save file (first line)
					size = x;
				}
				break;
			}
		}
	}

	Pathfinding::CustomWindow window(size, size_of_pixel, offset, Grid::CustomGrid(size));

	// load config or let user paint grid
	if (filePath == nullptr)
	{
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
			if (borderNode == start || borderNode == target)
			{
				continue;
			}
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
			outfile << size << " 0" << "\n";
			outfile << start.x << " " << start.y << "\n";
			outfile << target.x << " " << target.y << "\n";
			for (Node borderNode : window.startGrid.border)
			{
				outfile << borderNode.x << " " << borderNode.y << "\n";
			}
			outfile.close();
		}
		else
		{
			f.close();
			std::cout << "\"newgrid.txt\" already exists. Move it to save your own grid." << std::endl;
		}
	}
	else
	{
		std::cout << "Opening configuration from file: " << filePath << std::endl;
		std::ifstream infile(filePath);

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
			switch (counter)
			{
			case 0:
				break;
			case 1:
				std::cout << "StartNode x: " << x << " y: " << y << std::endl;
				window.startGrid.SetStartNode(Node(x, y, true));
				window.markPixel(x, y, sf::Color(0, 131, 204), true);
				break;
			case 2:
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


	std::cout << "Starting pathfinding. Cancel by pressing space." << std::endl;
	pathfinder.FindPath(window.startGrid.startNode, window.startGrid.targetNode, window, sleepMilliSecondsPerStep);
	std::cout << "Done." << std::endl;

	window.handleWindowEevents();
	return 0;
}
