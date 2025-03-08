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

//enum boardSize { EASY = 10, MEDIUM = 15, HARD = 20 };
enum underTile { TAKEN = 1, EMPTY = 0 };
enum tiles { WRONG = 2, BLOCKED = 3, CORRECT = 4, BLANK, WRONG_COLORED, LIGHTEN };
enum button_ { LEFT = 1, RIGHT = 2 };

enum boardSize;
class Game;

class Nanogram : public Scene {
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Cursor cursor;
	sf::Vector2i cursorPos;
	sf::Font bold_f;
	sf::Text node;
	sf::Color board_color;
	sf::Vector2f block_size, block_pos, line_size;
	sf::RectangleShape lineV_r, lineH_r, go_back;
	sf::FloatRect coords_square;
	sf::Music opt;

	sf::Time time;
	sf::Clock clock;

	bool isDraggingLeft, isDraggingRight;
	bool stillPlaying;
	bool usingHint;
	bool won;

	int lives, board_side;
	
	boardSize size;
	Game& game;

	struct SpecialBlocks {
		sf::RectangleShape square;
		sf::Vector2f pos;
	};
	SpecialBlocks heart, hint;

	struct SquareWithText {
		sf::Text text;
		sf::RectangleShape square;
		sf::Vector2f text_pos;
		sf::Vector2f square_pos;
	};
	SquareWithText end_game;

	struct Number {
		bool isColored; //Informacja czy numer zostal zakolorowany na planszy
		int number; //Numer do zakolorowania
		int already_pressed; //Ilosc aktualnie zakoolorowanych komorek
		sf::Text text; //Numer w formie string
		int first_instance; //Pierwsze pojawienie sie numeru na planszy
	};

	struct GuideNumbers {
		sf::RectangleShape square; //to jest kwadrat pod liczbami
		sf::Vector2f pos; //pozycja kwadratu pod liczbami
		sf::Vector2f text_pos; //pozycja poczatkowa liczb
		sf::Vector2f size; //size kwadratu pod liczbami
		std::vector<std::vector<Number>> num; //tablica z numerami left lub up
		std::vector<std::unordered_map<int, int>> num_instances;
		
	};
	GuideNumbers left, up;

	struct Squares {
		sf::Texture blank;
		sf::Texture correct;
		sf::Texture blocked;
		sf::Texture wrong;
		sf::Texture wrong_colored;
		sf::Texture heart;
		sf::Texture lighten;
		sf::Texture left_block;
		sf::Texture up_block;
		sf::Texture rounded_square;
		sf::Texture hint;
		sf::Texture hit_hint;
		sf::Texture go_back;
		sf::Texture block_anim;
	};
	Squares pics;

	struct Cell {
		underTile under; //Informacja czy komorka jest `TAKEN` czy `EMPTY`
		sf::RectangleShape square; //Pozycja kazdej komorki planszy.
		tiles tile; //Tekstura kazdej komorki.
	};
	std::vector<std::vector<Cell>> board_;

	struct FloodCoords {
		int x;
		int y;
	};
	FloodCoords coords;

	void initVar();
	void initSounds();
	void initColors();
	void initFont();
	void initTextures();

	void initPositions();
	void initText();
	void initRectangles();
public:

	Nanogram(sf::VideoMode mode, sf::RenderWindow* win, Game& g);
	~Nanogram();

/**
 * @brief Rozpoczyna now¹ grê.
 *
 * Metoda resetuje planszê, liczby po lewej i górnej stronie oraz inicjalizuje zmienne potrzebne do gry.
 */
	void clear_game();

/**
 * @brief Tworzy gre.
 */
	void create_game();

/**
 * @brief Wype³nia planszê gry komórkami.
 *
 * Funkcja tworzy dwuwymiarow¹ tablicê komórek o wymiarach `size x size`.
 * Ka¿da komórka jest inicjalizowana losow¹ wartoœci¹ `under` oraz pustym polem `tile`.
 * Pozycja i rozmiar graficznej reprezentacji komórki s¹ ustawiane na podstawie ustalonych parametrów.
 *
 */
	void fill_board();

/**
 * @brief Oblicza i zapisuje liczby do wyœwietlenia na lewej i górnej krawêdzi planszy.
 *
 * Funkcja przechodzi przez ka¿dy wiersz i kolumnê planszy, zliczaj¹c ci¹gi s¹siaduj¹cych komórek o wartoœci `TAKEN`.
 * Dla ka¿dego ci¹gu tworzy obiekt `Number` reprezentuj¹cy liczbê do wyœwietlenia i dodaje go do odpowiedniego wektora.
 *
 * @param board_ Dwuwymiowa tablica reprezentuj¹ca planszê gry.
 * @param left Struktura przechowuj¹ca informacje o liczbach do wyœwietlenia na lewej krawêdzi.
 * @param up Struktura przechowuj¹ca informacje o liczbach do wyœwietlenia na górnej krawêdzi.
 */
	void fill_left_up();

/**
 * @brief Blokuje ca³y wiersz lub kolumnê, jeœli wszystkie liczby na odpowiedniej krawêdzi s¹ "zaznaczone".
 *
 * Funkcja sprawdza, czy wszystkie liczby na lewej krawêdzi dla danego wiersza lub wszystkie liczby na górnej krawêdzi dla danej kolumny s¹ "zaznaczone" (maj¹ ustawione `isColored` na `true`). Jeœli tak, blokuje wszystkie puste komórki w tym wierszu lub kolumnie.
 *
 * @param row Numer wiersza do sprawdzenia.
 * @param column Numer kolumny do sprawdzenia.
 */
	void block_if_numbers_match(int row, int column);

/**
* @brief Aktualizuje stan komórek w planszy po poprawnym zaznaczeniu liczby.
*
* Ta metoda s³u¿y do zaktualizowania stanu komórek w planszy gry
* po tym, jak u¿ytkownik zaznaczy³ poprawn¹ liczbê. Zwiêksza licznik
* ju¿ zaznaczonych liczb dla danej komórki i ustawia flagê `isColored` na true,
* jeœli wszystkie liczby zosta³y ju¿ zaznaczone.
*
* @param i Indeks wiersza komórki.
* @param j Indeks kolumny komórki.
*/
	void add_correct_press(int i, int j);
	
/**
* @brief Sprawdza, czy kursor najechany jest na liczbê po lewej stronie planszy i podœwietla odpowiednie pola.
*
* Metoda iteruje po liczbach po lewej stronie planszy. Jeœli kursor znajduje siê nad liczb¹,
* zmienia kursor na hand i podœwietla odpowiednie pola na planszy. W przeciwnym przypadku,
* zmienia kursor na arrow i usuwa podœwietlenie.
*
* @return Zwraca `true`, jeœli kursor najechany jest na liczbê, w przeciwnym razie `false`.
*/
	bool hover_left_num_highlight_tiles();

/**
* @brief Sprawdza, czy kursor najechany jest na liczbê nad plansza i podœwietla odpowiednie pola.
*
* Metoda iteruje po liczbach nad plansza. Jeœli kursor znajduje siê nad liczb¹,
* zmienia kursor na hand i podœwietla odpowiednie pola na planszy. W przeciwnym przypadku,
* zmienia kursor na arrow i usuwa podœwietlenie.
*
* @return Zwraca `true`, jeœli kursor najechany jest na liczbê, w przeciwnym razie `false`.
*/
	bool hover_up_num_highlight_tiles();

/**
 * @brief Podswietla polaczone "correct" komorki w planszy zaczynajac od konkretnego miejsca.
 *
 * Metoda implementuje flood fill algorytm, zeby rekurencyjnie sprawdzac sasiadujace komorki.
 * Jesli sasiednia komorka jest rowniez "correct" (ma 'CORRECT' status w komorce), jest podswietlana poprzez
 * ustawienie komorki na status "LIGHTEN" oraz tekstura jest odpowiednio zmieniana.
 *
 * @param x Wspolrzedna x startowa.
 * @param y Wspolrzedna y startowa.
 */
	void highlight_connected_correct_tiles(int x, int y);

/**
* @brief Znajduje koordynaty x i y "correct" komorki, jesli znajduje sie nad nia myszka.
*
* Metoda przechodzi przez plansze, sprawdzajac kazda "correct" komorke czy jest nad nia myszka.
* Jesli ta komorka jest znalezione, wspolrzedne x i y sa zapisywane w strukturze `coords`.
*
* @return Zwraca `true` jesli taka komorka zostala znalezione, w przeciwnym wypadku `false`.
*/
	bool find_cell_coords_under_cursor();

/**
* @brief Obs³uguje klikniêcie na planszy nanogramu.
*
* Metoda przetwarza klikniêcia na poszczególne komórki planszy, aktualizuj¹c ich stan.
Obs³uguje korzystanie z wskazówek, lewy i prawy przycisk myszy oraz punktacjê.
*
* @param x Wspó³rzêdna x klikniêcia.
* @param y Wspó³rzêdna y klikniêcia.
* @param button Przycisk myszy, który zosta³ naciœniêty (LEFT lub RIGHT).
* @return Zwraca `true`, jeœli klikniêcie zosta³o obs³u¿one, w przeciwnym razie `false`.
*/
	bool click(int x, int y, int button);

/**
* @brief Przechodzi przez tablice z numerami i sprawdza, czy wszystkie sa pokolorowane.
* 
*@return Zwraca `true`, jesli wszystkie sa pokolorowane, w przeciwnym razie `false`.
*/
	bool win();

/**
* @brief Wyœwietla okno koñcowe gry.
*
* Metoda rysuje prostok¹t zaokr¹glony oraz przycisk "Nowa gra". Jeœli kursor znajduje siê nad przyciskiem,
* zmienia jego kolor i ustawia kursor na "rêkê".
*/
	void display_end_window();

/**
 * @brief Wyœwietla planszê nanogramu na ekranie
 *
 * Funkcja odpowiedzialna za wyœwietlanie planszy nanogramu. Rysuje linie poziome
 * i pionowe na planszy, ustawia pozycje i kolor tekstu dla liczb w lewej kolumnie
 * i górnej kolumnie oraz rysuje g³ówn¹ planszê z kwadratami.
 */
	void display_board();

/**
* @brief Wyœwietla g³ówn¹ grê nanogramu na ekranie
*
* Funkcja odpowiedzialna za wyœwietlanie g³ównej gry nanogramu. Najpierw wyœwietla planszê,
* nastêpnie sprawdza, czy gra jest przegrana (jeœli liczba ¿yæ jest równa 0). Jeœli tak,
* ustawia stan gry na zakoñczony i wyœwietla okno koñcowe. Nastêpnie rysuje ikony serc
* symbolizuj¹ce pozosta³e ¿ycia gracza. Jeœli gra wci¹¿ trwa, rysuje przycisk wskazówki
* zale¿nie od tego, czy jest u¿ywana.
*/
	void display_game();

/**
* @brief Handles mouse pressed events :).
*/
	void mouse_pressed_events(sf::Event& event);

/**
* @brief Handles mouse moved events :).
*/
	void mouse_moved_events(sf::Event& event);

/**
 * @brief Obs³uguje zdarzenia okna gry nanogramu
 *
 * Funkcja odpowiedzialna za obs³ugê zdarzeñ okna gry, takich jak zamkniêcie okna,
 * naciœniêcie klawiszy, klikniêcia myszk¹ i ruchy myszk¹. Zdarzenia te kontroluj¹
 * logikê gry, w tym rysowanie, wskazówki, przeci¹ganie i zaznaczanie komórek.
 */
	void handle_event(sf::Event& ev) override;

/**
* @brief Zapisuje polozenie myszki w oknie oraz obs³uguje zdarzenia okna gry nanogramu.
*/
	void render() override;

/**
* @brief Wyswietla caly ekran.
*/
	void update() override;
};
