#include "caveGenerator\caveGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

namespace ProceduralCaves
{
	Cell::Cell() : x{ 0 }, y{ 0 } {}

	Cell::Cell(int a, int b) : x{ a }, y{ b } {}

	CaveGenerator::CaveGenerator()
		: _width{ 10 }, _height{ 10 }, _fillProbability{ 50 }, _autoSmoothing{ false }, _smoothing{ 5 }
	{
		_map = std::vector <std::vector<int>>(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	CaveGenerator::CaveGenerator(int width, int height, int fillProbability, bool autoSmoothing, int smoothing) 
		: _width{ width }, _height{ height }, _fillProbability{ fillProbability }, _autoSmoothing{ autoSmoothing }, _smoothing{ smoothing }
	{
		_map = std::vector <std::vector<int>>(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	std::vector<std::vector<int>> CaveGenerator::GenerateMap()
	{
		std::srand(std::time(0)); // use current time as seed for random generator
		int min = 0;
		int max = 100;
		int random = 0;

		for (int i = 0; i < _width; ++i)
			for (int j = 0; j < _height; ++j)
				if (i == 0 || i == _width - 1 || j == 0 || j == _height - 1)
				{
					_map[i][j] = 1;
				}
				else
				{
					random = min + (std::rand() % (max - min + 1));
					_map[i][j] = (random < _fillProbability) ? 1 : 0;
				}

		if (_autoSmoothing)
			for (int i = 0; i < _smoothing; ++i)
			{
				SmoothMap();
			}

		return _map;
	}

	std::vector<std::vector<int>> CaveGenerator::SmoothMap()
	{
		std::vector<std::vector<int>> newCave(_map);

		for (int i = 0; i < _width; ++i)
		{
			for (int j = 0; j < _height; ++j)
			{
				//int neighbors_1 = GetNeighborsNumber(i, j, 1, 1);
				//int neighbors_2 = GetNeighborsNumber(i, j, 2, 2);

				//if (neighbors_1 >= 5 || neighbors_2 == 0)
				//	newCave[i][j] = 1;
				//else
				//	newCave[i][j] = 0;

				//int neighbors_1 = GetNeighborsNumber(i, j, 1, 1);

				//if (_cave[i][j])
				//{
				//	if (neighbors_1 >= 4)
				//		newCave[i][j] = 1;
				//	else if (neighbors_1 < 2)
				//		newCave[i][j] = 0;
				//}
				//else
				//{
				//	if (neighbors_1 >= 5)
				//		newCave[i][j] = 1;
				//}

				int neighbors_1 = GetNeighborsNumber(i, j, 1, 1);
				if (neighbors_1 > 4)
					newCave[i][j] = 1;
				else if (neighbors_1 < 4)
					newCave[i][j] = 0;

			}
		}

		_map = newCave;
		return _map;
	}

	int CaveGenerator::GetNeighborsNumber(int x, int y, int stepX, int stepY) const
	{
		unsigned int neighbors = 0;

		int minX = x - stepX;
		int maxX = x + stepX;
		int minY = y - stepY;
		int maxY = y + stepY;

		for (int i = minX; i <= maxX; ++i)
			for (int j = minY; j <= maxY; ++j)
				if (!IsOutOfBounds(i, j))
				{
					if (i != x || j != y)
					{
						neighbors += _map[i][j];
					}
				}
				else {
					neighbors++; // To increase the walls density
				}

		return neighbors;
	}

	bool CaveGenerator::IsOutOfBounds(int x, int y) const
	{
		return x < 0 || x >= _width || y < 0 || y >= _height;
	}

	std::vector<Cell> CaveGenerator::GetRegionCells(int startX, int startY) const
	{
		std::vector<Cell> cells;
		std::vector<std::vector<bool>> processedCells;
		int cellType = _map[startX][startY];
		std::vector<Cell> queue;

		processedCells = std::vector <std::vector<bool>>(_width);
		for (int i = 0; i < _width; ++i)
		{
			processedCells[i] = std::vector<bool>(_height);
		}

		queue.push_back(Cell(startX, startY));
		processedCells[startX][startY] = true;

		while (queue.size() > 0)
		{
			Cell cell = queue.front();
			queue.erase(queue.begin());
			cells.push_back(cell);


			for (int x = cell.x - 1; x <= cell.x + 1; ++x)
			{
				for (int y = cell.y - 1; y <= cell.y + 1; ++y)
				{
					if (!IsOutOfBounds(x, y) && (x == cell.x || y == cell.y))
					{
						if (!processedCells[x][y] && _map[x][y] == cellType)
						{
							processedCells[x][y] = true;
							queue.push_back(Cell(x, y));
						}
					}
				}
			}
		}

		return cells;
	}

	std::vector<std::vector<Cell>> CaveGenerator::GetRegions(int cellType) const
	{
		std::vector<std::vector<Cell>> regions;
		std::vector<std::vector<bool>> processedCells;

		regions = std::vector <std::vector<Cell>>();

		processedCells = std::vector <std::vector<bool>>(_width);
		for (int i = 0; i < _width; ++i)
		{
			processedCells[i] = std::vector<bool>(_height);
		}

		for (int x = 0; x < _width; ++x)
			for (int y = 0; y < _height; ++y)
				if (!processedCells[x][y] && _map[x][y] == cellType)
				{
					std::vector<Cell> newRegion = GetRegionCells(x, y);
					regions.push_back(newRegion);

					for (Cell cell : newRegion)
						processedCells[cell.x][cell.y] = true;
				}

		return regions;
	}

	std::vector<std::vector<int>> CaveGenerator::CleanMapWalls(int wallThresholdSize)
	{
		std::vector<std::vector<Cell>> wallRegions = GetRegions(1);

		for (std::vector<Cell> region : wallRegions)
		{
			if (region.size() < wallThresholdSize)
				for (Cell cell : region)
					_map[cell.x][cell.y] = 0;
		}

		return _map;
	}

	std::vector<std::vector<int>> CaveGenerator::CleanMapRooms(int roomThresholdSize)
	{
		std::vector<std::vector<Cell>> roomRegions = GetRegions(0);

		for (std::vector<Cell> region : roomRegions)
		{
			if (region.size() < roomThresholdSize)
			for (Cell cell : region)
				_map[cell.x][cell.y] = 1;
		}

		return _map;
	}


} // namespace ProceduralCaves