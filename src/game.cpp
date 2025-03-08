#include "game.h"

void Game::initVar() {
}

void Game::initWin() {
	this->window = nullptr;
	this->videoMode.height = 950;
	this->videoMode.width = 950;
	this->window = new sf::RenderWindow(videoMode, "klocki", sf::Style::Titlebar | sf::Style::Close);
}

void Game::initScenes() {
	currScene = START;
	scenes.push_back(new Start(videoMode, window, *this));
	scenes.push_back(new Nanogram(videoMode, window, *this));
}

Game::Game() {
	initWin();
	initScenes();
}

Game::~Game() {
	for (int i = 0; i < scenes.size(); ++i) {
		delete[] scenes[i];
	}
	scenes.erase(scenes.begin(), scenes.end());
	delete window;
}

void Game::play() {

	while (running()) {
		scenes[currScene]->update();
		pollEvents();
		scenes[currScene]->render();
	}
	exit(0);
}

void Game::change_scene(sceneNum scene) {
	currScene = scene;
}

void Game::pollEvents() {
	while (this->window->pollEvent(event)) {
		scenes[currScene]->handle_event(event);
	}
}

const bool Game::running() const {
	return this->window->isOpen();
}