#include <SFML/Graphics.hpp>

#include "MazeGenerator.h"

#include <iostream>

int main()
{

    // Create a MazeGenerator object
    int rows = 25;
    int cols = 25;
    MazeGenerator maze(rows, cols);


    // create the window
    sf::RenderWindow window(sf::VideoMode({ 500, 500 }), "Maze");
    
    maze.run_maze_gen(window);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            

            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            // Clear the window at the start of each frame
            window.clear(sf::Color::Black);

            // Draw the maze
            maze.draw_maze(window);

            // Display the current frame
            window.display();
        }

        //window.clear(sf::Color::Black); // background

        

        window.display();
    }
}