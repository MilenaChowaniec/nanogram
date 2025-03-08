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
 * @brief Rozpoczyna now� gr�.
 *
 * Metoda resetuje plansz�, liczby po lewej i g�rnej stronie oraz inicjalizuje zmienne potrzebne do gry.
 */
	void clear_game();

/**
 * @brief Tworzy gre.
 */
	void create_game();

/**
 * @brief Wype�nia plansz� gry kom�rkami.
 *
 * Funkcja tworzy dwuwymiarow� tablic� kom�rek o wymiarach `size x size`.
 * Ka�da kom�rka jest inicjalizowana losow� warto�ci� `under` oraz pustym polem `tile`.
 * Pozycja i rozmiar graficznej reprezentacji kom�rki s� ustawiane na podstawie ustalonych parametr�w.
 *
 */
	void fill_board();

/**
 * @brief Oblicza i zapisuje liczby do wy�wietlenia na lewej i g�rnej kraw�dzi planszy.
 *
 * Funkcja przechodzi przez ka�dy wiersz i kolumn� planszy, zliczaj�c ci�gi s�siaduj�cych kom�rek o warto�ci `TAKEN`.
 * Dla ka�dego ci�gu tworzy obiekt `Number` reprezentuj�cy liczb� do wy�wietlenia i dodaje go do odpowiedniego wektora.
 *
 * @param board_ Dwuwymiowa tablica reprezentuj�ca plansz� gry.
 * @param left Struktura przechowuj�ca informacje o liczbach do wy�wietlenia na lewej kraw�dzi.
 * @param up Struktura przechowuj�ca informacje o liczbach do wy�wietlenia na g�rnej kraw�dzi.
 */
	void fill_left_up();

/**
 * @brief Blokuje ca�y wiersz lub kolumn�, je�li wszystkie liczby na odpowiedniej kraw�dzi s� "zaznaczone".
 *
 * Funkcja sprawdza, czy wszystkie liczby na lewej kraw�dzi dla danego wiersza lub wszystkie liczby na g�rnej kraw�dzi dla danej kolumny s� "zaznaczone" (maj� ustawione `isColored` na `true`). Je�li tak, blokuje wszystkie puste kom�rki w tym wierszu lub kolumnie.
 *
 * @param row Numer wiersza do sprawdzenia.
 * @param column Numer kolumny do sprawdzenia.
 */
	void block_if_numbers_match(int row, int column);

/**
* @brief Aktualizuje stan kom�rek w planszy po poprawnym zaznaczeniu liczby.
*
* Ta metoda s�u�y do zaktualizowania stanu kom�rek w planszy gry
* po tym, jak u�ytkownik zaznaczy� poprawn� liczb�. Zwi�ksza licznik
* ju� zaznaczonych liczb dla danej kom�rki i ustawia flag� `isColored` na true,
* je�li wszystkie liczby zosta�y ju� zaznaczone.
*
* @param i Indeks wiersza kom�rki.
* @param j Indeks kolumny kom�rki.
*/
	void add_correct_press(int i, int j);
	
/**
* @brief Sprawdza, czy kursor najechany jest na liczb� po lewej stronie planszy i pod�wietla odpowiednie pola.
*
* Metoda iteruje po liczbach po lewej stronie planszy. Je�li kursor znajduje si� nad liczb�,
* zmienia kursor na hand i pod�wietla odpowiednie pola na planszy. W przeciwnym przypadku,
* zmienia kursor na arrow i usuwa pod�wietlenie.
*
* @return Zwraca `true`, je�li kursor najechany jest na liczb�, w przeciwnym razie `false`.
*/
	bool hover_left_num_highlight_tiles();

/**
* @brief Sprawdza, czy kursor najechany jest na liczb� nad plansza i pod�wietla odpowiednie pola.
*
* Metoda iteruje po liczbach nad plansza. Je�li kursor znajduje si� nad liczb�,
* zmienia kursor na hand i pod�wietla odpowiednie pola na planszy. W przeciwnym przypadku,
* zmienia kursor na arrow i usuwa pod�wietlenie.
*
* @return Zwraca `true`, je�li kursor najechany jest na liczb�, w przeciwnym razie `false`.
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
* @brief Obs�uguje klikni�cie na planszy nanogramu.
*
* Metoda przetwarza klikni�cia na poszczeg�lne kom�rki planszy, aktualizuj�c ich stan.
Obs�uguje korzystanie z wskaz�wek, lewy i prawy przycisk myszy oraz punktacj�.
*
* @param x Wsp�rz�dna x klikni�cia.
* @param y Wsp�rz�dna y klikni�cia.
* @param button Przycisk myszy, kt�ry zosta� naci�ni�ty (LEFT lub RIGHT).
* @return Zwraca `true`, je�li klikni�cie zosta�o obs�u�one, w przeciwnym razie `false`.
*/
	bool click(int x, int y, int button);

/**
* @brief Przechodzi przez tablice z numerami i sprawdza, czy wszystkie sa pokolorowane.
* 
*@return Zwraca `true`, jesli wszystkie sa pokolorowane, w przeciwnym razie `false`.
*/
	bool win();

/**
* @brief Wy�wietla okno ko�cowe gry.
*
* Metoda rysuje prostok�t zaokr�glony oraz przycisk "Nowa gra". Je�li kursor znajduje si� nad przyciskiem,
* zmienia jego kolor i ustawia kursor na "r�k�".
*/
	void display_end_window();

/**
 * @brief Wy�wietla plansz� nanogramu na ekranie
 *
 * Funkcja odpowiedzialna za wy�wietlanie planszy nanogramu. Rysuje linie poziome
 * i pionowe na planszy, ustawia pozycje i kolor tekstu dla liczb w lewej kolumnie
 * i g�rnej kolumnie oraz rysuje g��wn� plansz� z kwadratami.
 */
	void display_board();

/**
* @brief Wy�wietla g��wn� gr� nanogramu na ekranie
*
* Funkcja odpowiedzialna za wy�wietlanie g��wnej gry nanogramu. Najpierw wy�wietla plansz�,
* nast�pnie sprawdza, czy gra jest przegrana (je�li liczba �y� jest r�wna 0). Je�li tak,
* ustawia stan gry na zako�czony i wy�wietla okno ko�cowe. Nast�pnie rysuje ikony serc
* symbolizuj�ce pozosta�e �ycia gracza. Je�li gra wci�� trwa, rysuje przycisk wskaz�wki
* zale�nie od tego, czy jest u�ywana.
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
 * @brief Obs�uguje zdarzenia okna gry nanogramu
 *
 * Funkcja odpowiedzialna za obs�ug� zdarze� okna gry, takich jak zamkni�cie okna,
 * naci�ni�cie klawiszy, klikni�cia myszk� i ruchy myszk�. Zdarzenia te kontroluj�
 * logik� gry, w tym rysowanie, wskaz�wki, przeci�ganie i zaznaczanie kom�rek.
 */
	void handle_event(sf::Event& ev) override;

/**
* @brief Zapisuje polozenie myszki w oknie oraz obs�uguje zdarzenia okna gry nanogramu.
*/
	void render() override;

/**
* @brief Wyswietla caly ekran.
*/
	void update() override;
};
