#include "Platform/Platform.hpp"
#include <vector>

/** @brief Текстовое меню приложения.
 * Конструктор принимает целое число,
 * его возможные значения:
 * @tparam 1 Главное меню приложения
 * @tparam 2 Меню сетевого подключения*/
class Menu
{
private:
	struct item
	{
		sf::Text title;
		bool focus;
	};
	sf::Font defaultFont;
	std::vector<item> menuItems;
	void backgroundTextBox(sf::RenderWindow* window, sf::FloatRect* pt, float p);

public:
	/** @tparam 1 Главное меню приложения
	  * @tparam 2 Меню сетевого подключения*/
	Menu(short mode);

	/** @brief Отрисовывает меню.
	  * @tparam window Окно в котором будет происходить отрисовка.*/
	void render(sf::RenderWindow* window);

	/** @brief Возвращает номер элемента на который наведен курсор.*/
	short getFocused();
};