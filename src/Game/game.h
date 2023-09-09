#ifndef SS_GAME_H
#define SS_GAME_H

#include "snake.h"

#include <SFML/Graphics.hpp>
// #include <list>
#include <random>

#define BOX_SIZE 20

namespace game
{
/// \brief GameController - интерфейс игры.
// Этот класс будет следить за:
/// Движением змеи;
/// Проверка на столкновений;
/// Когда воспроизводить звук и т.Д.
class GameController
// Основной класс игры
{
public:
	GameController(sf::RenderWindow* w, int seed);
	Snake snake;

	// Подготавливет loop() к запуску,
	// и позвращет ссылку на bool для пркращения цикла
	void start(sf::Vector2f* start_position, bool isServer, bool* loop, int seed);
	bool* start();

	// Управление змеёй
	/// \brief Движение вверх.
	void moveUp();
	/// \brief Движение вниз.
	void moveDown();
	/// \brief Движение в лево.
	void moveLeft();
	/// \brief Движение в право.
	void moveRight();

	// способы доступа к ресурсам
	/// \brief true - если игра продолжается
	bool loopInvarient;
	/// \brief Возвращает закодированное в символ направление движения змейки.
	char get();

	/** @brief Проверка состояния игры.
	  * @tparam food ссылка на пищу.*/
	bool loopFunc(Food* food);

private:
	void gameOver();	  // нет
	void loadResources(); // нет

	////////////// Данные игры //////////////
	sf::Vector2<int> direction;
	sf::RenderWindow* screen;
	float scale;
	unsigned short score;
	bool created;
};
}

#endif