#include "proceduralCaves.h"

#include <vector>

namespace ProceduralCaves
{
	class RandomWalkGenerator
	{
	public:
		RandomWalkGenerator();
		RandomWalkGenerator(int width, int height);

		Map GenerateMap();

	private:

		Map _map; // 0 = empty, 1 = wall
		int _width;
		int _height;

	}; // class RandomWalkGenerator

} // namespace ProceduralCaves