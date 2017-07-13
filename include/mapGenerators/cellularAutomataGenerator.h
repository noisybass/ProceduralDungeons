#include "dataStructures.h"

#include <vector>

namespace ProceduralDungeons
{
	class CellularAutomataGenerator
	{
	public:
		CellularAutomataGenerator();
		CellularAutomataGenerator(int width, int height, int fillPercentage, bool autoSmoothing, int smoothing);

		Map GenerateMap();
		Map SmoothMap();
		Map CleanMapWalls(int wallThresholdSize);
		Map CleanMapRooms(int roomThresholdSize);
		Map CleanAndConnectMapRooms(int roomThresholdSize);
		
		void SetFillPercentage(int newFillPercentage) { _fillPercentage = newFillPercentage; }
		void SetAutoSmoothing(bool newAutoSmoothing) { _autoSmoothing = newAutoSmoothing; }
		void SetSmoothing(int newSmoothing) { _smoothing = newSmoothing; }
	private:

		Map _map; // 0 = empty, 1 = wall
		int _width;
		int _height;
		int _fillPercentage;

		bool _autoSmoothing;
		int _smoothing;

		int GetNeighborsNumber(int x, int y, int stepX, int stepY) const;
		bool IsOutOfBounds(int x, int y) const;

		std::vector<Cell> GetRegionCells(int startX, int startY) const;
		std::vector<std::vector<Cell>> GetRegions(int cellType) const;

		void ConnectClosestRooms(std::vector<Room> rooms, bool forceConnectionToMainRoom);
		void CreatePassage(Room* roomA, Room* roomB, Cell cellA, Cell cellB);
		void ConnectRooms(Room* a, Room* b) const;
		std::vector<Cell> GetLineBetweenCells(Cell from, Cell to) const;

	}; // class CellularAutomataGenerator

} // namespace ProceduralDungeons