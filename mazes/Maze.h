#ifndef MAZE_H
#define MAZE_H

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <ctime>

#define UP1    0
#define DOWN1  1
#define LEFT1  2
#define RIGHT1 3

class Maze {
public:
    std::vector<int> start_pos;
    std::vector<int> finish_pos;
    std::vector< std::vector< std::vector< bool > > > maze;
    Maze(int width, int height);
    void createMaze();

private:
    int width, height;
    int maze_size[2];
    int start_axis;
    int start_side;
    std::vector< std::vector< int > > dfs_path;

    void generateMaze();
    void printMaze();
    bool randomMove(bool first_move);
    void initializeMaze();
    void setSize();
    void randomPoint(bool part);
};
#endif // MAZE_H

