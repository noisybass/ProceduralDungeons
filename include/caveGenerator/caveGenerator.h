#include <vector>

namespace ProceduralCaves
{
	class CaveGenerator
	{
	public:
		CaveGenerator();
		CaveGenerator(int width, int height, int fillProbability, int smoothing);

		std::vector<std::vector<int>> GenerateMap();
		
		void SetFillProbability(int newFillProbability) { _fillProbability = newFillProbability; }
		void SetSmoothing(int newSmoothing) { _smoothing = newSmoothing; }
	private:
		std::vector<std::vector<int>> _cave; // 0 = empty, 1 = wall
		int _width;
		int _height;
		int _fillProbability;
		int _smoothing;

		void SmoothMap();
		int GetNeighborsNumber(int row, int column) const;

	}; // class CaveGenerator

} // namespace ProceduralCaves