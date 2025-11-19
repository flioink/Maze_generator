#include "MazeGenerator.h"

#include <iostream>
#include <bitset>

using std::cout;
using std::endl;


MazeGenerator::MazeGenerator(int r, int c, int cell_size)
	:m_total_rows(r),
	m_total_cols(c),
    CELL_SIZE(cell_size),
	m_cells(r, vector<int>(c, NORTH | SOUTH | EAST | WEST)), // adds up to 1111 binary 
	m_visited(r, vector<bool>(c, false)),
    m_rand_eng(m_rd())
{
    cout << "MazeGenerator constructor invoked." << endl;
}

vector<pair<int, int>> MazeGenerator::get_unvisited_neighbors(int r, int c)
{
    vector<pair<int, int>> neighbors;

    // North
    pair<int, int> north = { r - 1, c };
    if (north.first >= 0 && north.first < m_total_rows &&
        north.second >= 0 && north.second < m_total_cols &&
        !m_visited[north.first][north.second])
    {
        neighbors.push_back(north);
    }

    // South
    pair<int, int> south = { r + 1, c };
    if (south.first >= 0 && south.first < m_total_rows &&
        south.second >= 0 && south.second < m_total_cols &&
        !m_visited[south.first][south.second])
    {
        neighbors.push_back(south);
    }

    // East
    pair<int, int> east = { r, c + 1 };
    if (east.first >= 0 && east.first < m_total_rows &&
        east.second >= 0 && east.second < m_total_cols &&
        !m_visited[east.first][east.second])
    {
        neighbors.push_back(east);
    }

    // West
    pair<int, int> west = { r, c - 1 };
    if (west.first >= 0 && west.first < m_total_rows &&
        west.second >= 0 && west.second < m_total_cols &&
        !m_visited[west.first][west.second])
    {
        neighbors.push_back(west);
    }

    return neighbors;
}

bool MazeGenerator::in_bounds(int r, int c) const
{
	return false;
}

pair<int, int> MazeGenerator::pick_random_neighbor(const vector<pair<int, int>>& neighbors)
{    
   
    std::uniform_int_distribution<> distribution(0, neighbors.size() - 1);
    return neighbors[distribution(m_rand_eng)];
}

void MazeGenerator::run_maze_gen(sf::RenderWindow& window)
{
    cout << "MazeGenerator run_maze_gen invoked." << endl;

    while (!m_cell_stack.empty()) { m_cell_stack.pop(); } // clear stack 

    pair<int, int> first_cell = {0, 0}; // initialize at 0,0

    m_visited[0][0] = true; // mark it visited


    m_cell_stack.push(first_cell);

    while (!m_cell_stack.empty())
    {


        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return; // exit if window closed
            }
        }

        // break on window close
        if (!window.isOpen()) break;



        auto current = m_cell_stack.top();
        auto neighbors = get_unvisited_neighbors(current.first, current.second);

        if (!neighbors.empty())
        {
            auto new_cell = pick_random_neighbor(neighbors);

            m_visited[new_cell.first][new_cell.second] = true;

            // remove wall logic 
            remove_wall(current.first, current.second, new_cell.first, new_cell.second);

            m_cell_stack.push(new_cell);

            
            // draw logic
            window.clear();

            
            draw_maze(window);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // TIMING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            //cout << "Drawing at: " << new_cell.first << ", " << new_cell.second << endl;
        }

        else
        {
            m_cell_stack.pop();
        }
    }
}

void MazeGenerator::draw_maze(sf::RenderWindow& window)
{

    const float CORNER = static_cast<float>(CELL_SIZE) / 4;
    const float WALL_THICKNESS = CELL_SIZE / 4;


    for (int current_row = 0; current_row < m_total_rows; ++current_row)
    {
        for (int current_col = 0; current_col < m_total_cols; ++current_col)
        {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));

            sf::Vector2f curr_pos((current_col * CELL_SIZE), (current_row * CELL_SIZE));

            cell.setPosition(curr_pos);

            // calculate block size
            float x = current_col * CELL_SIZE;
            float y = current_row * CELL_SIZE;            

            if (m_visited[current_row][current_col])
            {
                cell.setFillColor(sf::Color(55, 55, 55));

                int walls = m_cells[current_row][current_col];
                               
                // NORTH
                if (walls & NORTH)
                { 
                    //draw_wall_line(window, x, y - 1, x + CELL_SIZE, y - 1);
                    sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, WALL_THICKNESS));
                    wall.setPosition({x, y - WALL_THICKNESS / 2});
                    wall.setFillColor(sf::Color::White);
                    window.draw(wall);                    
                }                              

                
                // WEST
                if (walls & WEST)
                {                    
                    //draw_wall_line(window, x, y, x, y + CELL_SIZE);

                    sf::RectangleShape wall(sf::Vector2f(WALL_THICKNESS, CELL_SIZE));
                    wall.setPosition({(x - WALL_THICKNESS / 2), y});
                    wall.setFillColor(sf::Color::White);
                    window.draw(wall);
                }

                if ((walls & WEST) && (walls & NORTH)) // patch where north & west meet
                {
                    sf::RectangleShape angle_patch(sf::Vector2f(CELL_SIZE / 4, CELL_SIZE / 4));
                    angle_patch.setPosition({ x - CORNER / 2, y - CORNER / 2});
                    angle_patch.setFillColor(sf::Color::White);
                    window.draw(angle_patch);
                }
            }

            else
            {
                cell.setFillColor(sf::Color::Black);
            }

            window.draw(cell);
        }
    }



}

void MazeGenerator::remove_wall(int r1, int c1, int r2, int c2)
{
    int dr = r2 - r1;
    int dc = c2 - c1;

    if (dr == -1 && dc == 0)
    {
        // neighbor is north
        m_cells[r1][c1] &= ~NORTH;
        m_cells[r2][c2] &= ~SOUTH;

        cout << "North bitmask ^ " << endl;
    }

    if (dr == 1 && dc == 0)
    {
        // neighbor is south

        m_cells[r1][c1] &= ~SOUTH;
        m_cells[r2][c2] &= ~NORTH;

        cout << "South bitmask V" << endl;

    }

    if (dr == 0 && dc == 1)
    {
        // neighbor is east

        m_cells[r1][c1] &= ~EAST;
        m_cells[r2][c2] &= ~WEST;

        cout << "East bitmask >" << endl;

    }

    if (dr == 0 && dc == -1)
    {
        // neighbor is west

        m_cells[r1][c1] &= ~WEST;
        m_cells[r2][c2] &= ~EAST;

        cout << "West bitmask <" << endl;

    }
}


void MazeGenerator::draw_wall_line(sf::RenderWindow& window,
    float x1, float y1, float x2, float y2)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1), sf::Color::White),
        sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White)
    };

    window.draw(line, 2, sf::PrimitiveType::Lines);
}

