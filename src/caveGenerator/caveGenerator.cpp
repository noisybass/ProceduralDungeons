#include "caveGenerator\caveGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

namespace ProceduralCaves
{
	CaveGenerator::CaveGenerator() 
		: _width{ 10 }, _height{ 10 }, _fillProbability{ 50 }, _smoothing{ 5 }
	{
		_cave = std::vector < std::vector<int>>(_width);
		for (int i = 0; i < _width; ++i)
		{
			_cave[i] = std::vector<int>(_height);
		}
	}

	CaveGenerator::CaveGenerator(unsigned int width, unsigned int height, unsigned int fillProbability, unsigned int smoothing) 
		: _width{ width }, _height{ height }, _fillProbability{ fillProbability }, _smoothing{ smoothing }
	{
		_cave = std::vector < std::vector<int>>(_width);
		for (int i = 0; i < _width; ++i)
		{
			_cave[i] = std::vector<int>(_height);
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
					_cave[i][j] = 1;
				}
				else
				{
					random = min + (std::rand() % (max - min + 1));
					_cave[i][j] = (random < _fillProbability) ? 1 : 0;
				}

		for (int i = 0; i < _smoothing; ++i)
		{
			SmoothMap();
		}

		return _cave;
	}

	void CaveGenerator::SmoothMap()
	{
		std::vector<std::vector<int>> newCave(_cave);

		for (int i = 0; i < _width; ++i)
		{
			for (int j = 0; j < _height; ++j)
			{
				unsigned int neighbors = GetNeighborsNumber(i, j);

				if (neighbors > 4)
					newCave[i][j] = 1;
				else if (neighbors < 4)
					newCave[i][j] = 0;

			}
		}

		_cave = newCave;
	}

	unsigned int CaveGenerator::GetNeighborsNumber(unsigned int row, unsigned int column) const
	{
		unsigned int neighbors = 0;

		int minRow = row - 1;
		int maxRow = row + 1;
		int minColumn = column - 1;
		int maxColumn = column + 1;

		for (int i = minRow; i <= maxRow; ++i)
			for (int j = minColumn; j <= maxColumn; ++j)
				if (i >= 0 && i < _width && j >= 0 && j < _height)
				{
					if (i != row || j != column)
					{
						neighbors += _cave[i][j];
					}
				}
				else {
					neighbors++; // To increase the walls density
				}

		return neighbors;
	}


} // namespace ProceduralCaves