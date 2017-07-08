#include "mapGenerators\randomWalkGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace ProceduralCaves
{
	RandomWalkGenerator::RandomWalkGenerator()
		: _width{ 10 }, _height{ 10 }, _fillPercentage{ 50 }, _totalFloorCells{ 0 }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	RandomWalkGenerator::RandomWalkGenerator(int width, int height, int fillPercentage)
		: _width{ width }, _height{ height }, _fillPercentage{ fillPercentage }
	{
		_totalFloorCells = (_width*_height*_fillPercentage) / 100;
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	Map RandomWalkGenerator::GenerateMap()
	{
		std::srand(static_cast<unsigned int>(std::time(0))); // use current time as seed for random generator
		int minX = 1;
		int maxX = _width - 1;
		int minY = 1;
		int maxY = _height - 1;
		int randomX = minX + (std::rand() % (maxX - minX + 1));
		int randomY = minY + (std::rand() % (maxY - minY + 1));
		Cell currentCell{ randomX, randomY };

		for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _height; ++j)
		{
			_map[i][j] = 1;
		}

		int floorCells = 0;
		while (floorCells < _totalFloorCells)
		{
			Cell nextCell = GetNextCell(currentCell, std::rand() % 4);

			if (nextCell.x > 1 && nextCell.x < _width - 1 && nextCell.y > 1 && nextCell.y < _height - 1)
			{
				currentCell = nextCell;
				if (_map[currentCell.x][currentCell.y] == 1)
				{
					_map[currentCell.x][currentCell.y] = 0;
					floorCells++;
				}
			}
		}
		
		return _map;
	}

	Cell RandomWalkGenerator::GetNextCell(Cell currentCell, int direction)
	{
		Cell nextCell;

		switch (direction)
		{
		default:
		case 0: // left
			nextCell.x = currentCell.x - 1;
			nextCell.y = currentCell.y;
			break;
		case 1: // up
			nextCell.x = currentCell.x;
			nextCell.y = currentCell.y - 1;
			break;
		case 2: // right
			nextCell.x = currentCell.x + 1;
			nextCell.y = currentCell.y;
			break;
		case 3: // down
			nextCell.x = currentCell.x;
			nextCell.y = currentCell.y + 1;
			break;
		}

		return nextCell;
	}

	bool RandomWalkGenerator::IsOutOfBounds(int x, int y) const
	{
		return x < 0 || x >= _width || y < 0 || y >= _height;
	}

	std::vector<Cell> RandomWalkGenerator::GetRegionCells(int startX, int startY) const
	{
		std::vector<Cell> cells;
		std::vector<std::vector<bool>> processedCells;
		int cellType = _map[startX][startY];
		std::vector<Cell> queue;

		processedCells = std::vector <std::vector<bool>>(_width);
		for (int i = 0; i < _width; ++i)
			processedCells[i] = std::vector<bool>(_height);

		queue.push_back(Cell(startX, startY));
		processedCells[startX][startY] = true;

		while (queue.size() > 0)
		{
			Cell cell = queue.front();
			queue.erase(queue.begin());
			cells.push_back(cell);


			for (int x = cell.x - 1; x <= cell.x + 1; ++x)
			for (int y = cell.y - 1; y <= cell.y + 1; ++y)
			{
				if (!IsOutOfBounds(x, y) && (x == cell.x || y == cell.y))
				if (!processedCells[x][y] && _map[x][y] == cellType)
				{
					processedCells[x][y] = true;
					queue.push_back(Cell(x, y));
				}
			}
		}

		return cells;
	}

	std::vector<std::vector<Cell>> RandomWalkGenerator::GetRegions(int cellType) const
	{
		std::vector<std::vector<Cell>> regions;
		std::vector<std::vector<bool>> processedCells;

		regions = std::vector <std::vector<Cell>>();

		processedCells = std::vector <std::vector<bool>>(_width);
		for (int i = 0; i < _width; ++i)
			processedCells[i] = std::vector<bool>(_height);

		for (int x = 0; x < _width; ++x)
		for (int y = 0; y < _height; ++y)
		{
			if (!processedCells[x][y] && _map[x][y] == cellType)
			{
				std::vector<Cell> newRegion = GetRegionCells(x, y);
				regions.push_back(newRegion);

				for (Cell cell : newRegion)
					processedCells[cell.x][cell.y] = true;
			}
		}

		return regions;
	}

	Map RandomWalkGenerator::CleanMapWalls(int wallThresholdSize)
	{
		std::vector<std::vector<Cell>> wallRegions = GetRegions(1);

		for (std::vector<Cell> region : wallRegions)
		if (region.size() < static_cast<unsigned int>(wallThresholdSize))
		for (Cell cell : region)
			_map[cell.x][cell.y] = 0;

		return _map;
	}

} // namespace ProceduralCaves