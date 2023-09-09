#include "Game/game.h"
#include "Platform/Platform.hpp"
#include <string>

#include "game/food.h"
#include "menu.h"
#include "network.h"

bool connector(Menu* netWorkMenu, net::Synch* NetGame, game::GameController* player);

int main()
{
	util::Platform platform;
	sf::Event event;
	bool* gameContinue;
	bool* gameContinueNetWork;
	bool NetWork;
	bool ThisIsMainMenu;

	Menu mainMenu(0);
	Menu netWorkMenu(1);
	sf::RenderWindow window;
	net::Synch NetGame;
	game::Food* food;
	food = NULL;

#if defined(_DEBUG)
	std::cout << "Тестовая сборка" << std::endl;
#endif

	// Создание окна программы
	window.create([](float screenScalingFactor) {
		// в Windows getScreenScalingFactor должен быть вызван перд созданием окна
		return sf::VideoMode(
			screenScalingFactor * 420, // ширина окна
			screenScalingFactor * 420  // высота окна
		);
	}(platform.getScreenScalingFactor(window.getSystemHandle())),
		"Snake c++ by Bogdan" // Заголовок окна
	);
	platform.setIcon(window.getSystemHandle());
	window.setVerticalSyncEnabled(true);

	// Создание персонажей
	game::GameController* protagonist;
	game::GameController* antagonist;
	protagonist = NULL;
	antagonist = NULL;

	// присваеваем значения
	gameContinue = (bool*)malloc(1);
	*gameContinue = false;
	gameContinueNetWork = (bool*)malloc(1);
	*gameContinueNetWork = false;
	NetWork = false;
	ThisIsMainMenu = true;

	// Основной цикл программы
	while (window.isOpen())
	{
		/////////////////  Действия  /////////////////
		if (NetWork)
		{
			if (*gameContinue && *gameContinueNetWork)
			{
				NetGame.run(antagonist);
				window.clear();
				if (protagonist->loopFunc(food) || antagonist->loopFunc(food))
				{
					delete food;
					food = [](sf::RenderWindow* window, net::Synch* net, sf::Vector2f loc) {
						float* arr = (float*)malloc(8);
						char* post = (char*)malloc(1);
						net->setBlocking(true);
						if (net->thisServer)
						{
						sendFood:
							*(arr + 0) = loc.x;
							*(arr + 1) = loc.y;
							net->send(arr, 8);
							net->get(post, 1);
							if (*post == 'e')
								goto sendFood;
						}
						else
						{
						getFood:
							if (net->get(arr, 8) == 8)
							{
								*post = 'o';
								loc.x = *(arr + 0);
								loc.y = *(arr + 1);
								net->send(post, 1);
							}
							else
							{
								*post = 'e';
								net->send(post, 1);
								goto getFood;
							}
						}
						delete arr;
						delete post;
						net->setBlocking(false);
						return new game::Food(window, loc);
					}(&window, &NetGame, protagonist->snake.getNextFoodLocation());
				}
				if (protagonist != NULL)
					protagonist->snake.drawSnake();
				if (antagonist != NULL)
					antagonist->snake.drawSnake();
				food->drawFood();
			}
			else
			{
				*gameContinue = false;
				*gameContinueNetWork = false;
				NetWork = false;
			}
		}
		else if (*gameContinue)
		{
			window.clear();
			if (protagonist->loopFunc(food))
			{
				delete food;
				food = new game::Food(&window, protagonist->snake.getNextFoodLocation());
			}
			protagonist->snake.drawSnake();
			food->drawFood();
		}
		else
		{
			window.clear();
			if (ThisIsMainMenu)
				// Отрисовка главного меню
				mainMenu.render(&window);
			else
				netWorkMenu.render(&window);
		}

		/////////////////  События  /////////////////
		while (window.pollEvent(event))
		{
			switch (event.type)
			{

				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
						case sf::Keyboard::Up:
						case sf::Keyboard::W:
							protagonist->moveUp();
							break;
						case sf::Keyboard::Down:
						case sf::Keyboard::S:
							protagonist->moveDown();
							break;
						case sf::Keyboard::Left:
						case sf::Keyboard::A:
							protagonist->moveLeft();
							break;
						case sf::Keyboard::Right:
						case sf::Keyboard::D:
							protagonist->moveRight();
							break;
						case sf::Keyboard::Escape:
							*gameContinue = false;
							break;

						default: break;
					}
					continue;
					break;

				case sf::Event::MouseButtonReleased:
					if ((event.mouseButton.button == sf::Mouse::Left) && (!*gameContinue))
					{
						if (ThisIsMainMenu)
							switch (mainMenu.getFocused())
							{
								case 2:
									// Одиночная игра
									protagonist = new game::GameController(&window, 1);
									gameContinue = protagonist->start();
									food = new game::Food(&window, protagonist->snake.getNextFoodLocation());
									break;
								case 1:
									// Сетевая игра
									protagonist = new game::GameController(&window, 3);
									antagonist = new game::GameController(&window, 17);
									ThisIsMainMenu = false;
									food = NULL;
									break;
								case 0:
									window.close();
									break;

								default:
									break;
							}
						else
						{
							if (connector(&netWorkMenu, &NetGame, protagonist))
							{
								NetGame.setBlocking(true);
								if (NetGame.thisServer)
								{

									//////////////////////////////////////////////////////////////////
									///////////////////////////// СЕРВЕР /////////////////////////////
									//////////////////////////////////////////////////////////////////

									[](game::GameController* p1, game::GameController* p2, net::Synch* net) {
										// Отправка цветов на клиент
										sf::Color colors[4] {
											p2->snake.colorHead,
											p2->snake.colorBody,
											p1->snake.colorHead,
											p1->snake.colorBody
										};
										net->send(&colors, 16);
									}(protagonist, antagonist, &NetGame);
									[](game::GameController* p1, game::GameController* p2, net::Synch* net, bool* gc, bool* gn) {
										// Отправка положений змей на клиент
										sf::Vector2f position1;
										sf::Vector2f position2;
										unsigned short* responseArray;
										responseArray = (unsigned short*)malloc(8);
										p1->start(&position1, net->thisServer, gc, 3);
										p2->start(&position2, net->thisServer, gn, 8);
										responseArray = new unsigned short[4] {
											(unsigned short)position1.x,
											(unsigned short)position1.y,
											(unsigned short)position2.x,
											(unsigned short)position2.y
										};
										net->send(responseArray, 8);
										delete responseArray;
									}(protagonist, antagonist, &NetGame, gameContinue, gameContinueNetWork);
								}
								else
								{

									//////////////////////////////////////////////////////////////////
									///////////////////////////// КЛИЕНТ /////////////////////////////
									//////////////////////////////////////////////////////////////////

									[](game::GameController* p1, game::GameController* p2, net::Synch* net) {
										// Получение цветов
										std::size_t response;
										sf::Color* responseColors = (sf::Color*)malloc(16);
										response = net->get(responseColors, 16);
										if (response == 16)
										{
											p1->snake.colorHead = *(responseColors + 0);
											p1->snake.colorBody = *(responseColors + 1);
											p2->snake.colorHead = *(responseColors + 2);
											p2->snake.colorBody = *(responseColors + 3);
										}
										else
										{
#if defined(_DEBUG)
											std::cout << "Ошибка в получении цветов." << std::endl;
#endif
											throw;
										}
										delete responseColors;
									}(protagonist, antagonist, &NetGame);
									[](game::GameController* p1, game::GameController* p2, net::Synch* net, bool* gc, bool* gn) {
										// Получение положений змей с сервера
										std::size_t response;
										unsigned short* responseArray;
										responseArray = (unsigned short*)malloc(8);
										response = net->get(responseArray, 8);
										sf::Vector2f position1((float)(*(responseArray + 0)), (float)(*(responseArray + 1)));
										sf::Vector2f position2((float)(*(responseArray + 2)), (float)(*(responseArray + 3)));
										delete responseArray;
										if (response == 8)
										{
											p1->start(&position2, net->thisServer, gc, 0);
											p2->start(&position1, net->thisServer, gn, 0);
										}
										else
										{
#if defined(_DEBUG)
											std::cout << "Ошибка в получении начальных положений." << std::endl;
#endif
											throw;
										}
									}(protagonist, antagonist, &NetGame, gameContinue, gameContinueNetWork);
								}
								NetWork = true;
								NetGame.setBlocking(false);
							}
							ThisIsMainMenu = true;
						}
					}
					break;

				case sf::Event::Resized:
					window.setView([](sf::Vector2u ws) {
						return sf::View(sf::Vector2f(ws.x / 2, ws.y / 2), sf::Vector2f(ws.x, ws.y));
					}(window.getSize()));
					break;

				case sf::Event::Closed:
					window.close();
					break;

				default: continue;
			}
		}

		/////////////////  Отрисовка игры  /////////////////
		window.display();
		window.setFramerateLimit(60);
	}

	return EXIT_SUCCESS;
}

bool connector(Menu* netWorkMenu, net::Synch* NetGame, game::GameController* player)
{
	// Кнопка со значением 2 это сервер, а со значенем 1 клиент
	unsigned short button = netWorkMenu->getFocused();
	if (button < 1)
		return false;

	std::ifstream file;
	std::string ip;
	unsigned short port;

	file.open("ip.txt");
	if (file)
	{
		std::string textIP;
		getline(file, textIP);
		file.close();
		std::string textPort = "";
		ip = "";
		int cc;
		bool thisPort = false;
		for (unsigned short i = 0; i < textIP.size(); i++)
		{
			cc = (int)textIP[i];
			if ((cc < 45) || (cc > 122))
				continue;
			if (thisPort && (cc > 47) && (cc < 58))
			{
				textPort += textIP[i];
				continue;
			}
			if (cc == 58)
			{
				thisPort = true;
				continue;
			}
			ip += textIP[i];
		}
		port = atoi(textPort.c_str());
	}
	else
	{
		file.close();
		std::ofstream oFile("ip.txt");
		oFile << "localhost:2000";
		ip = "localhost";
		port = 2000;
		oFile.close();
	}

#if defined(_DEBUG)
	std::cout << "Получен адрес подключения: " << ip << ":" << port << std::endl;
#endif

	NetGame->connect(ip, port, button == 2, player);
	return button > 0;
}