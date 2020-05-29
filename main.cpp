#include <SFML/Graphics.hpp>
#include <args.hxx>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "windowhandler.h"
#include "grid.h"
#include "astar.h"

const int default_size = 30; // size x size field
const int size_of_pixel = 20;
const float offset = 0.04;
int sleepMilliSecondsPerStep = 50;
std::string save_to_file = "newgrid.txt";


void save_grid_to_file(Grid _grid)
{
		// if saved grid does not exists save new grid to file
		std::ifstream f(save_to_file);
		if (!f.good())
		{
			f.close();
			std::ofstream outfile;
			std::cout << "Saving grid to \"" << save_to_file << "\"." << std::endl;
			outfile.open(save_to_file);
			outfile << _grid.size << " 0" << "\n";
			outfile << _grid.startNode.x << " " << _grid.startNode.y << "\n";
			outfile << _grid.targetNode.x << " " << _grid.targetNode.y << "\n";
			for (Node borderNode : _grid.border)
			{
				outfile << borderNode.x << " " << borderNode.y << "\n";
			}
			outfile.close();
		}
		else
		{
			std::cout << "\"newgrid.txt\" already exists. Move it to save your own grid." << std::endl;
		}
		f.close();
}

bool is_number(const std::string &s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it))
		++it;
	return !s.empty() && it == s.end();
}

int startPathfinding(int size)
{
	std::cout << "Creating new grid with size " << size << std::endl;
	WindowHandler window(size, size_of_pixel, offset, Grid(size));

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

	save_grid_to_file(window.startGrid);

	Astar pathfinder(size, window.startGrid);

	std::cout << "Starting pathfinding. Cancel by pressing space." << std::endl;
	pathfinder.FindPath(window.startGrid.startNode, window.startGrid.targetNode, window, sleepMilliSecondsPerStep);
	std::cout << "Done." << std::endl;

	window.handleWindowEevents();

	return 0;
}

int startPathfinding(std::string filePath)
{
	std::cout << "Loading savefile " << filePath << std::endl;
	int size = 0; // unknown

	std::ifstream check(filePath);
	if (!check.good())
	{
		std::cout << "This file does not exist." << std::endl;
		return 1;
	}
	check.close();

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

	WindowHandler window(size, size_of_pixel, offset, Grid(size));

	std::cout << "Opening configuration from file: " << filePath << std::endl;
	std::ifstream infile2(filePath);

	counter = 0;
	while (std::getline(infile2, line))
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

	Astar pathfinder(size, window.startGrid);

	std::cout << "Starting pathfinding. Cancel by pressing space." << std::endl;
	pathfinder.FindPath(window.startGrid.startNode, window.startGrid.targetNode, window, sleepMilliSecondsPerStep);
	std::cout << "Done." << std::endl;

	window.handleWindowEevents();

	return 0;
}

int main(int argc, char *argv[])
{
	args::ArgumentParser parser("Example A* pathfinding in C++ with SFML.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::ValueFlag<int> _size(parser, "size", "Set the size of a new created grid", {'s', "size"});
	args::ValueFlag<std::string> _savefile(parser, "savefile", "specify a path to a loadable savefile", {'f', "savefile"});
	args::ValueFlag<std::string> _saveat(parser, "saveat", "specify a path to save the new painted grid to", {"saveat"});
	args::ValueFlag<int> _framerate(parser, "framerate", "specify sleep time for each step in milliseconds", {"fps", "framerate"});
    args::CompletionFlag completion(parser, {"complete"});
	try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
	catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
	
	if (_framerate)
	{
		sleepMilliSecondsPerStep = args::get(_framerate);
	}
	if (_saveat)
	{
		save_to_file = args::get(_saveat);
	}

	if (_savefile)
	{
		return startPathfinding(args::get(_savefile));
	}

	int size = 30;  // default
	if (_size)
	{
		size = args::get(_size);
	}
	
	return startPathfinding(size);

}
