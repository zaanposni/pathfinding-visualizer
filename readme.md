# Pathfinding

This project is my first attempt to implement a Pathfinding Visualizer.<br/>
For now it contains a basic implementation of the A* algorithm.

Start and Endnode and Border are dynamicaly assignable at startup.<br/>
The program then visualizes the algorithm with a predefined sleeprate.


## Example

![Example Image](example.png)
![Example Image](savefilexample.png)

## Dependency

- sfml
```bash
sudo apt-get install libsfml-dev
```

- [args](https://github.com/Taywee/args) by [@Taywee](https://github.com/Taywee)

## Compile

```bash
/usr/bin/g++ -g ./main.cpp -o ./main -lsfml-graphics -lsfml-window -lsfml-system
```

## Usage

```
OPTIONS:

      -h, --help                        Display this help menu
      -s[size], --size=[size]           Set the size of a new created grid
      -f[savefile],
      --savefile=[savefile]             specify a path to a loadable savefile
      --saveat=[saveat]                 specify a path to save the new painted
                                        grid to
      --fps=[framerate],
      --framerate=[framerate]           specify sleep time for each step
```

Note: You cannot use both parameters at once, as the size is saved in the save file.


## Save file

After painting the grid, the programm saves your grid configuration to a save file called "newgrid.txt".<br/>
If this file already exists it does not get overwritten.<br/>
Specify a file to open on startup and load a saved grid:
```bash
./main --savefile examplegrid.txt
```
If you have downloaded the repository the file "examplegrid.txt" holds the config for the grid you can see above.
