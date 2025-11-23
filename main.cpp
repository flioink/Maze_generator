#include <SFML/Graphics.hpp>
#include "MazeGenerator.h"
#include <iostream>

int main()
{
    int rows = 25;
    int cols = 15;
    int cell_size = 30;    

    unsigned int window_width = cols * cell_size;
    unsigned int window_height = rows * cell_size;
    unsigned int utility_area = 100;

    sf::Vector2u screen_dimensions(window_width, window_height);

    // create the window
    sf::RenderWindow window(sf::VideoMode(screen_dimensions), "Maze Generator", sf::Style::Close); 

    // maze generator
    MazeGenerator maze(rows, cols, cell_size);
    maze.run_maze_gen(window);  

    // maze solver
    MazeSolver solver(maze, cell_size);
    solver.solve_maze();         

    window.setFramerateLimit(60);

    
    // main loop
    while (window.isOpen())
    {
        // event handler
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // clear and draw once per frame here:
        

        
        maze.draw_maze(window);        

        solver.reveal_path(window);
        
        window.display();
    }

    return 0;
}
