#include "MazeGenerator.h"

#include <iostream>
//#include <bitset>

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

            //std::this_thread::sleep_for(std::chrono::milliseconds(1)); // USE THIS TO SET THE MAZE BUILDING SPEED!

            //cout << "Drawing at: " << new_cell.first << ", " << new_cell.second << endl;
        }

        else
        {
            m_cell_stack.pop();
        }
    }

    //
}

void MazeGenerator::draw_maze(sf::RenderWindow& window)
{
    

    float factor = 3.0f;
    const float CORNER = static_cast<float>(CELL_SIZE) / factor;
    const float WALL_THICKNESS = CELL_SIZE / factor;
    
    float x, y;
    sf::Color bg_color(58, 58, 77);

    window.clear(bg_color); 

    float wall_fudge_factor = static_cast<float>(CELL_SIZE / 6);


    for (int current_row = 0; current_row < m_total_rows; ++current_row)
    {
        for (int current_col = 0; current_col < m_total_cols; ++current_col)
        {
            // get grid position for every cell
            x = current_col * CELL_SIZE;
            y = current_row * CELL_SIZE;                          

            if (m_visited[current_row][current_col])
            {
                
                int walls = m_cells[current_row][current_col];

                // NORTH
                if (walls & NORTH)
                {
                    sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, WALL_THICKNESS));
                    wall.setPosition({ x + wall_fudge_factor, y - WALL_THICKNESS / 2 });
                    //wall.setPosition({ x, y + CELL_SIZE - WALL_THICKNESS });  // Move INSIDE cell

                    
                    wall.setFillColor(sf::Color::White);
                    window.draw(wall);
                }
                // WEST
                if (walls & WEST)
                {
                    sf::RectangleShape wall(sf::Vector2f(WALL_THICKNESS, CELL_SIZE));
                    wall.setPosition({ (x - WALL_THICKNESS / 2), y + wall_fudge_factor});
                    //wall.setPosition({ x + CELL_SIZE - WALL_THICKNESS, y });  // Move INSIDE cell

                    wall.setFillColor(sf::Color::White);
                    window.draw(wall);
                }

                if ((walls & WEST) && (walls & NORTH)) // patch where north & west meet
                {
                    sf::RectangleShape angle_patch(sf::Vector2f(CELL_SIZE / 3, CELL_SIZE / 3));
                    angle_patch.setPosition({ x - CORNER / 2, y - CORNER / 2 });
                    angle_patch.setFillColor(sf::Color::White);
                    window.draw(angle_patch);
                }
            }           
        }

    }

    draw_maze_borders(window);
}

void MazeGenerator::remove_wall(int row1, int col1, int row2, int col2)
{
    int delta_row = row2 - row1; //delta row
    int delta_col = col2 - col1; // delta column

    if (delta_row == -1 && delta_col == 0)
    {
        // neighbor is north
        m_cells[row1][col1] &= ~NORTH;
        m_cells[row2][col2] &= ~SOUTH;
        //cout << "North bitmask ^ " << endl;
    }

    else if (delta_row == 1 && delta_col == 0)
    {
        // neighbor is south
        m_cells[row1][col1] &= ~SOUTH;
        m_cells[row2][col2] &= ~NORTH;
        //cout << "South bitmask V" << endl;
    }

    else if (delta_row == 0 && delta_col == 1)
    {
        // neighbor is east
        m_cells[row1][col1] &= ~EAST;
        m_cells[row2][col2] &= ~WEST;
        //cout << "East bitmask >" << endl;
    }

    else if (delta_row == 0 && delta_col == -1)
    {
        // neighbor is west
        m_cells[row1][col1] &= ~WEST;
        m_cells[row2][col2] &= ~EAST;
        //cout << "West bitmask <" << endl;
    }
}





void MazeGenerator::draw_maze_borders(sf::RenderWindow& window)
{
    float frame_w = CELL_SIZE / 4.0f;
    float h = m_total_rows * CELL_SIZE;

    sf::Color frame_color(sf::Color::Cyan);    

    sf::RectangleShape frame_left(sf::Vector2f(frame_w, h));
    
    // left
    frame_left.setPosition({ 0, float(CELL_SIZE) }); // 0,0 opening
    frame_left.setFillColor(frame_color);
    window.draw(frame_left);

    // right
    sf::RectangleShape frame_right(sf::Vector2f(m_total_cols * CELL_SIZE - frame_w, h - CELL_SIZE));
    frame_right.setPosition({ float(m_total_cols * CELL_SIZE - frame_w), 0});
    frame_right.setFillColor(frame_color);
    window.draw(frame_right);

    // bottom
    sf::RectangleShape frame_bottom(sf::Vector2f(m_total_cols * CELL_SIZE - CELL_SIZE, h));
    frame_bottom.setPosition({0, float(m_total_rows * CELL_SIZE - frame_w) });
    frame_bottom.setFillColor(frame_color);
    window.draw(frame_bottom);

    // top
    sf::RectangleShape frame_top(sf::Vector2f(m_total_cols * CELL_SIZE - CELL_SIZE, frame_w));
    frame_top.setPosition({ float(CELL_SIZE), 0 }); // Skip first cell for entrance
    frame_top.setFillColor(frame_color);
    window.draw(frame_top);

}
