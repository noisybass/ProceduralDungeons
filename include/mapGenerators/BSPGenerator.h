#include "dataStructures.h"

#include <vector>

namespace ProceduralDungeons
{
	class BSPGenerator
	{
	public:
		BSPGenerator();
		BSPGenerator(int width, int height);

		Map GenerateMap();

	private:

		Map _map; // 0 = empty, 1 = wall
		int _width;
		int _height;

		bool IsOutOfBounds(int x, int y) const;

		void GeneratePartition(int minX, int maxX, int minY, int maxY);
		void CreateRoom(int startX, int finalX, int startY, int finalY);

	}; // class BSPGenerator

} // namespace ProceduralDungeons