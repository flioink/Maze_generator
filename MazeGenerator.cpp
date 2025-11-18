#include "MazeGenerator.h"
//#include <SFML/Graphics.hpp> // in the header
#include <iostream>
using std::cout;
using std::endl;


MazeGenerator::MazeGenerator(int r, int c)
	:m_total_rows(r),
	m_total_cols(c),
	m_cells(r, vector<int>(c, 0)),
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

    pair<int, int> first_cell = { 0, 0 }; // initialize at 0,0

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

            m_cell_stack.push(new_cell);

            
            // draw logic
            window.clear();

            
            draw_maze(window);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            cout << "Drawing at: " << new_cell.first << ", " << new_cell.second << endl;

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

            if (m_visited[r][c])
            {
                cell.setFillColor(sf::Color::Blue);
            }
                
            else
                cell.setFillColor(sf::Color::Red);

            window.draw(cell);
        }
    }

}