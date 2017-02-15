#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <list>

using namespace sf;

const int Width = 1200;
const int Height = 980;

float D = 0.01745f;

class Animation
{
public:
	float m_frame, m_speed;
	Sprite sprite;
	std::vector<IntRect> frames;

	Animation() {}

	Animation(Texture& t, int x, int y, int Width, int Height, int count, float Speed)
	{
		m_frame = 0;
		m_speed = Speed;
		for (int i = 0; i < count; ++i)
		{
			frames.push_back(IntRect(x + i*Width, y, Width, Height));
		}

		sprite.setTexture(t);
		sprite.setOrigin(Width / 2, Height / 2);
		sprite.setTextureRect(frames[0]);
	}

	void update()
	{
		m_frame += m_speed;
		int n = frames.size();
		if (m_frame >= n)
		{
			m_frame -= n;
		}
		if (n > 0)
		{
			sprite.setTextureRect(frames[int(m_frame)]);
		}
	}

	bool isEnd()
	{
		return m_frame + m_speed >= frames.size();
	}

};


class Entity
{
public:
	float m_x, m_y, m_dx, m_dy, m_radius, m_angle;
	bool life;
	std::string name;
	Animation anim;

	Entity()
	{
		life = 1;
	}

	void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1)
	{
		anim = a;
		m_x = X; m_y = Y;
		m_angle = Angle;
		m_radius = radius;
	}

	virtual void update() {};

	void draw(RenderWindow& app)
	{
		anim.sprite.setPosition(m_x, m_y);
		anim.sprite.setRotation(m_angle + 90);
		app.draw(anim.sprite);
		CircleShape circle(m_radius);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setPosition(m_x, m_y);
		circle.setOrigin(m_radius, m_radius);
	}

	virtual ~Entity() {};
};


class asteroid : public Entity
{
public:
	asteroid()
	{
		m_dx = rand() % 8 - 4;
		m_dy = rand() % 8 - 4;
		name = "Asteroid";
	}

	void  update()
	{
		m_x += m_dx;
		m_y += m_dy;

		if (m_x > Width)
		{
			m_x = 0;
		}
		if (m_x < 0)
		{
			m_x = Width;
		}
		if (m_y > Height)
		{
			m_y = 0;
		}
		if (m_y < 0)
		{
			m_y = Height;
		}
	}
};



class bullet : public Entity
{
public:
	bullet()
	{
		name = "Bullet";
	}

	void  update()
	{
		m_dx = cos(m_angle * D) * 6;
		m_dy = sin(m_angle * D) * 6;
		m_x += m_dx;
		m_y += m_dy;
		if (m_x > Width || m_x < 0 || m_y > Height || m_y < 0)
		{
			life = 0;
		}
	}
};


class player : public Entity
{
public:
	bool m_thrust;

	player()
	{
		name = "Millenium Falcon";
	}

	void update()
	{
		if (m_thrust)
		{
			m_dx += cos(m_angle * D) * 0.2;
			m_dy += sin(m_angle * D) * 0.2;
		}
		else
		{
			m_dx *= 0.99;
			m_dy *= 0.99;
		}

		int m_maxSpeed = 15;
		float m_speed = sqrt(m_dx * m_dx + m_dy * m_dy);
		if (m_speed > m_maxSpeed)
		{
			m_dx *= m_maxSpeed / m_speed;
			m_dy *= m_maxSpeed / m_speed;
		}

		m_x += m_dx;
		m_y += m_dy;

		if (m_x > Width)
		{
			m_x = 0;
		}
		if (m_x < 0)
		{
			m_x = Width;
		}
		if (m_y > Height)
		{
			m_y = 0;
		}
		if (m_y < 0)
		{
			m_y = Height;
		}
	}

};


bool isCollide(Entity *a, Entity *b)
{
	return (b->m_x - a->m_x)*(b->m_x - a->m_x) +
		(b->m_y - a->m_y)*(b->m_y - a->m_y) <
		(a->m_radius + b->m_radius)*(a->m_radius + b->m_radius);
}


int main()
{
	srand(time(0));

	sf::Music music;
	if (!music.openFromFile("images/starwars.wav"))
	{
		return -1;
	}
	music.play();

	RenderWindow app(VideoMode(Width, Height), "Millennium Falcon");
	app.setFramerateLimit(60);

	Texture t1, t2, t3, t4, t5, t6, t7;
	t1.loadFromFile("images/spaceship2.png");
	t2.loadFromFile("images/background.jpg");
	t3.loadFromFile("images/explosions/type_C.png");
	t4.loadFromFile("images/rock.png");
	t5.loadFromFile("images/fire_blue.png");
	t6.loadFromFile("images/rock_small.png");
	t7.loadFromFile("images/explosions/type_B.png");

	t1.setSmooth(true);
	t2.setSmooth(true);

	Sprite background(t2);

	Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.4);
	Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
	Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.5);
	Animation sBullet(t5, 0, 0, 32, 64, 16, 0.3);
	Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
	Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
	Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);


	std::list<Entity*> entities;

	for (int i = 0; i < 15; ++i)
	{
		asteroid *a = new asteroid();
		a->settings(sRock, rand() % Width, rand() % Height, rand() % 360, 25);
		entities.push_back(a);
	}

	player *p = new player();
	p->settings(sPlayer, 200, 200, 0, 20);
	entities.push_back(p);

	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
				app.close();

			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Space)
				{
					bullet *b = new bullet();
					b->settings(sBullet, p->m_x, p->m_y, p->m_angle, 10);
					entities.push_back(b);
				}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p->m_angle += 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p->m_angle -= 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			p->m_thrust = true;
		}
		else
		{
			p->m_thrust = false;
		}



		for (auto a : entities)
			for (auto b : entities)
			{
				if (a->name == "Asteroid" && b->name == "Bullet")
					if (isCollide(a, b))
					{
						a->life = false;
						b->life = false;

						Entity *e = new Entity();
						e->settings(sExplosion, a->m_x, a->m_y);
						e->name = "explosion";
						entities.push_back(e);


						for (int i = 0; i < 2; i++)
						{
							if (a->m_radius == 15) continue;
							Entity *e = new asteroid();
							e->settings(sRock_small, a->m_x, a->m_y, rand() % 360, 15);
							entities.push_back(e);
						}

					}

				if (a->name == "Millenium Falcon" && b->name == "Asteroid")
					if (isCollide(a, b))
					{
						b->life = false;

						Entity *e = new Entity();
						e->settings(sExplosion_ship, a->m_x, a->m_y);
						e->name = "explosion";
						entities.push_back(e);

						p->settings(sPlayer, Width / 2, Height / 2, 0, 20);
						p->m_dx = 0; p->m_dy = 0;
					}
			}


		if (p->m_thrust)
		{
			p->anim = sPlayer_go;
		}
		else
		{
			p->anim = sPlayer;
		}


		for (auto e : entities)
			if (e->name == "explosion")
				if (e->anim.isEnd())
				{
					e->life = 0;
				}

		if (rand() % 150 == 0)
		{
			asteroid *a = new asteroid();
			a->settings(sRock, 0, rand() % Height, rand() % 360, 25);
			entities.push_back(a);
		}

		for (auto i = entities.begin(); i != entities.end();)
		{
			Entity *e = *i;

			e->update();
			e->anim.update();

			if (e->life == false)
			{
				i = entities.erase(i);
				delete e;
			}
			else i++;
		}

		app.draw(background);

		for (auto i : entities)
		{
			i->draw(app);
		}

		app.display();
	}

	return 0;
}
