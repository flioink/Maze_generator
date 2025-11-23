#include "MazeGenerator.h"

#include <iostream>
#include <bitset>

using std::cout;
using std::endl;


MazeGenerator::MazeGenerator(int r, int c)
    :m_total_rows(r),
    m_total_cols(c),
    m_cells(r, vector<int>(c, NORTH | SOUTH | EAST | WEST)), // adds up to 1111 binary 
    m_visited(r, vector<bool>(c, false)),
    m_rand_eng(m_rd())
{
    cout << "MazeGenerator constructor with default block size invoked." << endl;
}

MazeGenerator::MazeGenerator(int r, int c, int cell_size)
	:m_total_rows(r),
	m_total_cols(c),
    CELL_SIZE(cell_size),
	m_cells(r, vector<int>(c, NORTH | SOUTH | EAST | WEST)), // adds up to 1111 binary 
	m_visited(r, vector<bool>(c, false)),
    m_rand_eng(m_rd())
{
    cout << "MazeGenerator constructor with defined block size invoked." << endl;
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

    //cout << m_cells.size() << endl;

    //print_cells_bitmasks();

    
}

void MazeGenerator::draw_maze(sf::RenderWindow& window)
{
    

    float factor = 3.0f;
    const float CORNER = static_cast<float>(CELL_SIZE) / factor;
    const float WALL_THICKNESS = static_cast<float>(CELL_SIZE / factor);
    const float OUTLINE_THICKNESS = static_cast<float>(CELL_SIZE / 10.0f);
    
    float x, y;
    sf::Color bg_color(0, 170, 255);
    sf::Color wall_color(255, 204, 153);

    sf::Color outline_color(64, 64, 64);

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
                // current setup works best with north + west wall drawing
                // south + east also works but without the patching logic
                
                
                // WEST
                if ((walls & WEST) && !(current_col==0))
                {
                    sf::RectangleShape wall(sf::Vector2f(WALL_THICKNESS, CELL_SIZE));
                    wall.setPosition({(x - WALL_THICKNESS / 2), y + wall_fudge_factor});
                    wall.setOutlineThickness(OUTLINE_THICKNESS);
                    wall.setOutlineColor(outline_color);
                    //wall.setPosition({ x + CELL_SIZE - WALL_THICKNESS, y });  // for east

                    wall.setFillColor(wall_color);

                    window.draw(wall);
                }

                // NORTH
                if (walls & NORTH && !(current_row == 0))
                {
                    sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, WALL_THICKNESS));
                    wall.setOutlineThickness(OUTLINE_THICKNESS);
                    wall.setOutlineColor(outline_color);

                    wall.setPosition({x + wall_fudge_factor, y - WALL_THICKNESS / 2});
                    //wall.setPosition({ x , y + CELL_SIZE - WALL_THICKNESS/2 });  // for south                    
                    wall.setFillColor(wall_color);

                    window.draw(wall);

                }

                if ((walls & WEST) && (walls & NORTH) && !(current_col == 0)) // patch where north & west meet
                {
                    sf::RectangleShape angle_patch(sf::Vector2f(CELL_SIZE / 3, CELL_SIZE / 3));
                    angle_patch.setOutlineThickness(OUTLINE_THICKNESS);
                    angle_patch.setOutlineColor(outline_color);

                    angle_patch.setPosition({ x - CORNER / 2, y - CORNER / 2 });
                    angle_patch.setFillColor({ 128, 128, 128 });
                    window.draw(angle_patch);
                }
            }           
        }

    }

    draw_maze_borders(window);    
}

void MazeGenerator::print_cells_bitmasks()
{
    for (int i = 0; i < m_total_rows; ++i)
    {
        for (int j = 0; j < m_total_cols; ++j)
        {
            cout << "Bitmask on m_cells from MazeGenerator: " << i << ", " << j << " : " << std::bitset<4>(m_cells[i][j]) << std::endl;
        }
    }
}

void MazeGenerator::remove_wall(int prev_row, int prev_col, int curr_row, int curr_col)
{
    int delta_row = curr_row - prev_row; //delta row
    int delta_col = curr_col - prev_col; // delta column

    if (delta_row == -1 && delta_col == 0)
    {
        // neighbor is north
        m_cells[prev_row][prev_col] &= ~NORTH;
        m_cells[curr_row][curr_col] &= ~SOUTH;
        //cout << "North bitmask ^ " << endl;
    }

    else if (delta_row == 1 && delta_col == 0)
    {
        // neighbor is south
        m_cells[prev_row][prev_col] &= ~SOUTH;
        m_cells[curr_row][curr_col] &= ~NORTH;
        //cout << "South bitmask v" << endl;
    }

    else if (delta_row == 0 && delta_col == 1)
    {
        // neighbor is east
        m_cells[prev_row][prev_col] &= ~EAST;
        m_cells[curr_row][curr_col] &= ~WEST;
        //cout << "East bitmask >" << endl;
    }

    else if (delta_row == 0 && delta_col == -1)
    {
        // neighbor is west
        m_cells[prev_row][prev_col] &= ~WEST;
        m_cells[curr_row][curr_col] &= ~EAST;
        //cout << "West bitmask <" << endl;
    }
}



void MazeGenerator::draw_maze_borders(sf::RenderWindow& window)
{
    float frame_w = CELL_SIZE / 4.0f;
    float h = m_total_rows * CELL_SIZE;

    sf::Color frame_color(128, 0, 0);    

    sf::RectangleShape border_left(sf::Vector2f(frame_w, h));
    
    // left
    border_left.setPosition({ 0, 0}); // 0,0 opening
    border_left.setFillColor(frame_color);
    window.draw(border_left);

    // right
    sf::RectangleShape border_right(sf::Vector2f(frame_w, h));
    border_right.setPosition({ float(m_total_cols * CELL_SIZE - frame_w), 0});
    border_right.setFillColor(frame_color);
    window.draw(border_right);    

    // top
    sf::RectangleShape border_top(sf::Vector2f(m_total_cols * CELL_SIZE, frame_w));
    border_top.setPosition({ 0, 0 });
    border_top.setFillColor(frame_color);
    window.draw(border_top);

    // bottom
    sf::RectangleShape border_bottom(sf::Vector2f(m_total_cols * CELL_SIZE, h));
    border_bottom.setPosition({ 0, float(m_total_rows * CELL_SIZE - frame_w) });
    border_bottom.setFillColor(frame_color);
    window.draw(border_bottom);

    sf::Color bg_color(0, 170, 255);

    // entrance
    sf::RectangleShape entrance(sf::Vector2f(CELL_SIZE / 2, CELL_SIZE / 2));    
    entrance.setPosition({0, 0});
    entrance.setFillColor(bg_color);
    entrance.setOutlineThickness(4);
    entrance.setOutlineColor(sf::Color::Cyan);
    window.draw(entrance);

    // exit
    sf::RectangleShape exit(sf::Vector2f(CELL_SIZE / 2, CELL_SIZE / 2));
    exit.setPosition({ float(m_total_cols * CELL_SIZE - CELL_SIZE / 2), h - CELL_SIZE/2});
    exit.setFillColor(bg_color);
    exit.setOutlineThickness(4);
    exit.setOutlineColor(sf::Color::Green);
    window.draw(exit);

    // black area
    sf::RectangleShape utility_area(sf::Vector2f(frame_w * CELL_SIZE, h));
    utility_area.setPosition({float(m_total_cols * CELL_SIZE), 0 });
    utility_area.setFillColor(sf::Color(0, 0, 0));
    window.draw(utility_area);

}


// Solver class
MazeSolver::MazeSolver(MazeGenerator& maze_gen):
    m_maze_generator(maze_gen)
{
    cout << "Maze solver constructor called" << endl;
}

MazeSolver::MazeSolver(MazeGenerator& maze_gen, int cell_size) :
    m_maze_generator(maze_gen), CELL_SIZE(cell_size)
{
    cout << "Maze solver constructor called" << endl;
}

void MazeSolver::solve_maze()
{

    int maze_rows = m_maze_generator.get_rows();
    int maze_cols = m_maze_generator.get_cols();

    pair<int, int> start(0, 0); // top left
    pair<int, int> goal(maze_rows - 1, maze_cols - 1); 

    const auto& maze_cells = m_maze_generator.get_cells();

    print_cells_bitmasks(maze_cells);

    m_hose.push(start);
    m_wet_areas.insert(start);
    flow_direction[start] = start; // water started here


    while (!m_hose.empty())
    {
        auto current = m_hose.front();
        m_hose.pop();

        if (current == goal)
        {
            // trace back through flow_direction to build path, later
        }

        int r = current.first;
        int c = current.second;
        int walls = maze_cells[r][c];  // get wall bitmask for current cell

        if (!(walls & NORTH) && r > 0 )
        {
            pair<int, int> neighbor(r - 1, c);

            
            if (m_wet_areas.find(neighbor) == m_wet_areas.end())
            {
                m_wet_areas.insert(neighbor);
                flow_direction[neighbor] = current;
                m_hose.push(neighbor);
            }
        }

        if (!(walls & SOUTH) && r < maze_rows - 1 )
        {
            pair<int, int> neighbor(r + 1, c);

            
            if (m_wet_areas.find(neighbor) == m_wet_areas.end())
            {
                m_wet_areas.insert(neighbor);
                flow_direction[neighbor] = current;
                m_hose.push(neighbor);
            }
        }

        if (!(walls & EAST) && c < maze_cols - 1 )
        {
            pair<int, int> neighbor(r, c + 1);

            
            if (m_wet_areas.find(neighbor) == m_wet_areas.end())
            {
                m_wet_areas.insert(neighbor);
                flow_direction[neighbor] = current;
                m_hose.push(neighbor);
            }
        }

        if (!(walls & WEST) && c > 0 )
        {
            pair<int, int> neighbor(r, c - 1);

            
            if (m_wet_areas.find(neighbor) == m_wet_areas.end())
            {
                m_wet_areas.insert(neighbor);
                flow_direction[neighbor] = current;
                m_hose.push(neighbor);
            }
        }

        
    }
}



void MazeSolver::print_cells_bitmasks(const auto& maze_cells)
{
    for (size_t r = 0; r < maze_cells.size(); ++r)
    {
        for (size_t c = 0; c < maze_cells[r].size(); ++c)
        {
            std::cout << "Cell (" << r << "," << c << ") bitmask: "
                << std::bitset<4>(maze_cells[r][c]) << std::endl;
        }
    }
}


void MazeSolver::flood_paths(sf::RenderWindow& window)
{
    //std::cout << "Drawing " << m_wet_areas.size() << " wet cells" << std::endl;

    for (auto& cell : m_wet_areas)
    {
        int r = cell.first;
        int c = cell.second;

        float x = c * CELL_SIZE + CELL_SIZE / 3;
        float y = r * CELL_SIZE + CELL_SIZE / 3;        

        sf::CircleShape water_drop(CELL_SIZE / 7);
        water_drop.setPosition({ x, y });
        water_drop.setFillColor(sf::Color::Blue);
        window.draw(water_drop);        
    }
}