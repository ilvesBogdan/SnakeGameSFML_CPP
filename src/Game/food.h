#ifndef SS_HEADER_FOOD_H_
#define SS_HEADER_FOOD_H_

#include "Game/engine.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
namespace game
{
/** @brief Объект пищи.*/
class Food
{
public:
	Food(sf::RenderWindow*, sf::Vector2f loc);
	~Food()
	{
#if defined(_DEBUG)
		std::cout << "Еда скушана" << std::endl;
#endif
	}

	/** @brief Получить текстуру пищи.*/
	sf::RectangleShape getFood();

	/** @brief Отрисовка пищи.*/
	void drawFood();

private:
	sf::Color color;
	sf::Vector2f location;
	sf::RectangleShape food;
	sf::RenderWindow* screen;
};
}

#endif // !SS_HEADER_FOOD_H_