#include "mapGenerators\BSPGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace ProceduralDungeons
{
	BSPGenerator::BSPGenerator()
		: _width{ 10 }, _height{ 10 }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	BSPGenerator::BSPGenerator(int width, int height)
		: _width{ width }, _height{ height }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	Map BSPGenerator::GenerateMap()
	{
		std::srand(static_cast<unsigned int>(std::time(0))); // use current time as seed for random generator

		for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _height; ++j)
		{
			_map[i][j] = 1;
		}

		GeneratePartition(0, _width, 0, _height);

		return _map;
	}

	int maxRoomX = 70;
	int maxRoomY = 50;
	int errorX = 10;
	int errorY = 10;
	int roomSize = 2000;

	void BSPGenerator::GeneratePartition(int minX, int maxX, int minY, int maxY)
	{
		bool forceHorizontal = false;
		bool forceVertical = false;

		int sizeX = maxX - minX;
		int sizeY = maxY - minY;
		//if (sizeX < roomX && sizeY < roomY)
		if (sizeX*sizeY < roomSize && sizeX < maxRoomX && sizeY < maxRoomY)
			CreateRoom(minX, maxX, minY, maxY);
		else
		{
			int orientation;

			if (sizeX < maxRoomX)
				orientation = 1;
			else if (sizeY < maxRoomY)
				orientation = 0;
			else
				orientation = std::rand() % 2;


				int min;
				int max;
			if (orientation == 0) // vertical
			{
				min = minX + sizeX * 0.2;
				max = minX + sizeX * 0.8;
				int randomX = min + std::rand() % (max - min + 1);
				GeneratePartition(minX, randomX, minY, maxY);
				GeneratePartition(randomX, maxX, minY, maxY);
			}
			else // horizontal
			{
				min = minY + sizeY * 0.2;
				max = minY + sizeY * 0.8;
				int randomY = min + std::rand() % (max - min + 1);
				GeneratePartition(minX, maxX, minY, randomY);
				GeneratePartition(minX, maxX, randomY, maxY);
			}
		}
	}

	void BSPGenerator::CreateRoom(int startX, int finalX, int startY, int finalY)
	{
		int sizeX = finalX - startX;
		int sizeY = finalY - startY;
		int minSizeX = 0.5*sizeX;
		int minSizeY = 0.5*sizeY;
		int randomSizeX = minSizeX + std::rand() % (sizeX - minSizeX + 1);
		int randomSizeY = minSizeY + std::rand() % (sizeY - minSizeY + 1);

		int randomStartX = (startX) + std::rand() % ((finalX - randomSizeX) - (startX) + 1);
		int randomStartY = (startY) + std::rand() % ((finalY - randomSizeY) - (startY) + 1);

		for (int i = randomStartX + 1; i < randomStartX + randomSizeX - 1; ++i)
		for (int j = randomStartY + 1; j < randomStartY + randomSizeY - 1; ++j)
		{
			_map[i][j] = 0;
		}
	}

	bool BSPGenerator::IsOutOfBounds(int x, int y) const
	{
		return x < 0 || x >= _width || y < 0 || y >= _height;
	}


} // namespace ProceduralDungeons