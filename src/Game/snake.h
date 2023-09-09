#ifndef SS_HEADER_SNAKE_H_
#define SS_HEADER_SNAKE_H_

#include <SFML/Graphics.hpp>
#include <ctime>
#include <list>
#include <random>

#include "food.h"

#define SNAKE_MAX_LENGTH 2000

namespace game
{

class Random
// Класс для генерации случайных чисел
{
	std::random_device rd;
	std::mt19937 gen;

public:
	Random()
	{
		std::srand(std::time(0));
		gen.seed(rd() * std::rand());
	}

	Random(const int seed)
	{
		std::srand(std::time(0));
		gen.seed(rd() * std::rand() * seed);
	}

	int getRandomInt(int l, int u)
	{
		std::uniform_int_distribution<int> dist(l, u);
		return dist(gen);
	}
};

class Snake
// Это змея которой мы управляем.
// Она состоит из std::vector<sf::RectangleShape> objects.
{
public:
	/** @tparam window окно для отрисовки.
	  * @tparam seed для генирации случайных значений.*/
	Snake(sf::RenderWindow* window, int seed);

	/** @brief Отрисовка персонажа змеи.*/
	void drawSnake();

	/** @brief Проверка на смерт персонажа.*/
	bool died();

	/** @brief Обнуление змейки.
	  * @tparam position место спавна*/
	void clear(const sf::Vector2f* position);

	/** @brief Покушала ли змейка.
	  * @tparam food ссылка на пищу*/
	bool ateFood(Food* food);

	/** @brief Находится ли змейка в верхней половине поля.*/
	bool isPositionTop();

	/** @brief Перемещение змейки.
	  * @tparam direction направление перемещения*/
	void moveSnake(sf::Vector2<int> direction);

	/** @brief Генерация будущей локации пищи.
	  * @tparam snake2 ссылка на вторую змейку*/
	sf::Vector2f getNextFoodLocation(game::Snake* snake2);

	/** @brief Проверка на то что бы пища не заспавнилась в теле змейки.
	  * @tparam food_position положение пищи*/
	bool isFoodtoSnake(sf::Vector2f* food_position);

	/** @brief Генерация будущей локации пищи.*/
	sf::Vector2f getNextFoodLocation();

	// Цвет головы
	sf::Color colorBody;
	// Цвет тела
	sf::Color colorHead;

	Random rand;

private:
	sf::RenderWindow* screen;
	sf::Color randColor(int seed);

	// Показывает нужно ли увеличивать длинну змеи
	bool updateLegth;

	// Скорость движения змеи
	float movementScale;

	// Параметры
	int snake_length;
	std::list<sf::Vector2<int>> snake_direction_list;
	sf::Vector2<int> lastDirection;
	std::vector<sf::RectangleShape> body;
};

}

#endif