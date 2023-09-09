#include "network.h"

void net::Synch::connect(std::string ip, unsigned short port, bool thisServer, game::GameController* player)
{
	this->ip = sf::IpAddress(ip);
	this->thisServer = thisServer;
	this->old = '1';

	if (thisServer)
	{
		listener.close();
		listener.listen(port);
		if (listener.accept(socket) != sf::Socket::Done)
		{
#if defined(_DEBUG)
			std::cout << "Ошибка запуска сервера" << std::endl;
#endif
			throw;
		}
#if defined(_DEBUG)
		std::cout << "Удалось произвести запуск сервера." << std::endl;
#endif
	}
	else
	{
		socket.disconnect();
		if (socket.connect(this->ip, port) != sf::Socket::Done)
		{
#if defined(_DEBUG)
			std::cout << "Ошибка при подключениии клиента к серверу." << std::endl;
#endif
			throw;
		}
		socket.setBlocking(false);
#if defined(_DEBUG)
		std::cout << "Удалось произвести подключение." << std::endl;
#endif
	}
	this->player = player;
}

net::Synch::Synch()
{}

net::Synch::~Synch()
{
	if (thisServer)
		listener.close();
	else
		socket.disconnect();
	ip = NULL;
}

void net::Synch::send(char c)
{
	if (socket.send(&c, 1) != sf::Socket::Status::Done)
	{
#if defined(_DEBUG)
		std::cout << (thisServer ? "Сервер" : "Клиент") << "Ошибка отправки символа направления." << std::endl;
#endif
		throw;
	}
}

void net::Synch::send(const void* data, std::size_t size)
{
	if (socket.send(data, size) != sf::Socket::Status::Done)
	{
#if defined(_DEBUG)
		std::cout << (thisServer ? "Сервер" : "Клиент") << ": Ошибка при отправке данных." << std::endl;
#endif
		throw;
	}
}

void net::Synch::get(game::GameController* p2)
{
	char c = 'z';
	std::size_t size = 0;
	sf::Socket::Status status = socket.receive(&c, 1, size);
	if (status == sf::Socket::Status::Done)
	{
		switch (c)
		{
			case 'u':
				p2->moveUp();
				break;
			case 'd':
				p2->moveDown();
				break;
			case 'l':
				p2->moveLeft();
				break;
			case 'r':
				p2->moveRight();
				break;
			default:
				break;
		}
	}
}

std::size_t net::Synch::get(void* data, std::size_t max)
{
	std::size_t received = 0;
	sf::Socket::Status status = socket.receive(data, max, received);
	if (status != sf::Socket::Status::Done)
	{
#if defined(_DEBUG)
		std::cout << (thisServer ? "Сервер" : "Клиент") << ": Ошибка получения данных '"
				  << [](sf::Socket::Status status) {switch (status){
		case sf::Socket::Status::Disconnected:return "disconnected'."; break;
		case sf::Socket::Status::Error:return "error'."; break;
		case sf::Socket::Status::NotReady:return "not ready'."; break;
		case sf::Socket::Status::Partial:return "partial'."; break;
		default: return "иное'."; break;
		}; }(status) << std::endl;
#endif
		throw;
	}
	std::cout << (thisServer ? "Сервер" : "Клиент") << ": Получено " << received << " байт." << std::endl;
	return received;
}

void net::Synch::run(game::GameController* p2)
{
	char c = player->get();
	if (old != c)
	{
		old = c;
		send(c);
	}
	get(p2);
}

void net::Synch::setBlocking(bool value)
{
	socket.setBlocking(value);
}