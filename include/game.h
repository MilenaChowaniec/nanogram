#pragma once

#include <SFML/Audio.hpp>

#include "nanogram.h"
#include "start.h"

enum sceneNum {START = 0, NANOGRAM = 1, CUSTOM = 2 };

enum boardSize { EASY = 10, MEDIUM = 15, HARD = 20 };

class Game {
	sf::RenderWindow* window;
	sf::Event event;
	sf::VideoMode videoMode;

	sceneNum currScene;
	std::vector<Scene*> scenes;

	//czas
	sf::Clock clock;


	void initVar();
	void initWin();
	void initScenes();
public:
	boardSize size;
	bool create_new_game;

	Game();
	~Game();

	void play();
	void change_scene(sceneNum scene);
	void pollEvents();

/**
* @brief Sprawdza czy okno gry jest otwarte.
*
* @return Metoda zwraca `true jesli okno gry jest otwarte, w przeciwnym wypadku zwraca `false`.
*/
	const bool running() const;
};
