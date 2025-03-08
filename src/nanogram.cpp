#include "nanogram.h"

void Nanogram::initVar() {
	lives = 3;
	stillPlaying = true;
	usingHint = false;
	won = false;
}

void Nanogram::initSounds() {
	opt.openFromFile("sounds/opt.wav");
}

void Nanogram::initColors() {
	board_color = sf::Color::White;
}

void Nanogram::initFont() {
	bold_f.loadFromFile("fonts/numbers.ttf");
}

void Nanogram::initTextures() {
	pics.blank.loadFromFile("textures/blank.png");
	pics.blocked.loadFromFile("textures/blocked.png");
	pics.correct.loadFromFile("textures/board.png");
	pics.wrong.loadFromFile("textures/wrong.png");
	pics.heart.loadFromFile("textures/heart.png");
	pics.wrong_colored.loadFromFile("textures/wrong_colored.png");
	pics.lighten.loadFromFile("textures/lighten.png");
	pics.left_block.loadFromFile("textures/left_block.png");
	pics.up_block.loadFromFile("textures/up_block.png");
	pics.rounded_square.loadFromFile("textures/new_game.png");
	pics.hint.loadFromFile("textures/hint.png");
	pics.hit_hint.loadFromFile("textures/hit_hint.png");
	pics.go_back.loadFromFile("textures/back.png");
	pics.block_anim.loadFromFile("sprites/colored_block_anim.png");
}

void Nanogram::initPositions() {
	end_game.text_pos.x = videoMode.width / 2 - 100;
	end_game.text_pos.y = videoMode.height - 120 + size;
	end_game.square_pos.x = videoMode.width / 2 - 200;
	end_game.square_pos.y = videoMode.height - 135 + size;

	heart.pos.x = videoMode.width / 2 - 150;
	heart.pos.y = videoMode.height - 120 + size * 2;

	hint.pos.x = videoMode.width - 200;
	hint.pos.y = videoMode.height - 120 + size * 2;

	if (size == EASY) block_size.x = block_size.y = 60;
	else if (size == MEDIUM) block_size.x = block_size.y = 38;
	else block_size.x = block_size.y = 27;

	//kwadrat left size
	left.size.x = std::ceil(size / 2.0) * 18;
	left.size.y = block_size.x;

	//kwadrat up size
	up.size.x = block_size.x;
	up.size.y = std::ceil(size / 2.0) * 20;

	//tmp to dlugosc boku planszy
	int t = block_size.x * size + (5 * (size - 1)) - left.size.x;
	board_side = t + left.size.x;

	//pos PIERWSZEGO BLOKU planszy
	block_pos.x = (videoMode.width / 2) - (t / 2);
	block_pos.y = up.size.y + 5 + block_size.x;

	//kwadrat left pos
	left.pos.x = block_pos.x - left.size.x - 5;
	left.pos.y = block_pos.y;

	//kwadrat up pos
	up.pos.x = block_pos.x;
	up.pos.y = block_size.x;

	//text up pos
	up.text_pos.x = block_pos.x + (block_size.x / 2) - 3;
	up.text_pos.y = block_pos.y - 31;

	//text left pos
	left.text_pos.x = block_pos.x - 31;
	left.text_pos.y = block_pos.y + (block_size.x / 2) - 10;

	//line size
	line_size.x = board_side + 6;
	line_size.y = 3;
}

void Nanogram::initText() {
	//node jest do zapisania numerow do mapy??
	node.setFont(bold_f);
	node.setCharacterSize(20);

	end_game.text.setFont(bold_f);
	end_game.text.setCharacterSize(40);
	end_game.text.setFillColor(sf::Color::White);
	end_game.text.setPosition(end_game.text_pos);
	end_game.text.setString("new game");
}

void Nanogram::initRectangles() {
	left.square.setTexture(&pics.left_block, true);
	left.square.setSize(left.size);

	up.square.setTexture(&pics.up_block, true);
	up.square.setSize(up.size);

	heart.square.setTexture(&pics.heart);
	heart.square.setSize(sf::Vector2f(100 - size, 100- size));
	heart.square.setPosition(heart.pos);

	end_game.square.setPosition(end_game.square_pos);
	end_game.square.setSize(sf::Vector2f(400, 100 - size));
	end_game.square.setTexture(&pics.rounded_square);

	hint.square.setSize(sf::Vector2f(100 - size, 100- size));
	hint.square.setPosition(hint.pos);
	hint.square.setTexture(&pics.hint);

	lineH_r.setFillColor(sf::Color(157, 152, 152));
	lineH_r.setSize(line_size);
	lineH_r.setPosition(sf::Vector2f(block_pos.x - 3, block_pos.y - 3));

	lineV_r.setFillColor(sf::Color(157, 152, 152));
	lineV_r.setSize(sf::Vector2f(line_size.y, line_size.x));
	lineV_r.setPosition(sf::Vector2f(block_pos.x - 3, block_pos.y - 2));

	go_back.setTexture(&pics.go_back);
	go_back.setSize({ 90, 90 });
	go_back.setPosition({ 40, 26 });
	
}

Nanogram::Nanogram(sf::VideoMode mode, sf::RenderWindow* win, Game& g) : videoMode(mode), window(win), game(g) {
	initVar();
	initSounds();
	initColors();
	initFont();
	initTextures();
}

Nanogram::~Nanogram() {
	delete this->window;
}

void Nanogram::clear_game() {
	board_.erase(board_.begin(), board_.end());
	up.num.erase(up.num.begin(), up.num.end());
	up.num_instances.clear();
	left.num.erase(left.num.begin(), left.num.end());
	left.num_instances.clear();
}

void Nanogram::create_game() {
	size = game.size;
	lives = 3;
	won = false;
	stillPlaying = true;
	initPositions();
	initText();
	initRectangles();
	fill_board();
	fill_left_up();
}

void Nanogram::fill_board() {
	for (int i = 0; i < size; ++i) {
		board_.push_back(std::vector<Cell>());
		for (int j = 0; j < size; ++j) {

			double tmp = rand() / double(RAND_MAX);

			board_[i].push_back({ underTile(tmp > 0.3) });
			board_[i][j].square.setSize(block_size);
			board_[i][j].square.setPosition(sf::Vector2f(block_pos.x + j * (block_size.x + 5), block_pos.y + i * (block_size.y + 5)));
			board_[i][j].square.setFillColor(board_color);
			board_[i][j].tile = BLANK;
		}
	}
}

void Nanogram::fill_left_up() {
	
	int tmp = 0;
	int iterator = 0;
	int first = 0;

	for (int i = 0; i < board_.size(); ++i) {
		left.num.push_back(std::vector<Number>());
		std::unordered_map<int, int> map;
		for (int j = 0; j < board_[i].size(); ++j) {
			if (tmp == 1) first = j;

			if (board_[i][j].under == TAKEN) {
				++tmp;
				map[j] = iterator;
			}
			else if (tmp != 0){
				node.setString(std::to_string(tmp));
				left.num[i].push_back({ false, tmp, 0, node, first });
				tmp = 0;
				++iterator;
			}
		}
		if (tmp != 0) {
			node.setString(std::to_string(tmp));
			if (tmp == 1) first = 10;		
			left.num[i].push_back({ false, tmp, 0, node, first });
			tmp = 0;
		}
		left.num_instances.push_back(map);
		iterator = 0;
	}

	tmp = 0, iterator = 0, first = 0;
	for (int i = 0; i < board_.size(); ++i) {
		up.num.push_back(std::vector<Number>());

		std::unordered_map<int, int> map;

		for (int j = 0; j < board_[i].size(); ++j) {
			if (tmp == 1) first = j;

			if (board_[j][i].under == TAKEN) {
				++tmp;
				map[j] = iterator;

			}
			else if (tmp != 0) {
				node.setString(std::to_string(tmp));
				up.num[i].push_back({ false, tmp, 0, node, first });
				tmp = 0;
				++iterator;
			}
		}
		if (tmp != 0) {
			node.setString(std::to_string(tmp));
			if (tmp == 1) first = 10;
			up.num[i].push_back({ false, tmp, 0, node, first });
			tmp = 0;
		}
		up.num_instances.push_back(map);
		iterator = 0;
	}
}

void Nanogram::block_if_numbers_match(int row, int column) {

	int tmp = 0;
	for (int i = 0; i < left.num[row].size(); ++i) {
		if (left.num[row][i].isColored) ++tmp;
	}
	if (left.num[row].size() == tmp) {
		for (int i = 0; i < size; ++i) {
			if (board_[row][i].tile == BLANK) {
				board_[row][i].tile = BLOCKED;
				board_[row][i].square.setTexture(&pics.blocked);
			}

		}
	}

	tmp = 0;
	for (int i = 0; i < up.num[column].size(); ++i) {
		if (up.num[column][i].isColored) ++tmp;

		if (up.num[column].size() == tmp) {
			for (int i = 0; i < size; ++i) {
				if (board_[i][column].tile == BLANK) {
					board_[i][column].tile = BLOCKED;
					board_[i][column].square.setTexture(&pics.blocked);
				}
			}
		}
	}
}

void Nanogram::add_correct_press(int i, int j) {
	int num_idx_left = left.num_instances[i][j];
	auto& cell_left = left.num[i][num_idx_left];
	cell_left.already_pressed++;
	if (cell_left.already_pressed == cell_left.number) cell_left.isColored = true;

	int num_idx_up = up.num_instances[j][i];
	auto& cell_up = up.num[j][num_idx_up];
	cell_up.already_pressed++;
	if (cell_up.already_pressed == cell_up.number) cell_up.isColored = true;
}

bool Nanogram::hover_left_num_highlight_tiles() {
	for (int i = 0; i < left.num.size(); ++i) {
		for (int j = 0; j < left.num[i].size(); ++j) {
			auto& oneCell = left.num[i][j];

			if (oneCell.text.getGlobalBounds().contains(cursorPos.x, cursorPos.y) && oneCell.isColored) {
				cursor.loadFromSystem(sf::Cursor::Hand);
				this->window->setMouseCursor(cursor);
				for (int k = oneCell.first_instance - 1; k <= oneCell.number + oneCell.first_instance - 2; ++k) {
					if (board_[i][k].tile == CORRECT) {
						board_[i][k].tile = LIGHTEN;
						board_[i][k].square.setTexture(&pics.lighten);
					}
				}
				return true;
			}
			else {				
				cursor.loadFromSystem(sf::Cursor::Arrow);
				for (int k = oneCell.first_instance - 1; k < oneCell.number + oneCell.first_instance - 1; ++k) {
					if (board_[i][k].tile == LIGHTEN) {
						board_[i][k].tile = CORRECT;
						board_[i][k].square.setTexture(&pics.correct);
					}
				}
			}
		}
	}
	this->window->setMouseCursor(cursor);
	return false;
}

bool Nanogram::hover_up_num_highlight_tiles() {

	for (int i = 0; i < up.num.size(); ++i) {
		for (int j = 0; j < up.num[i].size(); ++j) {
			auto& oneCell = up.num[i][j];

			if (oneCell.text.getGlobalBounds().contains(cursorPos.x, cursorPos.y) && oneCell.isColored) {
				cursor.loadFromSystem(sf::Cursor::Hand);
				this->window->setMouseCursor(cursor);

				for (int k = oneCell.first_instance - 1; k <= oneCell.number + oneCell.first_instance - 2; ++k) {
					if (board_[k][i].tile == CORRECT) {
						board_[k][i].tile = LIGHTEN;
						board_[k][i].square.setTexture(&pics.lighten);
					}
				}
				return true;
			}
			else {
				cursor.loadFromSystem(sf::Cursor::Arrow);
				this->window->setMouseCursor(cursor);
				for (int k = oneCell.first_instance - 1; k < oneCell.number + oneCell.first_instance - 1; ++k) {
					if (board_[k][i].tile == LIGHTEN) {
						board_[k][i].tile = CORRECT;
						board_[k][i].square.setTexture(&pics.correct);
					}
				}
			}
		}
	}
	this->window->setMouseCursor(cursor);
	return false;
}

void Nanogram::highlight_connected_correct_tiles(int x, int y) {

	if (x < 0 || x >= size || y < 0 || y >= size || board_[x][y].tile != CORRECT) return;

	board_[x][y].tile = LIGHTEN;
	board_[x][y].square.setTexture(&pics.lighten);

	highlight_connected_correct_tiles(x + 1, y);
	highlight_connected_correct_tiles(x - 1, y);
	highlight_connected_correct_tiles(x, y + 1);
	highlight_connected_correct_tiles(x, y - 1);
}

bool Nanogram::find_cell_coords_under_cursor() {
	for (int i = 0; i < size;++i) {
		for (int j = 0; j < size;++j) {
			Cell& oneCell = board_[i][j];

			coords_square = oneCell.square.getGlobalBounds();

			if (oneCell.tile == CORRECT && coords_square.contains(cursorPos.x, cursorPos.y)) {
				coords.x = i;
				coords.y = j;
				return true;
			}
		}
	}
	return false;
}

bool Nanogram::click(int x, int y, int button) {

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size;++j) {
			Cell& oneCell = board_[i][j];

			coords_square = oneCell.square.getGlobalBounds();

			if (coords_square.contains(x, y)) {

				if (usingHint) {
					if (oneCell.tile == BLANK && oneCell.under == TAKEN) {
						oneCell.square.setTexture(&pics.correct);
						oneCell.tile = CORRECT;
						usingHint = false;
					}
					else if (oneCell.tile == BLANK && oneCell.under == EMPTY) {
						oneCell.square.setTexture(&pics.blocked);
						oneCell.tile = BLOCKED;
						usingHint = false;
					}
					add_correct_press(i, j);
					block_if_numbers_match(i, j);
					return true;
				}


				if (button == LEFT) {

					if (oneCell.tile == BLOCKED || oneCell.tile == WRONG || oneCell.tile == WRONG_COLORED || oneCell.tile == CORRECT || oneCell.tile == LIGHTEN) return true;
					if (oneCell.under == EMPTY) { //nie ma tam znaczku

						oneCell.square.setTexture(&pics.wrong);
						oneCell.tile = WRONG;

						--lives;
						std::cout << lives << std::endl;
						return false;
					}
					
					else {
						oneCell.square.setTexture(&pics.correct);
						oneCell.tile = CORRECT;
						add_correct_press(i, j);
						block_if_numbers_match(i, j);
					}
					
				}
				else if (button == RIGHT){
					if (oneCell.tile == CORRECT || oneCell.tile == WRONG || oneCell.tile == WRONG_COLORED || oneCell.tile == BLOCKED || oneCell.tile == LIGHTEN) return true;

					if (oneCell.under == TAKEN) {
						--lives;
						add_correct_press(i, j);

						oneCell.square.setTexture(&pics.wrong_colored);
						oneCell.tile = WRONG_COLORED;
						return false;
					}
					else {
						oneCell.square.setTexture(&pics.blocked);
						oneCell.tile = BLOCKED;
					}

				}
				return true;
			}
		}
	}
	return true;
}

bool Nanogram::win() {
	int m = 0, n = 0; //ilosc liczb pokolorowanych
	int amount_left = 0, amount_up = 0; //ilosc liczb 
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < left.num[i].size(); ++j) {
			++amount_left;
			if (left.num[i][j].isColored) ++m;
		}
		for (int j = 0; j < up.num[i].size(); ++j) {
			++amount_up;
			if (up.num[i][j].isColored) ++n;
		}
	}

	if (amount_left == m && amount_up == n) return true;
	else return false;
}

void Nanogram::display_end_window() {
	this->window->draw(end_game.square);
	this->window->draw(end_game.text);
	if (end_game.square.getGlobalBounds().contains(cursorPos.x, cursorPos.y)) {
		end_game.text.setFillColor(sf::Color(200, 200, 200));
		cursor.loadFromSystem(sf::Cursor::Hand);

	}
	else end_game.text.setFillColor(sf::Color::White);
	this->window->setMouseCursor(cursor);
}

void Nanogram::display_board() {
	//lines
	lineH_r.setPosition(sf::Vector2f(block_pos.x - 3, block_pos.y - 3));
	lineV_r.setPosition(sf::Vector2f(block_pos.x - 3, block_pos.y - 3));
	this->window->draw(lineV_r);
	this->window->draw(lineH_r);

	//poziomo
	lineH_r.setPosition(sf::Vector2f(block_pos.x - 3, block_pos.y + line_size.x - 6));
	lineV_r.setPosition(sf::Vector2f(block_pos.x - 6 + line_size.x, block_pos.y - 3));
	this->window->draw(lineV_r);
	this->window->draw(lineH_r);

	//lewa kolumna
	int m = 0;
	//tutaj ustalam tylko POZYCJE I KOLOR tekstu
	for (int i = 0; i < left.num.size(); ++i) {

		left.square.setPosition(left.pos.x, left.pos.y + i * (left.size.y + 5));
		this->window->draw(left.square);

		m = 0;
		for (int j = left.num[i].size() - 1; j >= 0; --j) {
			auto& oneCell = left.num[i][j];

			if (oneCell.isColored == false) oneCell.text.setFillColor(sf::Color(70, 70, 70));
			else oneCell.text.setFillColor(sf::Color(210, 210, 210));

			oneCell.text.setPosition(sf::Vector2f(left.text_pos.x - m++ * 15, left.text_pos.y + i * (left.size.y + 5)));
			this->window->draw(oneCell.text);
		}
	}

	//gorna kolumna
	for (int i = 0; i < up.num.size(); ++i) {
		up.square.setPosition(sf::Vector2f(up.pos.x + i * (up.size.x + 5), up.pos.y));
		this->window->draw(up.square);
		m = 0;

		for (int j = up.num[i].size() - 1; j >= 0; --j) {
			auto& oneCell = up.num[i][j];

			if (oneCell.isColored == false) oneCell.text.setFillColor(sf::Color(70, 70, 70));
			else oneCell.text.setFillColor(sf::Color(210, 210, 210));

			oneCell.text.setPosition(sf::Vector2f(up.text_pos.x + i * (up.size.x + 5), up.text_pos.y - m++ * 20));
			this->window->draw(oneCell.text);
		}
	}

	//glowna plansza

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			//linie na planszy
			if ((i + 1) % 5 == 0) {
				lineH_r.setPosition(sf::Vector2f(block_pos.x - 3, board_[i][j].square.getPosition().y + block_size.y + 1));
				this->window->draw(lineH_r);
			}
			if ((j + 1) % 5 == 0) {
				lineV_r.setPosition(sf::Vector2f(board_[i][j].square.getPosition().x + block_size.y + 1, block_pos.y - 3));
				this->window->draw(lineV_r);
			}


			this->window->draw(board_[i][j].square);
		}
	}
}

void Nanogram::display_game() {

	display_board();

	if (go_back.getGlobalBounds().contains(cursorPos.x, cursorPos.y)) cursor.loadFromSystem(sf::Cursor::Hand);
	this->window->draw(go_back);

	//gdy przegrana gra
	if (lives <= 0 || won) {
		stillPlaying = false;
		display_end_window();
		return;
	}

	//rysuje serca
	for (int i = 0; i < lives; ++i) {
		this->window->draw(heart.square);
		heart.square.setPosition(heart.pos.x + i * 100, heart.pos.y);
	}
	//rysuje hint
	if (stillPlaying) {
		if (!usingHint) hint.square.setTexture(&pics.hint);
		else hint.square.setTexture(&pics.hit_hint);
		this->window->draw(hint.square);
	}
	if(hint.square.getGlobalBounds().contains(cursorPos.x, cursorPos.y)) cursor.loadFromSystem(sf::Cursor::Hand);

	this->window->setMouseCursor(cursor);
}

void Nanogram::mouse_pressed_events(sf::Event& event) {
	if (event.mouseButton.button == sf::Mouse::Left && stillPlaying) {
		click(cursorPos.x, cursorPos.y, LEFT);
		isDraggingLeft = true;
	}
	if (event.mouseButton.button == sf::Mouse::Right && stillPlaying) {
		click(cursorPos.x, cursorPos.y, RIGHT);
		isDraggingRight = true;
	}
	//gdy jest koniec gry, tutaj jest nowa gra tworzona
	if (!stillPlaying && event.mouseButton.button == sf::Mouse::Left && end_game.square.getGlobalBounds().contains(cursorPos.x, cursorPos.y)) {
		clear_game();
		create_game();
		stillPlaying = true;	
	}
	//po kliknieciu na hint
	if (hint.square.getGlobalBounds().contains(cursorPos.x, cursorPos.y)) {
		if (event.mouseButton.button == sf::Mouse::Left) usingHint = true;
		else if (event.mouseButton.button == sf::Mouse::Right) usingHint = false;
	}
	if (go_back.getGlobalBounds().contains(cursorPos.x, cursorPos.y)) {
		opt.play();
		game.change_scene(START);
	}

	if (win()) won = true;

}

void Nanogram::mouse_moved_events(sf::Event& event) {
	if (isDraggingLeft && !click(cursorPos.x, cursorPos.y, LEFT)) isDraggingLeft = false;
	if (isDraggingRight && !click(cursorPos.x, cursorPos.y, RIGHT)) isDraggingRight = false;

	if (hover_up_num_highlight_tiles());
	else hover_left_num_highlight_tiles();

	if (find_cell_coords_under_cursor()) highlight_connected_correct_tiles(coords.x, coords.y);

}

void Nanogram::handle_event(sf::Event& event) {
	switch (event.type) {
		case sf::Event::Closed: {
			this->window->close();
			break;
		}
		case sf::Event::KeyPressed: {
			if (event.key.code == sf::Keyboard::Escape) game.change_scene(START);
			break;
		}
		case sf::Event::MouseButtonPressed: {
			mouse_pressed_events(event);
			break;
		}
		case sf::Event::MouseMoved: {
			mouse_moved_events(event);
			break;

		}
		case sf::Event::MouseButtonReleased: {

			if (event.mouseButton.button == sf::Mouse::Left) isDraggingLeft = false;
			else if (event.mouseButton.button == sf::Mouse::Right) isDraggingRight = false;
			break;
		}
	}
}

void Nanogram::update() {
	if (game.create_new_game) {
		clear_game();
		create_game();
		game.create_new_game = false;
	}
	cursorPos = sf::Mouse::getPosition(*window);
}

void Nanogram::render() {
	this->window->clear(sf::Color(211, 211, 211));

	display_game();

	this->window->display();
}
