#include "dataStructures.h"

#include <vector>

namespace ProceduralDungeons
{
	class RandomWalkGenerator
	{
	public:
		RandomWalkGenerator();
		RandomWalkGenerator(int width, int height, int fillPercentage, bool autoSmoothing, int smoothing);

		Map GenerateMap();
		Map SmoothMap();
		Map CleanMapWalls(int wallThresholdSize);

		void SetFillPercentage(int newFillPercentage) { _fillPercentage = newFillPercentage; _totalFloorCells = (_width*_height*_fillPercentage) / 100; }
		void SetAutoSmoothing(bool newAutoSmoothing) { _autoSmoothing = newAutoSmoothing; }
		void SetSmoothing(int newSmoothing) { _smoothing = newSmoothing; }
	private:

		Map _map; // 0 = empty, 1 = wall
		int _width;
		int _height;
		int _fillPercentage;
		int _totalFloorCells;

		bool _autoSmoothing;
		int _smoothing;

		int GetNeighborsNumber(int x, int y, int stepX, int stepY) const;
		bool IsOutOfBounds(int x, int y) const;
		Cell GetNextCell(Cell currentCell, int direction);

		std::vector<Cell> GetRegionCells(int startX, int startY) const;
		std::vector<std::vector<Cell>> GetRegions(int cellType) const;

	}; // class RandomWalkGenerator

} // namespace ProceduralDungeons