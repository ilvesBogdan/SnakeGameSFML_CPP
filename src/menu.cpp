#include "menu.h"

void Menu::render(sf::RenderWindow* window)
{
	short winX = window->getSize().x;
	short winY = window->getSize().y;
	sf::Text* text;
	sf::FloatRect pt; // Позиция текста
	sf::CircleShape shape(winX / 5);
	sf::Vector2i mouse = sf::Mouse::getPosition(*window);

	for (unsigned short i = 0; i < menuItems.size(); i++)
	{
		text = &menuItems[i].title;
		text->setCharacterSize((winX > winY ? winY : winX) * 0.06);
		text->setPosition(winX / 2, (winY / 2) - ((winX > winY ? winY : winX) * 0.08 * i + 4));
		pt = text->getGlobalBounds();
		if ((mouse.x > pt.left)
			&& (mouse.x < (pt.left + pt.width))
			&& (mouse.y > pt.top)
			&& (mouse.y < (pt.top + pt.height)))
		{
			backgroundTextBox(window, &pt, (winX > winY ? winY : winX) * 0.006);
			text->setFillColor(sf::Color::Black);
			menuItems[i].focus = true;
		}
		else
		{
			text->setFillColor(sf::Color::White);
			menuItems[i].focus = false;
		}

		// Отрисовка логотипа python
		sf::Texture shapeTexture;
		shape.setFillColor(sf::Color::White);
		shapeTexture.loadFromFile("content/python.png");
		shape.setTexture(&shapeTexture);
		shape.setPosition(winX * 0.06, (winY - shape.getLocalBounds().height) / 2 - winY * 0.06);

		window->draw(shape);
		window->draw(menuItems[i].title);
	}
}

Menu::Menu(short mode)
{
	if (!defaultFont.loadFromFile("content/Vetka.otf"))
		throw("Font not fond.");

	switch (mode)
	{
		case 0:
			// Главное меню
			menuItems = {
				item { sf::Text(L"Одиночная игра", defaultFont), false },
				item { sf::Text(L"Cетевая игра", defaultFont), false },
				item { sf::Text(L"Выход", defaultFont), false }
			};
			break;
		case 1:
			// Меню подключения
			menuItems = {
				item { sf::Text(L"Создать игру", defaultFont), false },
				item { sf::Text(L"Подключиться", defaultFont), false },
				item { sf::Text(L"Назад", defaultFont), false }
			};
			break;

		default:
			throw("Mode err.");
	}
	std::reverse(menuItems.begin(), menuItems.end());
}

short Menu::getFocused()
{
	if (menuItems.size())
		for (unsigned short i = 0; i < menuItems.size(); i++)
		{
			if (menuItems[i].focus)
				return i;
		}
	return -1;
}

void Menu::backgroundTextBox(sf::RenderWindow* window, sf::FloatRect* pt, float p)
{
	sf::RectangleShape box;
	box.setSize(sf::Vector2f(pt->width * 1.32, pt->height * 1.2));
	box.setPosition(pt->left - p, pt->top - p);
	box.setFillColor(sf::Color::White);
	window->draw(box);
}