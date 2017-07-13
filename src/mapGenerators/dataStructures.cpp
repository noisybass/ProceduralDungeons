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

	Node::Node() : data{ 0 }, left{ nullptr }, right{ nullptr } {}

	Node::Node(int v) : data{ v }, left{ nullptr }, right{ nullptr } {}

	Node* Find(Node* node, int data)
	{
		if (!node)
			return nullptr;

		if (node->data == data)
			return node;

		if (node->data > data)
			return Find(node->left, data);
		else
			return Find(node->right, data);

	}

	void Insert(Node* node, int data)
	{
		if (!node)
		{
			node = new Node(data);
		}
		else
		{
			if (node->data > data)
				Insert(node->left, data);
			else
				Insert(node->right, data);
		}
	}

} // namespace ProceduralDungeons