#include "MazeGenerator.h"

#include <iostream>
#include <bitset>

using std::cout;
using std::endl;


MazeGenerator::MazeGenerator(int r, int c, int cell_size)
	:m_total_rows(r),
	m_total_cols(c),
    CELL_SIZE(cell_size),
	m_cells(r, vector<int>(c, NORTH | SOUTH | EAST | WEST)),
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

            std::this_thread::sleep_for(std::chrono::milliseconds(7)); // TIMING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

    


    for (int r = 0; r < m_total_rows; ++r)
    {
        for (int c = 0; c < m_total_cols; ++c)
        {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));

            sf::Vector2f curr_pos((c * CELL_SIZE), (r * CELL_SIZE));

            cell.setPosition(curr_pos);

            float x = c * CELL_SIZE;
            float y = r * CELL_SIZE;

            // TEST: Draw a thick colored rectangle around each cell
            /*sf::RectangleShape border(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            border.setPosition(sf::Vector2f(x, y));
            border.setFillColor(sf::Color::Transparent);
            border.setOutlineColor(sf::Color::Red);
            border.setOutlineThickness(2.0f);
            window.draw(border);*/


            if (m_visited[r][c])
            {
                cell.setFillColor(sf::Color::Blue);

                int walls = m_cells[r][c];


                               
                // NORTH
                if (walls & NORTH)
                { 
                    draw_line(window, x, y - 1, x + CELL_SIZE, y - 1);                   
                   
                }                              

                // SOUTH
                /*if (walls & SOUTH)
                {
                    draw_line(window, x-1, y + CELL_SIZE - 15, x + CELL_SIZE-15, y + CELL_SIZE);                    
                }*/


                /*if (walls & EAST)
                {
                    
                    draw_line(window, x + CELL_SIZE, y, x + CELL_SIZE, y + CELL_SIZE);                    
                }*/

                if (walls & WEST)
                {
                    
                    draw_line(window, x, y, x, y + CELL_SIZE);

                    
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

    std::cout << "Removing wall between (" << r1 << "," << c1
        << ") and (" << r2 << "," << c2 << ")" << std::endl;

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




void MazeGenerator::draw_line(sf::RenderWindow& window,
    float x1, float y1, float x2, float y2)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1), sf::Color::White),
        sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White)
    };

    window.draw(line, 2, sf::PrimitiveType::Lines);
}


/*std::bitset<4> binary_mask(walls);
  std::cout << "Cell (" << r << "," << c << ") walls: " << binary_mask << std::endl;*/