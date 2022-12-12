#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

float offsetX = 0, offsetY = 0;

const int H = 17;
const int W = 83;

String TileMap[H] = {
	"P000000000000000000000000000000000000000000000000000000000000000000000000y20220000P",
	"P                  mhg                                                            P",
	"P                  laf                                                            P",
	"P                  jbe                                 w                   w      P",
	"P      w           kkkkkkkkkk                                                     P",
	"P                              kkkkkkkkkkkkkk                kkkkk                P",
	"P                                             kkk                                 P",
	"P         w                                          kkk                r         P",
	"P                                                                      rr         P",
	"P                                                                     rrr         P",
	"P               c    kckck                                           rrrr         P",
	"P                                      t0                           rrrrrr        P",
	"PG                                     00              t0          rrrrrrrrr      P",
	"P           d    g                     00              00         rrrrrrrrrr      P",
	"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
	"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
	"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
};

class PLAYER
{

public:
	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;
	bool win;

	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		rect = FloatRect(100, 180, 16, 16);

		dx = dy = 0.1;
		currentFrame = 0;
		win = false;
	}

	void update(float time)
	{

		rect.left += dx * time;
		Collision(0);

		if (!onGround)
			dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);

		currentFrame += time * 0.005;
		if (currentFrame > 3)
			currentFrame -= 3;

		if (dx > 0)
			sprite.setTextureRect(IntRect(112 + 31 * int(currentFrame), 144, 16, 16));
		if (dx < 0)
			sprite.setTextureRect(IntRect(112 + 31 * int(currentFrame) + 16, 144, -16, 16));

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}

	void Collision(int num)
	{

		for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++)
			for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++)
			{
				if ((TileMap[i][j] == 'P') || (TileMap[i][j] == 'k') || (TileMap[i][j] == '0') || (TileMap[i][j] == 'r') || (TileMap[i][j] == 't') || (TileMap[i][j] == 'g') || (TileMap[i][j] == 'c'))
				{
					if (dy > 0 && num == 1)
					{
						rect.top = i * 16 - rect.height;
						dy = 0;
						onGround = true;
					}
					if (dy < 0 && num == 1)
					{
						rect.top = i * 16 + 16;
						dy = 0;
					}
					if (dx > 0 && num == 0)
					{
						rect.left = j * 16 - rect.width;
					}
					if (dx < 0 && num == 0)
					{
						rect.left = j * 16 + 16;
					}
				}
				if ((TileMap[i][j] == 'a') || (TileMap[i][j] == 'b'))
					win = true;
			}
	}
};

class ENEMY
{

public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float currentFrame;
	bool life;

	void set(Texture& image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x, y, 16, 16);

		dx = 0.05;
		currentFrame = 0;
		life = true;
	}

	void update(float time)
	{
		rect.left += dx * time;

		Collision();

		currentFrame += time * 0.005;
		if (currentFrame > 2)
			currentFrame -= 2;

		sprite.setTextureRect(IntRect(18 * int(currentFrame), 0, 16, 16));
		if (!life)
			sprite.setTextureRect(IntRect(58, 0, 16, 16));

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}

	void Collision()
	{

		for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++)
			for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++)
				if ((TileMap[i][j] == 'P') || (TileMap[i][j] == '0')) //разворот при встрече стенки
				{
					if (dx > 0)
					{
						rect.left = j * 16 - rect.width;
						dx *= -1;
					}
					else if (dx < 0)
					{
						rect.left = j * 16 + 16;
						dx *= -1;
					}
				}
	}
};

int main()
{

	RenderWindow window(VideoMode(400, 250), "Mario!");

	Texture tileSet;
	tileSet.loadFromFile("C:/Programming/C++/University/sfml-vscode-boilerplate-master/src/Mario_tileset.png");
	Texture tilepriv;
	tilepriv.loadFromFile("C:/Programming/C++/University/sfml-vscode-boilerplate-master/src/group.png");

	PLAYER Mario(tileSet);
	ENEMY enemy;
	enemy.set(tileSet, 48 * 16, 13 * 16); //установка начальной позиции

	Sprite tile(tileSet);
	Sprite tile_p(tilepriv);

	Clock clock;

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 500; // замедление частоты кадров

		if (time > 20)
			time = 20;

		Event event;
		//логика команд
		while (window.pollEvent(event) || Mario.win)
		{
			if (event.type == Event::Closed || Mario.win)
			{
				window.close();
				exit(0);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
			Mario.dx = -0.1;

		if (Keyboard::isKeyPressed(Keyboard::Right))
			Mario.dx = 0.1;
		if (Keyboard::isKeyPressed(Keyboard::F))
		{
			Mario.rect.left = 27 * 16;
			Mario.rect.top = 4 * 16;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			Mario.dx *= 2;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
			if (Mario.onGround)
			{
				Mario.dy = -0.27;
				Mario.onGround = false;
			}

		//обновление состояний
		Mario.update(time);
		enemy.update(time);

		if (Mario.rect.intersects(enemy.rect)) //встреча с врагом
		{
			if (enemy.life)
			{
				if (Mario.dy > 0)
				{
					enemy.dx = 0;
					Mario.dy = -0.2;
					enemy.life = false;
				}
				else
				{
					Mario.sprite.setColor(Color::Red);
					Mario.win = true;
				}
			}
		}

		if (Mario.rect.left > 200) //остановка в конце карты
			offsetX = Mario.rect.left - 200;

		window.clear(Color(107, 140, 255)); //очистка экрана

		//отображение тайлов
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
			{
				if (TileMap[i][j] == 'P')
					tile.setTextureRect(IntRect(143 - 16 * 3, 112, 16, 16));

				if (TileMap[i][j] == 'k')
					tile.setTextureRect(IntRect(143, 112, 16, 16));

				if (TileMap[i][j] == 'c')
					tile.setTextureRect(IntRect(143 - 16, 112, 16, 16));

				if (TileMap[i][j] == 't')
					tile.setTextureRect(IntRect(0, 47, 32, 95 - 47));

				if (TileMap[i][j] == 'g')
					tile.setTextureRect(IntRect(0, 16 * 9 - 5, 3 * 16, 16 * 2 + 5));

				if (TileMap[i][j] == 'G')
					tile.setTextureRect(IntRect(145, 222, 222 - 145, 255 - 222));

				if (TileMap[i][j] == 'd')
					tile.setTextureRect(IntRect(0, 106, 74, 127 - 106));

				if (TileMap[i][j] == 'w')
					tile.setTextureRect(IntRect(99, 224, 140 - 99, 255 - 224));

				if (TileMap[i][j] == 'r')
					tile.setTextureRect(IntRect(143 - 32, 112, 16, 16));
				if (TileMap[i][j] == 'a')
					tile.setTextureRect(IntRect(135, 70, 16, 16));
				if (TileMap[i][j] == 'b')
					tile.setTextureRect(IntRect(135, 86, 16, 16));
				if (TileMap[i][j] == 'e')
					tile.setTextureRect(IntRect(151, 86, 16, 16));
				if (TileMap[i][j] == 'f')
					tile.setTextureRect(IntRect(151, 70, 16, 16));
				if (TileMap[i][j] == 'g')
					tile.setTextureRect(IntRect(151, 54, 16, 16));
				if (TileMap[i][j] == 'm')
					tile.setTextureRect(IntRect(119, 54, 16, 16));
				if (TileMap[i][j] == 'l')
					tile.setTextureRect(IntRect(119, 70, 16, 16));
				if (TileMap[i][j] == 'j')
					tile.setTextureRect(IntRect(119, 86, 16, 16));
				if (TileMap[i][j] == 'y')
				{
					tile_p.setTextureRect(IntRect(16, 0, 16, 16));
					tile_p.setPosition(j * 16 - offsetX, i * 16 - offsetY);
				}
				if (TileMap[i][j] == '2')
				{
					tile_p.setTextureRect(IntRect(0, 0, 16, 16));
					tile_p.setPosition(j * 16 - offsetX, i * 16 - offsetY);
				}
				if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0'))
					continue;

				tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);

				window.draw(tile);
				window.draw(tile_p);
			}
		//рисунок спрайтов
		window.draw(Mario.sprite);
		window.draw(enemy.sprite);
		//отображение на экран
		window.display();
	}

	return 0;
}
