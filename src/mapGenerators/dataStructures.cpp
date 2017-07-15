#include "mapGenerators\dataStructures.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <algorithm>

namespace ProceduralDungeons
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

	bool Room::IsConnected(const Room& room) const
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

	BSPRoom::BSPRoom() 
		: x{ 0 }, y{ 0 }, width{ 0 }, height{ 0 }, roomId{ 0 } 
	{
		connectedRooms = std::vector<int>();
	}

	BSPRoom::BSPRoom(int roomX, int roomY, int roomWidth, int roomHeight) 
		: x{ roomX }, y{ roomY }, width{ roomWidth }, height{ roomHeight }, roomId{ 0 } 
	{
		connectedRooms = std::vector<int>();
	}

	bool BSPRoom::IsConnected(int room) const
	{
		bool found = false;
		for (auto it = connectedRooms.begin(); !found && it != connectedRooms.end(); ++it)
		{
			if (*it == room)
				found = true;
		}
		return found;
	}

} // namespace ProceduralDungeons