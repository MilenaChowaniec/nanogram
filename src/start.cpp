#include "start.h"

void Start::initVar() {
	show_levels = false;
	show_custom = false;
}

void Start::initSounds() {
	opt.openFromFile("sounds/opt.wav");
}

void Start::initFont() {
	regular_font.loadFromFile("fonts/numbers.ttf");
	special_font.loadFromFile("fonts/nanogram.ttf");
}

void Start::initText() {
	new_game.text.setString("new game");
	new_game.text.setFont(regular_font);
	new_game.text.setCharacterSize(40);
	new_game.text.setFillColor(sf::Color::White);
	new_game.text.setPosition(sf::Vector2f(videoMode.width / 2 - 88, videoMode.height / 2 + 220));

	easy.text.setString("easy");
	easy.text.setCharacterSize(40);
	easy.text.setFillColor(sf::Color::White);
	easy.text.setFont(regular_font);
	easy.text.setPosition(sf::Vector2f(videoMode.width / 2 - 48, videoMode.height / 2 + 23));

	medium.text.setString("medium");
	medium.text.setCharacterSize(40);
	medium.text.setFillColor(sf::Color::White);
	medium.text.setFont(regular_font);
	medium.text.setPosition(sf::Vector2f(videoMode.width / 2 - 70, videoMode.height / 2 + 120));

	hard.text.setString("hard");
	hard.text.setCharacterSize(40);
	hard.text.setFillColor(sf::Color::White);
	hard.text.setFont(regular_font);
	hard.text.setPosition(sf::Vector2f(videoMode.width / 2 - 41, videoMode.height / 2 + 220));

	options.text.setString("options");
	options.text.setCharacterSize(40);
	options.text.setFillColor(sf::Color::White);
	options.text.setPosition(sf::Vector2f(videoMode.width / 2 - 70, videoMode.height / 2 + 350));
	options.text.setFont(regular_font);

	back_main.text.setString("back");
	back_main.text.setCharacterSize(40);
	back_main.text.setFillColor(sf::Color::White);
	back_main.text.setPosition(sf::Vector2f(videoMode.width / 2 - 45, videoMode.height / 2 + 350));
	back_main.text.setFont(regular_font);

	resume.text.setString("resume");
	resume.text.setCharacterSize(40);
	resume.text.setFillColor(sf::Color::White);
	resume.text.setFont(regular_font);

	nanogram.text.setString("nanogram.");
	nanogram.text.setCharacterSize(140);
	nanogram.text.setFillColor(sf::Color::White);
	nanogram.text.setPosition(sf::Vector2f(videoMode.width / 2 - 390, 100));
	nanogram.text.setFont(special_font);

	custom.text.setString("custom");
	custom.text.setCharacterSize(40);
	custom.text.setFillColor(sf::Color::White);
	custom.text.setPosition(sf::Vector2f(videoMode.width / 2 - 70, videoMode.height / 2 - 80));
	custom.text.setFont(regular_font);
}

void Start::initTextures() {
	rounded_square.loadFromFile("textures/new_game.png");
}

void Start::initRectangles() {
	new_game.square.setTexture(&rounded_square);
	new_game.square.setPosition(sf::Vector2f(videoMode.width / 2 - 200, videoMode.height / 2 + 200));
	new_game.square.setSize({ 400, 100 });
	new_game.bounds = new_game.square.getGlobalBounds();

	easy.square.setTexture(&rounded_square);
	easy.square.setPosition(sf::Vector2f(videoMode.width / 2 - 200, videoMode.height / 2));
	easy.square.setSize({ 400, 100 });
	easy.bounds = easy.square.getGlobalBounds();

	medium.square.setTexture(&rounded_square);
	medium.square.setPosition(sf::Vector2f(videoMode.width / 2 - 200, videoMode.height / 2 + 100));
	medium.square.setSize({ 400, 100 });
	medium.bounds = medium.square.getGlobalBounds();
	
	hard.square.setTexture(&rounded_square);
	hard.square.setPosition(sf::Vector2f(videoMode.width / 2 - 200, videoMode.height / 2 + 200));
	hard.square.setSize({ 400, 100 });
	hard.bounds = hard.square.getGlobalBounds();

	options.square.setTexture(&rounded_square);
	options.square.setPosition(sf::Vector2f(videoMode.width / 2 - 200, videoMode.height / 2 + 330));
	options.square.setSize({ 400, 100 });
	options.bounds = options.square.getGlobalBounds();

	back_main.square.setTexture(&rounded_square);
	back_main.square.setPosition(sf::Vector2f(videoMode.width / 2 - 130, videoMode.height / 2 + 330));
	back_main.square.setSize({ 260, 100 });
	back_main.bounds = back_main.square.getGlobalBounds();

	custom.square.setTexture(&rounded_square);
	custom.square.setPosition(sf::Vector2f(videoMode.width / 2 - 200, videoMode.height / 2 - 100));
	custom.square.setSize({ 400, 100 });
	custom.bounds = custom.square.getGlobalBounds();
}

Start::Start(sf::VideoMode mode, sf::RenderWindow* win, Game& g) : videoMode(mode), window(win), game(g) {
	initVar();
	initSounds();
	initFont();
	initText();
	initTextures();
	initRectangles();
}

void Start::change_color(const sf::FloatRect& bounds, sf::Text& text) {
	if (bounds.contains(cursorPos.x, cursorPos.y)) {
		text.setFillColor(sf::Color(200, 200, 200));
		cursor.loadFromSystem(sf::Cursor::Hand);
	}
	else text.setFillColor(sf::Color::White);
}

void Start::display_levels() {
	cursor.loadFromSystem(sf::Cursor::Arrow);

	this->window->draw(easy.square);
	this->window->draw(medium.square);
	this->window->draw(hard.square);
	this->window->draw(back_main.square);

	change_color(easy.bounds, easy.text);
	change_color(medium.bounds, medium.text);
	change_color(hard.bounds, hard.text);
	change_color(back_main.bounds, back_main.text);

	this->window->draw(easy.text);
	this->window->draw(medium.text);
	this->window->draw(hard.text);
	this->window->draw(back_main.text);
	this->window->draw(nanogram.text);
	this->window->setMouseCursor(cursor);
}

void Start::display_main_menu() {
	cursor.loadFromSystem(sf::Cursor::Arrow);
	this->window->draw(new_game.square);
	this->window->draw(options.square);
	
	change_color(new_game.bounds, new_game.text);
	change_color(options.bounds, options.text);

	this->window->draw(new_game.text);
	this->window->draw(options.text);
	this->window->draw(nanogram.text);

	this->window->setMouseCursor(cursor);
}

void Start::mouse_pressed_events(sf::Event& event) {
	if (!show_levels && new_game.bounds.contains(cursorPos.x, cursorPos.y)) {
		opt.play();
		show_levels = true;
	}
	else if (back_main.bounds.contains(cursorPos.x, cursorPos.y) && show_levels) {
		opt.play();
		show_levels = false;
	}
	else if (show_levels) {
		if (easy.bounds.contains(cursorPos.x, cursorPos.y)) {
			game.size = EASY;
			game.create_new_game = true;
			opt.play();
		}
		if (medium.bounds.contains(cursorPos.x, cursorPos.y)) {
			game.size = MEDIUM;
			game.create_new_game = true;
			opt.play();
		}
		if (hard.bounds.contains(cursorPos.x, cursorPos.y)) {
			game.size = HARD;
			game.create_new_game = true;
			opt.play();
		}
		if (game.create_new_game) { //zmiana sceny na nanogram, po powrocie wracamy do START, ale nie levele
			opt.play();
			game.change_scene(NANOGRAM);
			show_levels = false;
		}
		//if (custom.bounds.contains(cursorPos.x, cursorPos.y)) { 
		//	game.change_scene(CUSTOM);
		//	show_levels = true;
		//}
	}
}

void Start::handle_event(sf::Event& event) {
	switch (event.type) {
		case sf::Event::Closed: {
		    this->window->close();
			break;
		}
		case sf::Event::KeyPressed: {
			if (event.key.code == sf::Keyboard::Escape && show_levels) show_levels = false;
			else if (event.key.code == sf::Keyboard::Escape) this->window->close();
			break;
		}
		case sf::Event::MouseButtonPressed: {
			mouse_pressed_events(event);
			break;
		}
		case sf::Event::MouseMoved: {
			break;
		}
		case sf::Event::MouseButtonReleased: {
			break;
		}
	}
}

void Start::update() {
	cursorPos = sf::Mouse::getPosition(*window);
}

void Start::render() {
	this->window->clear(sf::Color(211, 211, 211));

	if (!show_levels) display_main_menu();
	if (show_levels) display_levels();

	this->window->display();
}
