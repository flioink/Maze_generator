#pragma once

#include <stack>
#include <vector>
#include <utility>
#include <random>

#include <map>
#include <set>
#include <queue>

#include <SFML/Graphics.hpp>

using std::stack;
using std::string;
using std::vector;
using std::stack;
using std::queue;
using std::map;
using std::set;
using std::pair;
using std::make_pair;
using std::mt19937;
using std::random_device;
using sf::Color;

class MazeGenerator
{
	private:

		const int m_total_rows;
		const int m_total_cols;

		const int CELL_SIZE = 20;

		vector<vector<int>> m_cells;
		vector<vector<bool>> m_visited;

		stack<pair<int, int>> m_cell_stack;

		random_device m_rd;
		mt19937 m_rand_eng;	

		// walls bitmask
		static constexpr int NORTH = 1;   // 0001
		static constexpr int SOUTH = 2;   // 0010
		static constexpr int EAST = 4;    // 0100
		static constexpr int WEST = 8;    // 1000

	public:

		MazeGenerator(int r, int c);

		MazeGenerator(int r, int c, int cell_size);

		//~MazeGenerator();

		vector<pair<int, int>> get_unvisited_neighbors(int r, int c);		

		pair<int, int> pick_random_neighbor(const vector<pair<int, int>>& neighbors);

		void run_maze_gen(sf::RenderWindow& window);

		

		bool in_bounds(int r, int c) const; // for later

		void draw_maze(sf::RenderWindow& window);

		void print_cells_bitmasks();

		void remove_wall(int r1, int c1, int r2, int c2);		

		void draw_maze_borders(sf::RenderWindow& window);

		const vector<vector<int>>& get_cells()const { return m_cells; }

		int get_rows() const { return m_total_rows; }

		int get_cols() const { return m_total_cols; }

};



class MazeSolver
{
	private:

		const int CELL_SIZE = 20;

		const MazeGenerator& m_maze_generator;

		queue<pair<int, int>> m_hose;

		set<pair<int, int>> m_wet_areas;

		map<pair<int, int>, pair<int, int>> flow_direction;

		vector<pair<int, int>> m_found_path;		

		static constexpr int NORTH = 1;   // 0001
		static constexpr int SOUTH = 2;   // 0010
		static constexpr int EAST = 4;    // 0100
		static constexpr int WEST = 8;    // 1000

	public:
		MazeSolver(MazeGenerator& maze_gen);

		MazeSolver(MazeGenerator& maze_gen, int cell_size);

		void solve_maze();

		void flood_paths(sf::RenderWindow& window);

		void reveal_path(sf::RenderWindow& window);

		void store_solved_path(pair<int, int> start, pair<int, int> goal);

		void draw_line_between_cells(sf::RenderWindow& window, const pair<int, int>& first, const pair<int, int>& second);

		void draw_line(sf::RenderWindow& window, float x1, float y1, float x2, float y2);		

		void print_cells_bitmasks(const auto& maze_cells);

};