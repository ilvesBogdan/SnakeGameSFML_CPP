#ifndef SS_HEADER_ENGINE_H_
#define SS_HEADER_ENGINE_H_

#include <SFML/Graphics.hpp>

/** @brief Содержит функции, которые будут использоваться
 * для выполнения определенных действий, которые будут
 * необходимы в игре.
*/
namespace game
{
/** @brief Пересекаются ли объекты между собой.*/
bool checkCollision(const sf::RectangleShape&, const sf::RectangleShape&);

/** @brief Возвращает прямоугольник в указанных координатах.
 ** @tparam location Где должен находиться прямоугольник.
 ** @tparam color цвет прямоугольника.
 ** @return объект прямоугольника.
*/
sf::RectangleShape getRectangleAt(sf::Vector2f location, sf::Color);
}
#endif