# Procedural Dungeons

Different algorithm examples of procedural dungeons generation. 
Currently implemented:
* Cellular Automata Generator
* Random Walk Generator (also known as Drunkard's Walk)

## How to build the project

This is the actual structure of the project:

```
.
|--CMakeLists.txt
|--app
|   |--CMakeLists.txt
|   |--app.cpp
|--cmake_modules
|   |--FindSFML.cmake
|--include
|   |--mapGenerators
|      |--*.h
|--src
|   |--CMakeLists.txt
|   |--mapGenerators
|      |--CMakeLists.txt
|      |--*.cpp
|--libs
|   |--imgui-1.50
|   |--imgui-sfml
|   |--SFML-2.4.2
|--linuxBuild.sh
|--linuxClean.sh
|--macBuild.sh
|--macClean.sh
|--windowsBuild.bat
|--windowsClean.bat
```

All you need to build the project is provide the **/libs** folder, that includes all the depenencies of the poject. You can download this dependencies from the links I provide below.

In the project are included scripts for easy building and cleaning.

### Windows - Visual Studio 12
For building or cleaning the project execute <code> ./windowsBuild.bat </code> or <code> ./windowsClean.bat </code> in the root.

### Linux - GCC
For building or cleaning the project execute <code> sh ./linuxBuild.sh </code> or <code> sh ./linuxClean.sh </code> in the root.

### OS X - XCode
For building or cleaning the project execute <code> sh ./macBuild.sh </code> or <code> sh ./macClean.sh </code> in the root.

## Dependencies

 - **[CMake > 2.8](https://cmake.org/)** as buildsystem.
 - **[SFML > 2.4.2](https://www.sfml-dev.org/)** as graphics library
 - **[ImGUI > 1.50](https://github.com/ocornut/imgui)** as graphical user interface library
 - **[ImGui + SFML](https://github.com/eliasdaler/imgui-sfml)** as binding between SFML and ImGUI

## References

 - [Cellular Automata Method for Generating Random Cave-Like Levels [RogueBasin]](http://www.roguebasin.com/index.php?title=Cellular_Automata_Method_for_Generating_Random_Cave-Like_Levels)
 - [Random Walk Cave Generation [RogueBasin]](http://www.roguebasin.com/index.php?title=Random_Walk_Cave_Generation)
 - [Procedural Dungeon Generation Algorithm [Gamasutra]](http://www.gamasutra.com/blogs/AAdonaac/20150903/252889/Procedural_Dungeon_Generation_Algorithm.php)
 - [Algorithms for Making More Interesting Mazes [Gamasutra]](http://www.gamasutra.com/blogs/HermanTulleken/20161005/282629/Algorithms_for_making_more_interesting_mazes.php)
 - [Procedural Level Generation in Games Tutorial: Part 1 [Kim Pedersen]](https://www.raywenderlich.com/49502/procedural-level-generation-in-games-tutorial-part-1)
 - [A Handcrafted Feel: 'Unexplored' Explores Cyclic Dungeon Generation [Joris Dormans]](http://ctrl500.com/tech/handcrafted-feel-dungeon-generation-unexplored-explores-cyclic-dungeon-generation/)

## License

Procedural Caves is license under the MIT license. See LICENSE for details.