#include "caveGenerator\caveGenerator.h"

#include <iostream>

int main(int argc, char **argv)
{
	ProceduralCaves::CaveGenerator generator{20, 60, 50, 3};

	std::vector<std::vector<int>> map = generator.GenerateMap();

	for (std::vector<int> row : map)
	{
		for (int cell : row)
		{
			std::cout << ((cell == 1) ? "#" : " ");
		}

		std::cout << std::endl;
	}

	std::cout << "stop" << std::endl;
}