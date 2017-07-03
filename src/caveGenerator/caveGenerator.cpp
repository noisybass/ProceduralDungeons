#include "caveGenerator\caveGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

namespace ProceduralCaves
{
	CaveGenerator::CaveGenerator()
		: _width{ 10 }, _height{ 10 }, _fillProbability{ 50 }, _autoSmoothing{ false }, _smoothing{ 5 }
	{
		_cave = std::vector < std::vector<int>>(_width);
		for (int i = 0; i < _width; ++i)
		{
			_cave[i] = std::vector<int>(_height);
		}
	}

	CaveGenerator::CaveGenerator(int width, int height, int fillProbability, bool autoSmoothing, int smoothing) 
		: _width{ width }, _height{ height }, _fillProbability{ fillProbability }, _autoSmoothing{ autoSmoothing }, _smoothing{ smoothing }
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

		if (_autoSmoothing)
			for (int i = 0; i < _smoothing; ++i)
			{
				SmoothMap();
			}

		return _cave;
	}

	std::vector<std::vector<int>> CaveGenerator::SmoothMap()
	{
		std::vector<std::vector<int>> newCave(_cave);

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

		_cave = newCave;
		return _cave;
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
						neighbors += _cave[i][j];
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


} // namespace ProceduralCaves