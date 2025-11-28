#include <SFML/Graphics.hpp>
#include "MazeGenerator.h"
#include <iostream>


// use mainCRTStartup in project_name->properties->linker->advanced->entry point to be able to remove the console window on startup

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

    solver.solve_maze(0, 0, window_height / cell_size - 3, window_width/cell_size - 3);

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

            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {

                    int current_x = mouseButtonPressed->position.x;
                    int current_y = mouseButtonPressed->position.y;

                    /*std::cout << "the left button was pressed" << std::endl;
                    std::cout << "mouse x: " << current_x << std::endl;
                    std::cout << "mouse y: " << current_y << std::endl;*/

                    solver.set_selection(current_y, current_x, window);

                    //solver.solve_maze(solver.get_start_pos().first, solver.get_start_pos().second, solver.get_end_pos().first, solver.get_end_pos().second);
                }

                if(mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    solver.reset_selected_start();
                    solver.reset_selected_end();
                }
            }
        }

        // clear and draw once per frame here:
        
        maze.draw_maze(window);        

        solver.reveal_path(window);

        solver.redraw_start(window);
        solver.redraw_end(window);
        
        window.display();
    }

    return 0;
}
