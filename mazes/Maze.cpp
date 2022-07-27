#include "Maze.h"

Maze::Maze(int width, int height)
{
    this->width = width;
    this->height = height;
    setSize();
    srand(time(NULL));

    // The width and height must be greater than or equal to 5 or it won't work
    // The width and height must be odd or else we will have extra walls
    for(int a = 0; a < 2; a++)
    {
        if(maze_size[a] < 5){
            maze_size[a] = 5;
        } else if(maze_size[a] % 2 == 0){
            maze_size[a]--;
        }
    }
}

void Maze::createMaze()
{
    initializeMaze();
    randomPoint(false);
    randomPoint(true);
    generateMaze();
    printMaze();
}

void Maze::initializeMaze()
{
    for(int a = 0; a < maze_size[1]; a++){
        for(int b = 0; b < maze_size[0]; b++){
            bool is_border;

            if(a == 0 || a == maze_size[1] - 1 ||
               b == 0 || b == maze_size[0] - 1){
                is_border = true;
            } else {
                is_border = false;
            }

            // just a new cell
            std::vector< bool > new_cell = {true, is_border};
            // a renewed?
            if((unsigned int) a + 1 > maze.size()){
                // yes, add first cell to new row
                std::vector< std::vector< bool > > new_row = {new_cell};
                maze.push_back(new_row);
            } else {
                // no, add cell to current row
                maze[a].push_back(new_cell);
            }
        }
    }
}

void Maze::generateMaze()
{
    bool first_move = true;
    bool success = true;

    while((int) dfs_path.size() > 1 - first_move){
        if(!success){
            dfs_path.pop_back();

            if(!first_move && dfs_path.size() > 2){
                dfs_path.pop_back();
            } else {
                break;
            }

            success = true;
        }

        while(success){
            success = randomMove(first_move);

            if(first_move){
                first_move = false;
            }
        }
    }
}

bool Maze::randomMove(bool first_move)
{
    int random_neighbor;
    std::vector< std::vector< int > > unvisited_neighbors;

    // traversal 4 direction
    for(int direction = 0; direction < 4; direction++){
        int possible_pmd[2] = {0, 0};

        if(direction == UP1){
            possible_pmd[1] = -1;
        } else if(direction == DOWN1){
            possible_pmd[1] = 1;
        } else if(direction == LEFT1){
            possible_pmd[0] = -1;
        } else {
            possible_pmd[0] = 1;
        }

        if(dfs_path.back()[0] + possible_pmd[0] * 2 > 0 &&
           dfs_path.back()[0] + possible_pmd[0] * 2 < maze_size[0] - 1 &&
           dfs_path.back()[1] + possible_pmd[1] * 2 > 0 &&
           dfs_path.back()[1] + possible_pmd[1] * 2 < maze_size[1] - 1){
            // not on border
            if(!maze[dfs_path.back()[1] + possible_pmd[1] * 2]
                    [dfs_path.back()[0] + possible_pmd[0] * 2][1]){
                std::vector< int > possible_move_delta = {possible_pmd[0], possible_pmd[1]};

                unvisited_neighbors.push_back(possible_move_delta);
            }
        }
    }

    if(unvisited_neighbors.size() > 0){
        random_neighbor = rand() % unvisited_neighbors.size();

        for(int a = 0; a < !first_move + 1; a++){
            std::vector< int > new_location;

            new_location.push_back(dfs_path.back()[0] + unvisited_neighbors[random_neighbor][0]);
            new_location.push_back(dfs_path.back()[1] + unvisited_neighbors[random_neighbor][1]);

            dfs_path.push_back(new_location);

            maze[dfs_path.back()[1]][dfs_path.back()[0]][0] = false;
            maze[dfs_path.back()[1]][dfs_path.back()[0]][1] = true;
        }

        return true;
    } else {
        return false;
    }
}

void Maze::setSize()
{
      this->maze_size[0] = this->width;
      this->maze_size[1] = this->height;
}


void Maze::randomPoint(bool part)
{
    int axis;
    int side;

    if(!part){
        axis = rand() % 2;
        side = rand() % 2;

        // start position
        start_axis = axis;
        start_side = side;

    } else {
        bool done = false;

        // not the same point with start point, end loop.
        // this loop may only run once most of the time.
        while(!done){
            axis = rand() % 2;
            side = rand() % 2;

            if(axis != start_axis ||
               side != start_side){
                done = true;
            }
        }
    }

    std::vector< int > location = {0, 0};

    if(!side){
        location[!axis] = 0;
    } else {
        location[!axis] = maze_size[!axis] - 1;
    }

    location[axis] = 2 * (rand() % ((maze_size[axis] + 1) / 2 - 2)) + 1;

    if(!part){
        dfs_path.push_back(location);
    }

    // set start or end point
    // std::cout<<"maze[loc[1],loc[0]]:\t"<<location[1]<<", "<<location[0]<<std::endl;
    //maze[loc[1],loc[0]]:    9, 0      =>  maze[9][0][0] = false; maze[9][0][1] = true;
    //maze[loc[1],loc[0]]:    18, 3     =>  maze[18][3][0] = false; maze[18][3][1] = true;
    if (part)
        start_pos = location;
    else
        finish_pos = location;
    maze[location[1]][location[0]][0] = false;
    maze[location[1]][location[0]][1] = true;
}


void Maze::printMaze()
{
    for(unsigned int a = 0; a < maze.size(); a++){
        for(unsigned int b = 0; b < maze[a].size(); b++){
            if(maze[a][b][0]){
                std::cout << "¨€¨€";      // true -> wall
            } else {
                std::cout << "  ";          // false -> path
            }
        }

        std::cout << std::endl;
    }
}

