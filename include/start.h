#pragma once

#include <iostream>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "scene.h"
#include "game.h"

class Game;

class Start : public Scene {
	sf::RenderWindow* window;
	sf::Event event;
	sf::VideoMode videoMode;
	sf::Vector2i cursorPos;
	sf::Font regular_font, special_font;
	sf::Texture rounded_square;
	sf::Cursor cursor;
	sf::Music opt;

	bool show_levels, show_custom;

	Game& game;

	struct SquareWithText {
		sf::RectangleShape square;
		sf::Text text;
		sf::FloatRect bounds;
	};
	SquareWithText new_game, custom, easy, medium, hard, options, resume, nanogram, back_main;

	void initVar();
	void initSounds();
	void initFont();
	void initText();
	void initTextures();
	void initRectangles();

public:
	Start(sf::VideoMode mode, sf::RenderWindow* win, Game& g);

	void change_color(const sf::FloatRect& bounds, sf::Text& text);
	void display_main_menu();
	void display_levels();
	void mouse_pressed_events(sf::Event& event);
	void handle_event(sf::Event& ev) override;
	void update() override;
	void render() override;
};
