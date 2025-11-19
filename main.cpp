#include <SFML/Graphics.hpp>
#include "MazeGenerator.h"
#include <iostream>

int main()
{
    int rows = 35;
    int cols = 15;
    int cell_size = 20;

    MazeGenerator maze(rows, cols, cell_size);

    unsigned int window_width = cols * cell_size;
    unsigned int window_height = rows * cell_size;

    sf::Vector2u screen_dimensions(window_width, window_height);

    // create the window
    sf::RenderWindow window(sf::VideoMode(screen_dimensions), "Maze");

   
    maze.run_maze_gen(window);

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
        window.clear(sf::Color::Black);
        maze.draw_maze(window);
        window.display();
    }

    return 0;
}