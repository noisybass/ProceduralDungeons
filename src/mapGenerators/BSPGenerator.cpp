#include "mapGenerators\BSPGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace ProceduralDungeons
{
	BSPGenerator::BSPGenerator()
		: _width{ 10 }, _height{ 10 }, _minRoomSize{ 20 }, _passagesSize{ 4 }, _nRooms{ 0 }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	BSPGenerator::BSPGenerator(int width, int height)
		: _width{ width }, _height{ height }, _minRoomSize{ 20 }, _passagesSize{ 4 }, _nRooms{ 0 }
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

		GeneratePartition(0, 0, _width, _height);

		return _map;
	}

	std::vector<BSPRoom> BSPGenerator::GeneratePartition(int x, int y, int width, int height)
	{
		std::vector<BSPRoom> roomsA;
		std::vector<BSPRoom> roomsB;
		bool splitVertical;

		if (width > height && width / height >= 1.25)
			splitVertical = true; 
		else if (height > width && height / width >= 1.25)
			splitVertical = false;
		else
			splitVertical = std::rand() % 2;


		int max = (splitVertical ? width : height) - _minRoomSize;
		if (max <= _minRoomSize)
			roomsA.push_back(CreateRoom(x, y, width, height));
		else
		{
			int randomSplit = _minRoomSize + std::rand() % (max - _minRoomSize + 1);

			if (splitVertical) // vertical
			{
				roomsA = GeneratePartition(x, y, randomSplit, height);
				roomsB = GeneratePartition(x + randomSplit, y, width - randomSplit, height);
			}
			else // horizontal
			{
				roomsA = GeneratePartition(x, y, width, randomSplit);
				roomsB = GeneratePartition(x, y + randomSplit, width, height - randomSplit);
			}

			ConnectRooms(&roomsA, &roomsB);

			roomsA.insert(roomsA.end(), roomsB.begin(), roomsB.end());
		}

		return roomsA;
	}

	BSPRoom BSPGenerator::CreateRoom(int x, int y, int width, int height)
	{
		BSPRoom room;
		int minWidth = 0.5*width;
		int minHeight = 0.5*height;
		int randomWidth = minWidth + std::rand() % (width - minWidth + 1);
		int randomHeight = minHeight + std::rand() % (height - minHeight + 1);

		int randomX = x + std::rand() % ((x + width - randomWidth) - x + 1);
		int randomY = y + std::rand() % ((y + height - randomHeight) - y + 1);

		for (int i = randomX + 1; i < randomX + randomWidth - 1; ++i)
		for (int j = randomY + 1; j < randomY + randomHeight - 1; ++j)
		{
			_map[i][j] = 0;
		}

		room.x = randomX;
		room.y = randomY;
		room.width = randomWidth;
		room.height = randomHeight;
		room.roomId = _nRooms;
		_nRooms++;

		return room;
	}

	void BSPGenerator::ConnectRooms(std::vector<BSPRoom>* roomsA, std::vector<BSPRoom>* roomsB)
	{
		int indexA = 0;
		int indexB = 0;

		if (roomsA->size() > 1)
			indexA = std::rand() % roomsA->size();
		if (roomsB->size() > 1)
			indexB = std::rand() % roomsB->size();

		if (!(*roomsA)[indexA].IsConnected((*roomsB)[indexB].roomId))
		{
			CreatePassage((*roomsA)[indexA], (*roomsB)[indexB]);
			(*roomsA)[indexA].connectedRooms.push_back((*roomsB)[indexB].roomId);
			(*roomsB)[indexB].connectedRooms.push_back((*roomsA)[indexA].roomId);
		}
	}

	void BSPGenerator::CreatePassage(BSPRoom roomA, BSPRoom roomB)
	{
		int fromX = (roomA.x + 1 + _passagesSize) + std::rand() % (roomA.width - 1 - 2 * _passagesSize);
		int fromY = (roomA.y + 1 + _passagesSize) + std::rand() % (roomA.height - 1 - 2 * _passagesSize);
		int toX = (roomB.x + 1 + _passagesSize) + std::rand() % (roomB.width - 1 - 2 * _passagesSize);
		int toY = (roomB.y + 1 + _passagesSize) + std::rand() % (roomB.height - 1 - 2 * _passagesSize);

		int dx = toX - fromX;
		int dy = toY - fromY;

		if (dx < 0)
		{
			if (dy < 0)
			{
				DrawPassage(toX, fromX, toY, toY + _passagesSize);
				DrawPassage(fromX - _passagesSize, fromX, toY, fromY);
			}
			else if (dy > 0)
			{
				DrawPassage(toX, fromX, toY - _passagesSize, toY);
				DrawPassage(fromX - _passagesSize, fromX, fromY, toY);
			}
			else
			{
				DrawPassage(toX, fromX, fromY, fromY + _passagesSize);
			}
		}
		else if (dx > 0)
		{
			if (dy < 0)
			{
				DrawPassage(fromX, toX, toY, toY + _passagesSize);
				DrawPassage(fromX, fromX + _passagesSize, toY, fromY);
			}
			else if (dy > 0)
			{
				DrawPassage(fromX, toX, toY - _passagesSize, toY);
				DrawPassage(fromX, fromX + _passagesSize, fromY, toY);
			}
			else
			{
				DrawPassage(fromX, toX, fromY - _passagesSize, fromY);
			}
		}
		else
		{
			if (dy < 0)
			{
				DrawPassage(fromX, fromX + _passagesSize, toY, fromY);
			}
			else if (dy > 0)
			{
				DrawPassage(fromX - _passagesSize, fromX, fromY, toY);
			}

		}

	}

	void BSPGenerator::DrawPassage(int startX, int finalX, int startY, int finalY)
	{
		for (int x = startX; x < finalX; ++x)
		for (int y = startY; y < finalY; ++y)
			_map[x][y] = 0;
	}

	bool BSPGenerator::IsOutOfBounds(int x, int y) const
	{
		return x < 0 || x >= _width || y < 0 || y >= _height;
	}


} // namespace ProceduralDungeons