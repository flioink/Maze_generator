#pragma once


#include <stack>
#include <vector>
#include <utility>
#include <random>
#include <thread>

#include <SFML/Graphics.hpp>

using std::stack;
using std::string;
using std::vector;
using std::stack;
using std::pair;
using std::make_pair;
using std::mt19937;
using std::random_device;



class MazeGenerator
{
	private:

		const int m_total_rows;
		const int m_total_cols;

		static const int CELL_SIZE = 20;

		vector<vector<int>> m_cells;
		vector<vector<bool>> m_visited;

		stack<pair<int, int>> m_cell_stack;

		random_device m_rd;
		mt19937 m_rand_eng;

	public:

		MazeGenerator(int r, int c);

		//~MazeGenerator();

		vector<pair<int, int>> get_unvisited_neighbors(int r, int c);		

		pair<int, int> pick_random_neighbor(const vector<pair<int, int>>& neighbors);

		void run_maze_gen(sf::RenderWindow& window);

		const std::vector<std::vector<int>>& get_grid() const { return m_cells; }

		bool in_bounds(int r, int c) const; // for later

		void draw_maze(sf::RenderWindow& window);

};