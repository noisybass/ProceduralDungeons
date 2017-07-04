#include <vector>

namespace ProceduralCaves
{
	struct Cell
	{
	public:
		int x, y;

		Cell();
		Cell(int a, int b);
	};

	class CaveGenerator
	{
	public:
		CaveGenerator();
		CaveGenerator(int width, int height, int fillProbability, bool autoSmoothing, int smoothing);

		std::vector<std::vector<int>> GenerateMap();
		std::vector<std::vector<int>> SmoothMap();
		std::vector<std::vector<int>> CleanMapWalls(int wallThresholdSize);
		std::vector<std::vector<int>> CleanMapRooms(int roomThresholdSize);
		
		void SetFillProbability(int newFillProbability) { _fillProbability = newFillProbability; }
		void SetAutoSmoothing(bool newAutoSmoothing) { _autoSmoothing = newAutoSmoothing; }
		void SetSmoothing(int newSmoothing) { _smoothing = newSmoothing; }
	private:

		std::vector<std::vector<int>> _map; // 0 = empty, 1 = wall
		int _width;
		int _height;
		int _fillProbability;

		bool _autoSmoothing;
		int _smoothing;

		int GetNeighborsNumber(int x, int y, int stepX, int stepY) const;
		bool IsOutOfBounds(int x, int y) const;

		std::vector<Cell> GetRegionCells(int startX, int startY) const;
		std::vector<std::vector<Cell>> GetRegions(int cellType) const;

	}; // class CaveGenerator

} // namespace ProceduralCaves