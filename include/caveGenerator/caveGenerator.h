#include <vector>

namespace ProceduralCaves
{
	class CaveGenerator
	{
	public:
		CaveGenerator();
		CaveGenerator(int width, int height, int fillProbability, bool autoSmoothing, int smoothing);

		std::vector<std::vector<int>> GenerateMap();
		std::vector<std::vector<int>> SmoothMap();
		
		void SetFillProbability(int newFillProbability) { _fillProbability = newFillProbability; }
		void SetAutoSmoothing(bool newAutoSmoothing) { _autoSmoothing = newAutoSmoothing; }
		void SetSmoothing(int newSmoothing) { _smoothing = newSmoothing; }
	private:
		std::vector<std::vector<int>> _cave; // 0 = empty, 1 = wall
		int _width;
		int _height;
		int _fillProbability;

		bool _autoSmoothing;
		int _smoothing;

		int GetNeighborsNumber(int x, int y, int stepX, int stepY) const;
		bool IsOutOfBounds(int x, int y) const;

	}; // class CaveGenerator

} // namespace ProceduralCaves