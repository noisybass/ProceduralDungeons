#include "mapGenerators\randomWalkGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace ProceduralCaves
{
	RandomWalkGenerator::RandomWalkGenerator()
		: _width{ 10 }, _height{ 10 }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	RandomWalkGenerator::RandomWalkGenerator(int width, int height)
		: _width{ width }, _height{ height }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	Map RandomWalkGenerator::GenerateMap()
	{
		for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _height; ++j)
		{
			_map[i][j] = 1;
		}
		return _map;
	}

} // namespace ProceduralCaves