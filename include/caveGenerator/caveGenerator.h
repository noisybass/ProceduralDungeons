#include <vector>

namespace ProceduralCaves
{
	class CaveGenerator
	{
	public:
		CaveGenerator();
		CaveGenerator(unsigned int width, unsigned int height, unsigned int fillProbability, unsigned int smoothing);

		std::vector<std::vector<int>> GenerateMap();
	private:
		std::vector<std::vector<int>> _cave; // 0 = empty, 1 = wall
		unsigned int _width;
		unsigned int _height;
		unsigned int _fillProbability;
		unsigned int _smoothing;

		void SmoothMap();
		unsigned int GetNeighborsNumber(unsigned int row, unsigned int column) const;

	}; // class CaveGenerator

} // namespace ProceduralCaves