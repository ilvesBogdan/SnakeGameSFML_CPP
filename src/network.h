#include "Game/game.h"
#include "SFML/Network.hpp"

/** @brief Объединяет все для работы с TCP.*/
namespace net
{
/** @brief Осуществляет обмен данными между приложениями*/
class Synch
{
private:
	sf::TcpSocket socket;
	sf::TcpListener listener;
	sf::IpAddress ip;
	game::GameController* player;
	void send(char c);
	char old;

public:
	Synch();
	~Synch();
	bool thisServer;

	/** @brief Подключение к другому приложению.
	  * @tparam ip адрес или домен другого приложения.
	  * @tparam port порт подключения к приложению.
	  * @tparam thisServer является ли текущее приложение сервером.
	  * @tparam player персонаж для передачи его направления другому приложению.*/
	void connect(std::string ip, unsigned short port, bool thisServer, game::GameController* player);

	/** @brief Отправка данных в другое приложение.
	  * @tparam data ссылка на отправляемые данные.
	  * @tparam size размер отправляемых данных в байтах.*/
	void send(const void* data, std::size_t size);

	/** @brief Получение данных из другого приложения.
	  * @tparam data ссылка на выделенную область памяти.
	  * @tparam size максимальный размер получаемых данных в байтах.
	  * @return фактический размер полученных данных в байтах.*/
	std::size_t get(void* data, std::size_t max);

	/** @brief Получает напрявление движения персонажа из другого приложения.
	  * @tparam p2 ссылка на второго персонажа.*/
	void get(game::GameController* p2);

	/** @brief Cинхронизация персонажей.
	  * @tparam p2 ссылка на второго персонажа.*/
	void run(game::GameController* p2);

	/** @brief Ожидать получения данных get.
	  * @tparam value булевое значение.*/
	void setBlocking(bool value);
};
}