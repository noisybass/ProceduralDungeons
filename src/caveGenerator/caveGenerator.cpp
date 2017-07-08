#include "caveGenerator\caveGenerator.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace ProceduralCaves
{
	Cell::Cell() : x{ 0 }, y{ 0 } {}

	Cell::Cell(int a, int b) : x{ a }, y{ b } {}

	Room::Room() : roomId{ 0 }, isMainRoom{ false }, isConnectedToMainRoom{ false }
	{
		cells = std::vector<Cell>();
		edges = std::vector<Cell>();
		connectedRooms = std::vector<Room>();
	}

	Room::Room(unsigned int id, std::vector<Cell> roomCells, Map map) : roomId{ id }, isMainRoom{ false }, isConnectedToMainRoom{ false }
	{
		cells = roomCells;
		edges = std::vector<Cell>();
		connectedRooms = std::vector<Room>();

		for (Cell cell : cells)
		{
			for (int x = cell.x - 1; x <= cell.x + 1; ++x)
			for (int y = cell.y - 1; y <= cell.y + 1; ++y)
			{
				if (x == cell.x || y == cell.y)
				{
					if (map[x][y])
						edges.push_back(cell);
				}
			}			
		}
	}

	bool Room::IsConnected(Room room) const
	{
		bool found = false;
		for (auto it = connectedRooms.begin(); !found && it != connectedRooms.end(); ++it)
		{
			if (it->roomId == room.roomId)
				found = true;
		}
		return found;
	}

	void Room::ConnectToMainRoom()
	{
		if (!isConnectedToMainRoom)
		{
			isConnectedToMainRoom = true;

			for (Room room : connectedRooms)
			{
				room.ConnectToMainRoom();
			}
		}
	}

	CaveGenerator::CaveGenerator()
		: _width{ 10 }, _height{ 10 }, _fillProbability{ 50 }, _autoSmoothing{ false }, _smoothing{ 5 }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	CaveGenerator::CaveGenerator(int width, int height, int fillProbability, bool autoSmoothing, int smoothing) 
		: _width{ width }, _height{ height }, _fillProbability{ fillProbability }, _autoSmoothing{ autoSmoothing }, _smoothing{ smoothing }
	{
		_map = Map(_width);
		for (int i = 0; i < _width; ++i)
		{
			_map[i] = std::vector<int>(_height);
		}
	}

	Map CaveGenerator::GenerateMap()
	{
		std::srand(static_cast<unsigned int>(std::time(0))); // use current time as seed for random generator
		int min = 0;
		int max = 100;
		int random = 0;

		for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _height; ++j)
		{
			if (i == 0 || i == _width - 1 || j == 0 || j == _height - 1)
			{
				_map[i][j] = 1;
			}
			else
			{
				random = min + (std::rand() % (max - min + 1));
				_map[i][j] = (random < _fillProbability) ? 1 : 0;
			}
		}

		if (_autoSmoothing)
			for (int i = 0; i < _smoothing; ++i)
				SmoothMap();

		return _map;
	}

	Map CaveGenerator::SmoothMap()
	{
		Map newCave(_map);

		for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _height; ++j)
		{
			int neighbors_1 = GetNeighborsNumber(i, j, 1, 1);

			if (neighbors_1 > 4)
				newCave[i][j] = 1;
			else if (neighbors_1 < 4)
				newCave[i][j] = 0;

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
		{
			if (!IsOutOfBounds(i, j))
			{
				if (i != x || j != y)
					neighbors += _map[i][j];
			}
			else
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

	std::vector<std::vector<Cell>> CaveGenerator::GetRegions(int cellType) const
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

	Map CaveGenerator::CleanMapWalls(int wallThresholdSize)
	{
		std::vector<std::vector<Cell>> wallRegions = GetRegions(1);

		for (std::vector<Cell> region : wallRegions)
			if (region.size() < static_cast<unsigned int>(wallThresholdSize))
				for (Cell cell : region)
					_map[cell.x][cell.y] = 0;

		return _map;
	}

	Map CaveGenerator::CleanMapRooms(int roomThresholdSize)
	{
		std::vector<std::vector<Cell>> roomRegions = GetRegions(0);

		for (std::vector<Cell> region : roomRegions)
		{
			if (region.size() < static_cast<unsigned int>(roomThresholdSize))
			for (Cell cell : region)
				_map[cell.x][cell.y] = 1;
		}

		return _map;
	}

	Map CaveGenerator::CleanAndConnectMapRooms(int roomThresholdSize)
	{
		std::vector<std::vector<Cell>> roomRegions = GetRegions(0);
		std::vector<Room> survivingRooms;
		unsigned int nSurvivingRooms = 0;

		for (std::vector<Cell> region : roomRegions)
		{
			if (region.size() < static_cast<unsigned int>(roomThresholdSize))
			{
				for (Cell cell : region)
					_map[cell.x][cell.y] = 1;
			}
			else
			{
				survivingRooms.push_back(Room{ nSurvivingRooms, region, _map });
				nSurvivingRooms++;
			}
		}

		std::sort(survivingRooms.begin(), survivingRooms.end(), [](Room a, Room b) {
			return a.cells.size() > b.cells.size();
		});
		survivingRooms[0].isMainRoom = true;
		survivingRooms[0].isConnectedToMainRoom = true;

		ConnectClosestRooms(survivingRooms, false);

		return _map;
	}

	void CaveGenerator::ConnectClosestRooms(std::vector<Room> rooms, bool forceConnectionToMainRoom)
	{
		std::vector<Room> notConnected;
		std::vector<Room> connected;

		int minDistance = _width*_width + _height*_height;
		Cell bestCellA;
		Cell bestCellB;
		int bestRoomA;
		int bestRoomB;
		bool connectionFound = false;

		if (forceConnectionToMainRoom) 
		{
			for (Room room : rooms) {
				if (room.isConnectedToMainRoom)
					connected.push_back(room);
				else
					notConnected.push_back(room);
			}
		}
		else {
			connected = rooms;
			notConnected = rooms;
		}

		for (int i = 0; i < notConnected.size(); ++i)
		{
			if (!forceConnectionToMainRoom)
			{
				connectionFound = false;
				minDistance = _width*_width + _height*_height;
			}

			//if (!notConnected[i].connectedRooms.size() > 0)
			//{
				for (int j = 0; j < connected.size(); ++j)
				{
					if (notConnected[i].roomId != connected[j].roomId && !notConnected[i].IsConnected(connected[j]))
					{
						for (Cell edgeA : notConnected[i].edges)
						for (Cell edgeB : connected[j].edges)
						{
							int dist = (edgeA.x - edgeB.x) * (edgeA.x - edgeB.x) + (edgeA.y - edgeB.y) * (edgeA.y - edgeB.y);

							if (dist < minDistance || !connectionFound)
							{
								minDistance = dist;
								connectionFound = true;

								bestCellA = edgeA;
								bestCellB = edgeB;
								bestRoomA = i;
								bestRoomB = j;
							}
						}
					}
				}
			//}
			if (connectionFound)
				CreatePassage(&rooms[bestRoomA], &rooms[bestRoomB], bestCellA, bestCellB);
		}

		//if (connectionFound && forceConnectionToMainRoom) {
		//	CreatePassage(&rooms[bestRoomA], &rooms[bestRoomB], bestCellA, bestCellB);
		//	ConnectClosestRooms(rooms, true);
		//}

		if (!forceConnectionToMainRoom) {
			ConnectClosestRooms(rooms, true);
		}
		
	}

	void CaveGenerator::CreatePassage(Room* roomA, Room* roomB, Cell cellA, Cell cellB)
	{
		ConnectRooms(roomA, roomB);

		std::vector<Cell> line = GetLineBetweenCells(cellA, cellB);
		for(Cell cell : line) {
			/*for (int x = cell.x - 1; x <= cell.x + 1; ++x)
			for (int y = cell.y - 1; y <= cell.y + 1; ++y) {
				if (!IsOutOfBounds(x, y)) {
					_map[x][y] = 0;
				}
			}*/
			int radius = 1;
			for (int x = -radius; x <= radius; ++x) 
			for (int y = -radius; y <= radius; y++) 
			{
				if (x*x + y*y <= radius*radius) 
				{
					int drawX = cell.x + x;
					int drawY = cell.y + y;
					if (!IsOutOfBounds(drawX, drawY))
						_map[drawX][drawY] = 0;
				}
			}
		}
	}

	void CaveGenerator::ConnectRooms(Room* a, Room* b) const
	{
		if (a->isConnectedToMainRoom)
			b->ConnectToMainRoom();
		else if (b->isConnectedToMainRoom)
			a->ConnectToMainRoom();
			
		a->connectedRooms.push_back(*b);
		b->connectedRooms.push_back(*a);
	}

	std::vector<Cell> CaveGenerator::GetLineBetweenCells(Cell from, Cell to) const
	{
		std::vector<Cell> line;

		int dx = to.x - from.x;
		int dy = to.y - from.y;

		int x = from.x;
		int y = from.y;

		bool inverted = false;
		int step = 0;
		int gradientStep = 0;
		int longDist = 0;
		int shortDist = 0;

		if (std::abs(dx) >= std::abs(dy))
		{
			step = dx >= 0 ? 1 : -1;
			gradientStep = dy >= 0 ? 1 : -1;

			longDist = std::abs(dx);
			shortDist = std::abs(dy);
		}
		else
		{
			inverted = true;
			step = dy >= 0 ? 1 : -1;
			gradientStep = dx >= 0 ? 1 : -1;

			longDist = std::abs(dy);
			shortDist = std::abs(dx);
		}

		int gradientAccumulation = longDist / 2;
		for (int i = 0; i < longDist; i++)
		{
			line.push_back(Cell{ x, y });

			if (inverted)
				y += step;
			else
				x += step;

			gradientAccumulation += shortDist;
			if (gradientAccumulation >= longDist)
			{
				if (inverted)
					x += gradientStep;
				else
					y += gradientStep;

				gradientAccumulation -= longDist;
			}
		}

		return line;
	}


} // namespace ProceduralCaves