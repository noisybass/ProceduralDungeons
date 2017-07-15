#include "mapGenerators\cellularAutomataGenerator.h"
#include "mapGenerators\randomWalkGenerator.h"
#include "mapGenerators\BSPGenerator.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML\Graphics.hpp"
#include <SFML\System\Clock.hpp>
#include <SFML\Window\Event.hpp>

#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
	int mapWidth = 200;
	int mapHeight = 100;
	float cellSize = 5.0f;
	int fillPercentage = 55;
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

	ProceduralDungeons::CellularAutomataGenerator caGenerator{ mapWidth, mapHeight, fillPercentage, autoSmoothing, smoothing };
	ProceduralDungeons::RandomWalkGenerator rwGenerator{ mapWidth, mapHeight, fillPercentage, autoSmoothing, smoothing };
	ProceduralDungeons::BSPGenerator bspGenerator{ mapWidth, mapHeight };

	ProceduralDungeons::Map map;


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

		// Select algorithm
		//const char* items[] = { "Random Cellular Automata", "Drunkard's Wals" };
		static int item = 0;
		ImGui::Combo("Select Algorithm", &item, "Random Cellular Automata\0Drunkard's Walk\0BSP");

		if (item == 0)
		{
			ImGui::InputInt("Fill Percentage", &fillPercentage, 5);
			ImGui::InputInt("Smoothing", &smoothing);
			ImGui::Checkbox("Auto Smooth", &autoSmoothing);

			if (ImGui::Button("Smooth Map")) {
				map = caGenerator.SmoothMap();
			}

			if (ImGui::Button("Generate New Map")) {
				caGenerator.SetFillPercentage(fillPercentage);
				caGenerator.SetAutoSmoothing(autoSmoothing);
				caGenerator.SetSmoothing(smoothing);
				map = caGenerator.GenerateMap();
			}

			ImGui::InputInt("Wall Threshold Size", &wallThresholdSize, 10);

			if (ImGui::Button("Clean Map Walls")) {
				map = caGenerator.CleanMapWalls(wallThresholdSize);
			}

			ImGui::InputInt("Room Threshold Size", &roomThresholdSize, 10);

			if (ImGui::Button("Clean Map Rooms")) {
				map = caGenerator.CleanMapRooms(roomThresholdSize);
			}

			if (ImGui::Button("Clean And Connect Map Rooms")) {
				map = caGenerator.CleanAndConnectMapRooms(roomThresholdSize);
			}
		}
		else if (item == 1)
		{
			ImGui::InputInt("Fill Percentage", &fillPercentage, 5);
			ImGui::InputInt("Smoothing", &smoothing);
			ImGui::Checkbox("Auto Smooth", &autoSmoothing);

			if (ImGui::Button("Generate New Map")) {
				rwGenerator.SetFillPercentage(fillPercentage);
				rwGenerator.SetAutoSmoothing(autoSmoothing);
				rwGenerator.SetSmoothing(smoothing);
				map = rwGenerator.GenerateMap();
			}

			if (ImGui::Button("Smooth Map")) {
				map = rwGenerator.SmoothMap();
			}

			ImGui::InputInt("Wall Threshold Size", &wallThresholdSize, 1, 5);

			if (ImGui::Button("Clean Map Walls")) {
				map = rwGenerator.CleanMapWalls(wallThresholdSize);
			}
		}
		else if (item == 2)
		{
			if (ImGui::Button("Generate New Map")) {
				map = bspGenerator.GenerateMap();
			}
		}

		ImGui::End(); // end window

		window.clear();

		if (map.size() != 0)
		{
			for (int i = 0; i < mapWidth; ++i)
			{
				for (int j = 0; j < mapHeight; ++j)
				{
					graphicMap[i][j].setPosition(window.getSize().x / 2 - (mapWidth*cellSize) / 2 + i*cellSize + square.getRadius(),
						window.getSize().y / 2 - (mapHeight*cellSize) / 2 + j*cellSize + square.getRadius());

					if (map[i][j] == 1)
						graphicMap[i][j].setFillColor(sf::Color(69, 115, 133));
					else if (map[i][j] == 2)
						graphicMap[i][j].setFillColor(sf::Color(255, 115, 133));
					else
						graphicMap[i][j].setFillColor(sf::Color(35, 35, 35));

					window.draw(graphicMap[i][j]);
				}
			}
		}

		ImGui::Render();

		window.display();

	}
}