#include <vector>

namespace ProceduralCaves
{
	typedef std::vector<std::vector<int>> Map;
	struct Cell
	{
	public:
		int x, y;

		Cell();
		Cell(int a, int b);
	};

	struct Room
	{
	public:
		unsigned int roomId;
		std::vector<Cell> cells;
		std::vector<Cell> edges;
		std::vector<Room> connectedRooms;

		Room();
		Room(unsigned int id, std::vector<Cell> roomCells, Map map);

		bool IsConnected(Room other) const;
	};

	class CaveGenerator
	{
	public:
		CaveGenerator();
		CaveGenerator(int width, int height, int fillProbability, bool autoSmoothing, int smoothing);

		Map GenerateMap();
		Map SmoothMap();
		Map CleanMapWalls(int wallThresholdSize);
		Map CleanMapRooms(int roomThresholdSize);
		Map CleanAndConnectMapRooms(int roomThresholdSize);
		
		void SetFillProbability(int newFillProbability) { _fillProbability = newFillProbability; }
		void SetAutoSmoothing(bool newAutoSmoothing) { _autoSmoothing = newAutoSmoothing; }
		void SetSmoothing(int newSmoothing) { _smoothing = newSmoothing; }
	private:

		Map _map; // 0 = empty, 1 = wall
		int _width;
		int _height;
		int _fillProbability;

		bool _autoSmoothing;
		int _smoothing;

		int GetNeighborsNumber(int x, int y, int stepX, int stepY) const;
		bool IsOutOfBounds(int x, int y) const;

		std::vector<Cell> GetRegionCells(int startX, int startY) const;
		std::vector<std::vector<Cell>> GetRegions(int cellType) const;

		void ConnectClosestRooms(std::vector<Room> rooms);
		void CreatePassage(Room roomA, Room roomB, Cell cellA, Cell cellB);
		void ConnectRooms(Room a, Room b) const;
		std::vector<Cell> GetLineBetweenCells(Cell from, Cell to) const;

	}; // class CaveGenerator

} // namespace ProceduralCaves