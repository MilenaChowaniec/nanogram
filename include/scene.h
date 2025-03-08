#pragma once

#include "SFML/Window.hpp"
#include <SFML/Audio.hpp>

#include <iostream>

class Scene {
public:
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void handle_event(sf::Event& ev) = 0;
};

