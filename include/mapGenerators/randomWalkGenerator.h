#include "proceduralCaves.h"

#include <vector>

namespace ProceduralCaves
{
	class RandomWalkGenerator
	{
	public:
		RandomWalkGenerator();
		RandomWalkGenerator(int width, int height, int fillPercentage);

		Map GenerateMap();
		Map CleanMapWalls(int wallThresholdSize);

		void SetFillPercentage(int newFillPercentage) { _fillPercentage = newFillPercentage; _totalFloorCells = (_width*_height*_fillPercentage) / 100; }
	private:

		Map _map; // 0 = empty, 1 = wall
		int _width;
		int _height;
		int _fillPercentage;
		int _totalFloorCells;

		bool IsOutOfBounds(int x, int y) const;
		Cell GetNextCell(Cell currentCell, int direction);

		std::vector<Cell> GetRegionCells(int startX, int startY) const;
		std::vector<std::vector<Cell>> GetRegions(int cellType) const;

	}; // class RandomWalkGenerator

} // namespace ProceduralCaves