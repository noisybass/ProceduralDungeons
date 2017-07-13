#ifndef PROCEDURAL_CAVES_H
#define PROCEDURAL_CAVES_H

#include <vector>

namespace ProceduralDungeons
{
	typedef std::vector<std::vector<int>> Map;

	struct Cell
	{
	public:
		int x, y;

		Cell();
		Cell(int a, int b);

	}; // struct Cell

	struct Room
	{
	public:
		unsigned int roomId;
		std::vector<Cell> cells;
		std::vector<Cell> edges;
		std::vector<Room> connectedRooms;
		bool isMainRoom;
		bool isConnectedToMainRoom;

		Room();
		Room(unsigned int id, std::vector<Cell> roomCells, Map map);

		bool IsConnected(Room other) const;
		void ConnectToMainRoom();

	}; // struct Room

	struct Node
	{
	public:
		int data;
		Node* left;
		Node* right;

		Node();
		Node(int v);
	}; // struct Node

} // namespace ProceduralDungeons

#endif