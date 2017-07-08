#include "caveGenerator\caveGenerator.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML\Graphics.hpp"
#include <SFML\System\Clock.hpp>
#include <SFML\Window\Event.hpp>

#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
	int mapWidth = 100;
	int mapHeight = 50;
	float cellSize = 10.0f;
	int fillProbability = 55;
	int smoothing = 5;
	bool autoSmoothing = true;
	int wallThresholdSize = 50;
	int roomThresholdSize = 50;

	sf::RenderWindow window(sf::VideoMode(mapWidth*static_cast<int>(cellSize) + 200, mapHeight*static_cast<int>(cellSize) + 200), "Procedural Map Generator");
	ImGui::SFML::Init(window);

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

	ProceduralCaves::CaveGenerator generator{ mapWidth, mapHeight, fillProbability, autoSmoothing, smoothing };

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

	sf::Clock deltaClock;

	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();

		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Map Values", NULL, ImGuiWindowFlags_AlwaysAutoResize); // begin window

		// Window title text edit
		ImGui::InputInt("Fill Probability", &fillProbability, 5);
		ImGui::InputInt("Smoothing", &smoothing);
		ImGui::Checkbox("Auto Smooth", &autoSmoothing);

		if (ImGui::Button("Smooth Map")) {
			map = generator.SmoothMap();
		}

		if (ImGui::Button("Generate New Map")) {
			generator.SetFillProbability(fillProbability);
			generator.SetAutoSmoothing(autoSmoothing);
			generator.SetSmoothing(smoothing);
			map = generator.GenerateMap();
		}

		ImGui::InputInt("Wall Threshold Size", &wallThresholdSize, 10);

		if (ImGui::Button("Clean Map Walls")) {
			map = generator.CleanMapWalls(wallThresholdSize);
		}

		ImGui::InputInt("Room Threshold Size", &roomThresholdSize, 10);

		if (ImGui::Button("Clean Map Rooms")) {
			map = generator.CleanMapRooms(roomThresholdSize);
		}

		if (ImGui::Button("Clean And Connect Map Rooms")) {
			map = generator.CleanAndConnectMapRooms(roomThresholdSize);
		}

		ImGui::End(); // end window

		window.clear();

		for (int i = 0; i < mapWidth; ++i)
		{
			for (int j = 0; j < mapHeight; ++j)
			{
				graphicMap[i][j].setPosition(window.getSize().x/2 - (mapWidth*cellSize)/2 + i*cellSize + square.getRadius(), 
											 window.getSize().y/2 - (mapHeight*cellSize)/2 + j*cellSize + square.getRadius());

				if (map[i][j] == 1)
					graphicMap[i][j].setFillColor(sf::Color(69, 115, 133));
				else
					graphicMap[i][j].setFillColor(sf::Color(35, 35, 35));

				window.draw(graphicMap[i][j]);
			}
		}

		ImGui::Render();

		window.display();

	}
}