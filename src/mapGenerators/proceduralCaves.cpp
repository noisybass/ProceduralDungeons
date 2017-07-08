#include "mapGenerators\proceduralCaves.h"

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

} // namespace ProceduralCaves