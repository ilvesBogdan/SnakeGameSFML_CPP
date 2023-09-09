#include "Game/game.h"

#include <iostream>
#include <random>
namespace game
{
GameController::GameController(sf::RenderWindow* w, int seed) :
	snake(w, seed)
{
	screen = w;
	score = 0;
}

void GameController::start(sf::Vector2f* start_position, bool isServer, bool* loop, int seed)
{
	loadResources();
	game::Random rand(seed);
	if (isServer)
		*start_position = sf::Vector2f(
			rand.getRandomInt(screen->getSize().x / 4, screen->getSize().x * 3 / 4),
			rand.getRandomInt(screen->getSize().y / 4, screen->getSize().y * 3 / 4));
	snake.clear(start_position);
	score = 0;

	// Присвоение значений для Loop
	created = true;
	loopInvarient = loop;
	direction = sf::Vector2<int>(0, this->snake.isPositionTop() ? 1 : -1);
	scale = 5;
	*loop = true;
}

bool* GameController::start()
{
	loadResources();
	sf::Vector2f position;
	game::Random rand;
	position = sf::Vector2f(
		rand.getRandomInt(screen->getSize().x / 4, screen->getSize().x * 3 / 4),
		rand.getRandomInt(screen->getSize().y / 4, screen->getSize().y * 3 / 4));
	snake.clear(&position);
	score = 0;

	// Присвоение значений для Loop
	created = true;
	loopInvarient = true;
	direction = sf::Vector2<int>(0, this->snake.isPositionTop() ? 1 : -1);
	scale = 5;

	return &this->loopInvarient;
}

char GameController::get()
{
	if (direction.y != 0)
		return direction.y < 0 ? 'u' : 'd';
	else if (direction.x != 0)
		return direction.x < 0 ? 'l' : 'r';
	return '0';
}

bool GameController::loopFunc(Food* food)
{
	if (food == NULL)
		return true;
	bool result;
	snake.moveSnake(direction);
	if (snake.died())
	{
		// Если игра окончена
		loopInvarient = false;
#if defined(_DEBUG)
		std::cout << "Игра окончена, с счетом в " << score << " очков." << std::endl;
#endif
	}
	result = snake.ateFood(food);
	if (result)
	{
		score++;
#if defined(_DEBUG)
		std::cout << "Увелечение очков до " << score << "." << std::endl;
#endif
	}
	return result;
}

void GameController::moveUp()
{
	if (direction.y == 0)
	{
		direction.y = -1;
		direction.x = 0;
	}
}

void GameController::moveDown()
{
	if (direction.y == 0)
	{
		direction.y = 1;
		direction.x = 0;
	}
}

void GameController::moveLeft()
{
	if (direction.x == 0)
	{
		direction.x = -1;
		direction.y = 0;
	}
}

void GameController::moveRight()
{
	if (direction.x == 0)
	{
		direction.x = 1;
		direction.y = 0;
	}
}

bool checkCollision(const sf::RectangleShape& a, const sf::RectangleShape& b)
{
	return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

sf::RectangleShape getRectangleAt(sf::Vector2f location, sf::Color color)
{
	sf::RectangleShape box;
	box.setSize(sf::Vector2f(BOX_SIZE, BOX_SIZE));
	box.setPosition(location);
	box.setFillColor(color);
	return box;
}
void GameController::loadResources()
{
	//................
}
}