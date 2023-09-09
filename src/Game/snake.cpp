#include "Game/snake.h"
#include "Game/game.h"

game::Snake::Snake(sf::RenderWindow* w, int seed)
{
	colorBody = randColor(seed + 1);
	colorHead = randColor(seed);
	screen = w;
}

sf::Color game::Snake::randColor(int seed)
{
	switch (abs(rand.getRandomInt(10 * seed + seed, 100 * seed) % 10 - 5))
	{
		case 0:
			return sf::Color::White;
		case 1:
			return sf::Color::Magenta;
		case 2:
			return sf::Color::Green;
		case 3:
			return sf::Color::Yellow;
		case 4:
			return sf::Color::Cyan;
		default:
			return randColor(seed);
	}
}

void game::Snake::clear(const sf::Vector2f* result_position)
{
	snake_direction_list.clear();
	body.clear();

	// Скорость перемещения змейки.
	movementScale = 0.5;
	// Текущая длина змейки.
	snake_length = 1;

	body.push_back(getRectangleAt(*result_position, colorHead));
	snake_direction_list.push_front(sf::Vector2<int>(-1, 0));
	updateLegth = false;
}

bool game::Snake::isPositionTop()
{
	return (screen->getSize().y / 2) > body[0].getPosition().y;
}

void game::Snake::drawSnake()
{
	for (int i = 1; i < snake_length; ++i)
	{
		screen->draw(body[i]);
	}
	screen->draw(body[0]);
}

bool game::Snake::died()
{
	for (int i = BOX_SIZE / (movementScale / 10); i < snake_length; ++i)
	{
		if (checkCollision(body[0], body[i]))
			return true;
	}
	// Проверка столкновения
	int x = body[0].getGlobalBounds().left, y = body[0].getGlobalBounds().top;
	int mx = screen->getSize().x, my = screen->getSize().y;
	return (x > mx || x < 0) || (y > my || y < 0);
}

bool game::Snake::ateFood(Food* food)
{
	if (updateLegth)
	{
		snake_length++;
		updateLegth = false;
	}
	if (checkCollision(body[0], food->getFood()))
	{
		updateLegth = true;
		sf::Vector2f new_location = body[snake_length - 1].getPosition();
		body.push_back(getRectangleAt(new_location, colorBody));
		snake_direction_list.push_back(lastDirection);
		return true;
	}
	return false;
}

void game::Snake::moveSnake(sf::Vector2<int> direction)
{
	snake_direction_list.push_front(direction);
	lastDirection = snake_direction_list.back();
	snake_direction_list.pop_back();
	std::list<sf::Vector2<int>>::iterator i = snake_direction_list.begin();
	int index = 0;
	while (i != snake_direction_list.end() && index < snake_length)
	{
		body[index].move(movementScale * (*i).x, movementScale * (*i).y);
		index++;
		i++;
	}
}

sf::Vector2f game::Snake::getNextFoodLocation()
{
	sf::Vector2f food_loc;
	// Цикл через goto )
	// Он служит для того что предатвратить генерацию еды в теле змеи
start_proc:
	food_loc = [](sf::Vector2u srnSize, Random* rand) {
		return sf::Vector2f(
			rand->getRandomInt(0, srnSize.x - 4 * BOX_SIZE),
			rand->getRandomInt(0, srnSize.y - 4 * BOX_SIZE));
	}(screen->getSize(), &rand);
	if (!isFoodtoSnake(&food_loc))
		goto start_proc;
	return food_loc;
}

sf::Vector2f game::Snake::getNextFoodLocation(game::Snake* snake2)
{
	sf::Vector2f food_loc;
start_proc:
	food_loc = [](sf::Vector2u srnSize, Random* rand) {
		return sf::Vector2f(
			rand->getRandomInt(0, srnSize.x - 4 * BOX_SIZE),
			rand->getRandomInt(0, srnSize.y - 4 * BOX_SIZE));
	}(screen->getSize(), &rand);
	if (!(isFoodtoSnake(&food_loc) && snake2->isFoodtoSnake(&food_loc)))
		goto start_proc;
	return food_loc;
}

bool game::Snake::isFoodtoSnake(sf::Vector2f* food_loc)
{
	for (auto be = body.begin(); body.end() != be; be++)
		if (be->getGlobalBounds().contains(*food_loc))
			return false;
	return true;
}