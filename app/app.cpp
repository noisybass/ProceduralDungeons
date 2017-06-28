#include "caveGenerator\caveGenerator.h"
#include "SFML\Graphics.hpp"

#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
	unsigned int mapWidth = 80;
	unsigned int mapHeight = 80;
	int cellSize = 10;

	sf::RenderWindow window(sf::VideoMode(mapWidth*cellSize, mapHeight*cellSize), "Procedural Map Generator");

	std::vector<std::vector<sf::CircleShape>> graphicMap;

	sf::CircleShape square(cellSize, 4);
	square.setOrigin(square.getRadius(), square.getRadius());
	square.setRotation(45.0f);
	for (int i = 0; i < mapWidth; ++i)
	{
		graphicMap.push_back(std::vector<sf::CircleShape>());
		for (int j = 0; j < mapHeight; ++j)
		{
			graphicMap[i].push_back(square);
		}
	}

	ProceduralCaves::CaveGenerator generator{mapWidth, mapHeight, 50, 3};

	std::vector<std::vector<int>> map = generator.GenerateMap();

	// Print the map
	//for (std::vector<int> row : map)
	//{
	//	for (int cell : row)
	//	{
	//		std::cout << ((cell == 1) ? "#" : " ");
	//	}

	//	std::cout << std::endl;
	//}

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				map = generator.GenerateMap();
			}
		}

		window.clear();

		for (int i = 0; i < mapWidth; ++i)
		{
			for (int j = 0; j < mapHeight; ++j)
			{
				graphicMap[i][j].setPosition(i*cellSize + square.getRadius() - 2.0f, j*cellSize + square.getRadius() - 2.0f);

				if (map[i][j] == 1)
					graphicMap[i][j].setFillColor(sf::Color(69, 115, 133));
				else
					graphicMap[i][j].setFillColor(sf::Color(35, 35, 35));

				window.draw(graphicMap[i][j]);
			}
		}

		window.display();

	}
}