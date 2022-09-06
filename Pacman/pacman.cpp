//g++ pacman.cpp -o pacman -lsfml-graphics -lsfml-window -lsfml-system

#include <array>
#include <string>
#include <chrono>
#include <ctime>
#include <cmath>
#include <SFML/Graphics.hpp>


constexpr unsigned char CELL_SIZE = 16;
constexpr unsigned char FONT_HEIGHT = 16;

constexpr unsigned char GHOST_1_CHASE = 2;
constexpr unsigned char GHOST_2_CHASE = 1;
constexpr unsigned char GHOST_3_CHASE = 4;
constexpr unsigned char GHOST_ANIMATION_FRAMES = 6;
constexpr unsigned char GHOST_ANIMATION_SPEED = 4;
constexpr unsigned char GHOST_ESCAPE_SPEED = 4;
constexpr unsigned char GHOST_FRIGHTENED_SPEED = 3;
constexpr unsigned char GHOST_SPEED = 1;

constexpr unsigned char MAP_HEIGHT = 21;
constexpr unsigned char MAP_WIDTH = 21;

constexpr unsigned char PACMAN_ANIMATION_FRAMES = 6;
constexpr unsigned char PACMAN_ANIMATION_SPEED = 4;
constexpr unsigned char PACMAN_DEATH_FRAMES = 12;
constexpr unsigned char PACMAN_SPEED = 2;

constexpr unsigned char SCREEN_RESIZE = 2;

constexpr unsigned short CHASE_DURATION = 1024;
constexpr unsigned short ENERGIZER_DURATION = 512;
constexpr unsigned short FRAME_DURATION = 16667;
constexpr unsigned short GHOST_FLASH_START = 64;
constexpr unsigned short LONG_SCATTER_DURATION = 512;
constexpr unsigned short SHORT_SCATTER_DURATION = 256;


void draw_text(bool center, unsigned short x, unsigned short y, const std::string& text, sf::RenderWindow& window)
{
	short character_x = x;
	short character_y = y;

	unsigned char character_width;

	sf::Sprite character_sprite;

	sf::Texture font_texture;
	font_texture.loadFromFile("Font.png");

	character_width = font_texture.getSize().x / 96;

	character_sprite.setTexture(font_texture);

	if (center)
	{
		character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * text.substr(0, text.find_first_of('\n')).size())));
		character_y = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_HEIGHT - FONT_HEIGHT * (1 + std::count(text.begin(), text.end(), '\n')))));
	}

	for (std::string::const_iterator a = text.begin(); a != text.end(); a++)
	{
		if (*a == '\n')
		{
			if (center)
				character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * text.substr(1 + a - text.begin(), text.find_first_of('\n', 1 + a - text.begin()) - (1 + a - text.begin())).size())));
			else
				character_x = x;

			character_y += FONT_HEIGHT;
		}
		else
		{
			character_sprite.setPosition(character_x, character_y);

			character_sprite.setTextureRect(sf::IntRect(character_width * (*a - 32), 0, character_width, FONT_HEIGHT));

			character_x += character_width;

			window.draw(character_sprite);
		}
	}
}


enum class Cell
{
	Door,
	Empty,
	Energizer,
	Pellet,
	Wall
};


struct Position
{
	short x;
	short y;

	Position& operator=(const Position pos)
	{
		x = pos.x;
		y = pos.y;
		return *this;
	}

	bool operator==(const Position& pos)
	{
		return this->x == pos.x && this->y == pos.y;
	}
};


class Map
{
	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> data;

	Position pacman_position;
	Position home;
	Position home_exit;
public:
	Map(const std::array<std::string, MAP_HEIGHT>& map_sketch)
	{
		for (unsigned char a = 0; a < MAP_HEIGHT; a++)
		{
			for (unsigned char b = 0; b < MAP_WIDTH; b++)
			{
				data[b][a] = Cell::Empty;

				switch (map_sketch[a][b])
				{
					case '#':
					{
						data[b][a] = Cell::Wall;
						break;
					}
					case '=':
					{
						data[b][a] = Cell::Door;
						break;
					}
					case '.':
					{
						data[b][a] = Cell::Pellet;
						break;
					}
					case 'E':
					{
						home_exit.x = CELL_SIZE * b;
						home_exit.y = CELL_SIZE * a;
						break;
					}
					case 'H':
					{
						home.x = CELL_SIZE * b;
						home.y = CELL_SIZE * a;
						break;
					}
					case 'P':
					{
						pacman_position = {static_cast<short int>(CELL_SIZE * b), static_cast<short int>(CELL_SIZE * a)};
						break;
					}
					case 'o':
					{
						data[b][a] = Cell::Energizer;
						break;
					}
				}
			}
		}
	}

	Position get_home()
	{
		return home;
	}

	Position get_home_exit()
	{
		return home_exit;
	}

	Position get_pacman_position()
	{
		return pacman_position;
	}

	std::array<Cell, MAP_HEIGHT>& operator[](size_t ind)
	{
		return data[ind];
	}

	void reset(const std::array<std::string, MAP_HEIGHT>& map_sketch)
	{
		for (unsigned char a = 0; a < MAP_HEIGHT; a++)
		{
			for (unsigned char b = 0; b < MAP_WIDTH; b++)
			{
				data[b][a] = Cell::Empty;

				switch (map_sketch[a][b])
				{
					case '#':
					{
						data[b][a] = Cell::Wall;
						break;
					}
					case '=':
					{
						data[b][a] = Cell::Door;
						break;
					}
					case '.':
					{
						data[b][a] = Cell::Pellet;
						break;
					}
					case 'E':
					{
						home_exit.x = CELL_SIZE * b;
						home_exit.y = CELL_SIZE * a;
						break;
					}
					case 'H':
					{
						home.x = CELL_SIZE * b;
						home.y = CELL_SIZE * a;
						break;
					}
					case 'P':
					{
						pacman_position = {static_cast<short int>(CELL_SIZE * b), static_cast<short int>(CELL_SIZE * a) };
						break;
					}
					case 'o':
					{
						data[b][a] = Cell::Energizer;
						break;
					}
				}
			}
		}
	}

	bool collision(bool collect_pellets, bool use_door, short ent_x, short ent_y)
	{
		bool output = false;

		float cell_x = ent_x / static_cast<float>(CELL_SIZE);
		float cell_y = ent_y / static_cast<float>(CELL_SIZE);

		for (unsigned char a = 0; a < 4; a++)
		{
			short x;
			short y;

			if (a % 2 == 0)
				x = static_cast<short>(floor(cell_x));
			else
				x = static_cast<short>(ceil(cell_x));

			if (a < 2)
				y = static_cast<short>(floor(cell_y));
			else
				y = static_cast<short>(ceil(cell_y));


			if (x >= 0 && y >= 0 && y < MAP_HEIGHT && x < MAP_WIDTH)
			{
				if (collect_pellets)
				{
					output |= data[x][y] == Cell::Energizer;
					if (data[x][y] == Cell::Energizer || data[x][y] == Cell::Pellet)
						data[x][y] = Cell::Empty;
				}
				else
					output |= data[x][y] == Cell::Wall || !use_door && data[x][y] == Cell::Door;
			}
		}

		return output;
	}

	friend sf::RenderWindow& operator<<(sf::RenderWindow& window, Map & map);
};


sf::RenderWindow& operator<<(sf::RenderWindow& window, Map & map)
{
	sf::Sprite sprite;

	sf::Texture texture;
	texture.loadFromFile("Map" + std::to_string(CELL_SIZE) + ".png");

	sprite.setTexture(texture);

	for (unsigned char a = 0; a < MAP_WIDTH; a++)
	{
		for (unsigned char b = 0; b < MAP_HEIGHT; b++)
		{
			sprite.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

			switch (map.data[a][b])
			{
				case Cell::Door:
				{
					sprite.setTextureRect(sf::IntRect(2 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
					window.draw(sprite);
					break;
				}
				case Cell::Energizer:
				{
					sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
					window.draw(sprite);
					break;
				}
				case Cell::Pellet:
				{
					sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));
					window.draw(sprite);
					break;
				}
				case Cell::Wall:
				{
					bool down = b < MAP_HEIGHT - 1 && map.data[a][1 + b] == Cell::Wall;
					bool left = a < 0 || a > 0 && map.data[a - 1][b] == Cell::Wall;
					bool right = a > MAP_WIDTH - 1 || a < MAP_WIDTH - 1 && map.data[1 + a][b] == Cell::Wall;
					bool up = b > 0 && map.data[a][b - 1] == Cell::Wall;

					sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));
					window.draw(sprite);
					break;
				}
				default:
					break;
			}
		}
	}
	return window;
}


class Entity
{
protected:
	unsigned char direction;

	unsigned short animation_timer;

	Position position;
public:
	virtual unsigned char get_direction()
	{
		return direction;
	}

	virtual void draw(bool condition, sf::RenderWindow& window) = 0;

	virtual void reset() = 0;

	virtual void set_animation_timer(unsigned short animation_timer)
	{
		this->animation_timer = animation_timer;
	}

	virtual void set_position(short x, short y)
	{
		position = {x, y};
	}

	virtual void set_position(const Position& pos)
	{
		position = pos;
	}

	virtual Position get_position()
	{
		return position;
	}

	virtual ~Entity() {}
};


class Pacman : public Entity
{
	bool animation_over;

	bool dead;

	unsigned short energizer_timer;
public:
	Pacman(): animation_over(false), dead(false), energizer_timer(0)
	{
		direction = 0;
		position = {0, 0};
	}

	bool get_animation_over()
	{
		return animation_over;
	}
	
	bool get_dead()
	{
		return dead;
	}

	unsigned short get_energizer_timer()
	{
		return energizer_timer;
	}

	void draw(bool victory, sf::RenderWindow& window)
	{
		unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

		sf::Sprite sprite;

		sf::Texture texture;

		sprite.setPosition(position.x, position.y);

		if (dead || victory)
		{
			if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
			{
				animation_timer++;

				texture.loadFromFile("PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

				sprite.setTexture(texture);
				sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

				window.draw(sprite);
			}
			else
				animation_over = true;
		}
		else
		{
			texture.loadFromFile("Pacman" + std::to_string(CELL_SIZE) + ".png");

			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));

			window.draw(sprite);

			animation_timer = (animation_timer + 1) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
		}
	}

	Pacman& operator++()
	{
		reset();
		return *this;
	}

	Pacman operator++(int)
	{
		reset();
		return *this;
	}

	Pacman& operator--()
	{
		reset();
		return *this;
	}

	Pacman operator--(int)
	{
		reset();
		return *this;
	}

	void set_dead(bool dead)
	{
		this->dead = dead;

		if (dead)
			animation_timer = 0;
	}

	void update(unsigned char level, Map& map)
	{
		std::array<bool, 4> walls;
		walls[0] = map.collision(false, false, PACMAN_SPEED + position.x, position.y);
		walls[1] = map.collision(false, false, position.x, position.y - PACMAN_SPEED);
		walls[2] = map.collision(false, false, position.x - PACMAN_SPEED, position.y);
		walls[3] = map.collision(false, false, position.x, PACMAN_SPEED + position.y);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !walls[0])
			direction = 0;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !walls[1])
			direction = 1;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !walls[2])
			direction = 2;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !walls[3])
			direction = 3;

		if (!walls[direction])
		{
			switch (direction)
			{
				case 0:
				{
					position.x += PACMAN_SPEED;
					break;
				}
				case 1:
				{
					position.y -= PACMAN_SPEED;
					break;
				}
				case 2:
				{
					position.x -= PACMAN_SPEED;
					break;
				}
				case 3:
				{
					position.y += PACMAN_SPEED;
					break;
				}
			}
		}

		if (position.x <= -CELL_SIZE)
			position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
		else if (position.x >= CELL_SIZE * MAP_WIDTH)
			position.x = PACMAN_SPEED - CELL_SIZE;

		if (map.collision(1, 0, position.x, position.y))
			energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, level));
		else
			energizer_timer = std::max(0, energizer_timer - 1);
	}

private:
	void reset()
	{
		animation_over = false;
		dead = false;

		direction = 0;

		animation_timer = 0;
		energizer_timer = 0;
	}
};


class Ghost : public  Entity
{
protected:
	bool movement_mode;
	bool use_door;

	unsigned char frightened_mode;

	unsigned char frightened_speed_timer;

	sf::Color color;

	Position home;
	Position home_exit;
	Position target;
public:
	virtual bool pacman_collision(const Position& pacman_position)
	{
		bool same_x_positions = position.x > pacman_position.x - CELL_SIZE && position.x < CELL_SIZE + pacman_position.x;
		bool same_y_positions = position.y > pacman_position.y - CELL_SIZE && position.y < CELL_SIZE + pacman_position.y;
		return same_x_positions && same_y_positions;
	}

	virtual float get_target_distance(unsigned char direction)
	{
		short x = position.x;
		short y = position.y;

		switch (direction)
		{
			case 0:
			{
				x += GHOST_SPEED;
				break;
			}
			case 1:
			{
				y -= GHOST_SPEED;
				break;
			}
			case 2:
			{
				x -= GHOST_SPEED;
				break;
			}
			case 3:
			{
				y += GHOST_SPEED;
				break;
			}
		}

		return static_cast<float>(sqrt(pow(x - target.x, 2) + pow(y - target.y, 2)));
	}

	virtual void draw(bool flash, sf::RenderWindow& window)
	{
		unsigned char body_frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(GHOST_ANIMATION_SPEED)));

		sf::Sprite body;
		sf::Sprite face;

		sf::Texture texture;
		texture.loadFromFile("Ghost" + std::to_string(CELL_SIZE) + ".png");

		body.setTexture(texture);
		body.setPosition(position.x, position.y);
		body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

		face.setTexture(texture);
		face.setPosition(position.x, position.y);


		if (frightened_mode == 0)
		{
			body.setColor(color);

			face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE));

			window.draw(body);
		}
		else if (frightened_mode == 1)
		{
			body.setColor(sf::Color(36, 36, 255));
			face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

			if (flash && body_frame % 2 == 0)
			{
				body.setColor(sf::Color(255, 255, 255));
				face.setColor(sf::Color(255, 0, 0));
			}
			else
			{
				body.setColor(sf::Color(36, 36, 255));
				face.setColor(sf::Color(255, 255, 255));
			}

			window.draw(body);
		}
		else
			face.setTextureRect(sf::IntRect(CELL_SIZE * direction, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE));

		window.draw(face);

		animation_timer = (animation_timer + 1) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
	}

	void reset()
	{
		movement_mode = false;
		
		direction = 0;
		frightened_mode = 0;
		frightened_speed_timer = 0;

		animation_timer = 0;

		target = home_exit;
	}

	virtual void switch_mode()
	{
		movement_mode = !movement_mode;
	}

	virtual void update(unsigned char level, Map& map, Ghost& blinky, Pacman& pacman)
	{
		bool move = false;

		unsigned char available_ways = 0;
		unsigned char speed = GHOST_SPEED;

		std::array<bool, 4> walls;

		if (frightened_mode == 0 && pacman.get_energizer_timer() == ENERGIZER_DURATION / pow(2, level))
		{
			frightened_speed_timer = GHOST_FRIGHTENED_SPEED;
			frightened_mode = 1;
		}
		else if (pacman.get_energizer_timer() == 0 && frightened_mode == 1)
		{
			frightened_mode = 0;
		}

		if (frightened_mode == 2 && position.x % GHOST_ESCAPE_SPEED == 0 && position.y % GHOST_ESCAPE_SPEED == 0)
			speed = GHOST_ESCAPE_SPEED;

		update_target(pacman.get_direction(), blinky.get_position(), pacman.get_position());

		walls[0] = map.collision(false, use_door, speed + position.x, position.y);
		walls[1] = map.collision(false, use_door, position.x, position.y - speed);
		walls[2] = map.collision(false, use_door, position.x - speed, position.y);
		walls[3] = map.collision(false, use_door, position.x, speed + position.y);

		if (frightened_mode != 1)
		{
			unsigned char optimal_direction = 4;

			move = true;

			for (unsigned char a = 0; a < 4; a++)
			{
				if (a == (2 + direction) % 4)
					continue;
				else if (!walls[a])
				{
					if (optimal_direction == 4 || get_target_distance(a) < get_target_distance(optimal_direction))
						optimal_direction = a;
					available_ways++;
				}
			}

			if (available_ways > 1)
				direction = optimal_direction;
			else
			{
				if (optimal_direction == 4)
					direction = (2 + direction) % 4;
				else
					direction = optimal_direction;
			}
		}
		else
		{
			unsigned char random_direction = rand() % 4;

			if (frightened_speed_timer == 0)
			{
				move = true;

				frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

				for (unsigned char a = 0; a < 4; a++)
				{
					if (a != (2 + direction) % 4 && 0 == walls[a])
						available_ways++;
				}

				if (0 < available_ways)
				{
					while (walls[random_direction] || random_direction == (2 + direction) % 4)
						random_direction = rand() % 4;
					direction = random_direction;
				}
				else
					direction = (2 + direction) % 4;
			}
			else
				frightened_speed_timer--;
		}

		if (move)
		{
			switch (direction)
			{
				case 0:
				{
					position.x += speed;
					break;
				}
				case 1:
				{
					position.y -= speed;
					break;
				}
				case 2:
				{
					position.x -= speed;
					break;
				}
				case 3:
				{
					position.y += speed;
					break;
				}
			}

			if (position.x <= -CELL_SIZE)
				position.x = CELL_SIZE * MAP_WIDTH - speed;
			else if (position.x >= CELL_SIZE * MAP_WIDTH)
				position.x = speed - CELL_SIZE;
		}

		if (pacman_collision(pacman.get_position()))
		{
			if (frightened_mode == 0)
				pacman.set_dead(true);
			else
			{
				use_door = 1;
				frightened_mode = 2;
				target = home;
			}
		}
	}

	virtual void update_target(unsigned char pacman_direction, const Position& blinky_position, const Position& pacman_position) = 0;

	virtual ~Ghost() {}
};


class Blinky : public Ghost
{
public:
	Blinky(const Position& home, const Position& home_exit)
	{
		this->home = home;
		this->home_exit = home_exit;
		color = sf::Color(255, 0, 0);
	}

	void reset()
	{
		Ghost::reset();
		use_door = false;
	}

	virtual void update_target(unsigned char pacman_direction, const Position& blinky_position, const Position& pacman_position)
	{
		if (use_door)
		{
			if (position == target)
			{
				if (target == home_exit)
					use_door = false;
				else if (target == home)
				{
					frightened_mode = 0;
					target = home_exit;
				}
			}
		}
		else
		{
			if (!movement_mode)
				target = {CELL_SIZE * (MAP_WIDTH - 1), 0};
			else
				target = pacman_position;
		}
	}
};


class Pinky : public Ghost
{
public:
	Pinky(const Position& home, const Position& home_exit)
	{
		this->home = home;
		this->home_exit = home_exit;
		color = sf::Color(255, 182, 255);
	}

	void reset()
	{
		Ghost::reset();
		use_door = true;
	}

	virtual void update_target(unsigned char pacman_direction, const Position& blinky_position, const Position& pacman_position)
	{
		if (use_door)
		{
			if (position == target)
			{
				if (target == home_exit)
					use_door = false;
				else if (target == home)
				{
					frightened_mode = 0;
					target = home_exit;
				}
			}
		}
		else
		{
			if (!movement_mode)
				target = {0, 0};
			else
			{
				target = pacman_position;
				switch (pacman_direction)
				{
					case 0:
					{
						target.x += CELL_SIZE * GHOST_1_CHASE;
						break;
					}
					case 1:
					{
						target.y -= CELL_SIZE * GHOST_1_CHASE;
						break;
					}
					case 2:
					{
						target.x -= CELL_SIZE * GHOST_1_CHASE;
						break;
					}
					case 3:
					{
						target.y += CELL_SIZE * GHOST_1_CHASE;
						break;
					}
				}
			}
		}
	}
};


class Inky : public Ghost
{
public:
	Inky(const Position& home, const Position& home_exit)
	{
		this->home = home;
		this->home_exit = home_exit;
		color = sf::Color(0, 255, 255);
	}

	void reset()
	{
		Ghost::reset();
		use_door = true;
	}

	virtual void update_target(unsigned char pacman_direction, const Position& blinky_position, const Position& pacman_position)
	{
		if (use_door)
		{
			if (position == target)
			{
				if (target == home_exit)
					use_door = false;
				else if (target == home)
				{
					frightened_mode = 0;
					target = home_exit;
				}
			}
		}
		else
		{
			if (!movement_mode)
				target = {CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1)};
			else
			{
				target = pacman_position;
				switch (pacman_direction)
				{
					case 0:
					{
						target.x += CELL_SIZE * GHOST_2_CHASE;
						break;
					}
					case 1:
					{
						target.y -= CELL_SIZE * GHOST_2_CHASE;
						break;
					}
					case 2:
					{
						target.x -= CELL_SIZE * GHOST_2_CHASE;
						break;
					}
					case 3:
					{
						target.y += CELL_SIZE * GHOST_2_CHASE;
						break;
					}
				}
				target.x += target.x - blinky_position.x;
				target.y += target.y - blinky_position.y;
			}
		}
	}
};


class Clyde : public Ghost
{
public:
	Clyde(const Position& home, const Position& home_exit)
	{
		this->home = home;
		this->home_exit = home_exit;
		color = sf::Color(255, 182, 85);
	}

	void reset()
	{
		Ghost::reset();
		use_door = true;
	}

	virtual void update_target(unsigned char pacman_direction, const Position& blinky_position, const Position& pacman_position)
	{
		if (use_door)
		{
			if (position == target)
			{
				if (target == home_exit)
					use_door = false;
				else if (target == home)
				{
					frightened_mode = 0;
					target = home_exit;
				}
			}
		}
		else
		{
			if (!movement_mode)
				target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
			else
			{
				if (CELL_SIZE * GHOST_3_CHASE <=
				    sqrt(pow(position.x - pacman_position.x, 2) + pow(position.y - pacman_position.y, 2)))
					target = pacman_position;
				else
					target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
			}
		}
	}
};


class GameManager
{
	unsigned char current_wave;

	unsigned short wave_timer;

	std::array<std::string, MAP_HEIGHT> map_sketch;

	std::array<Ghost*, 4> ghosts;

	Pacman pacman;

	Map map;
public:
	GameManager(const std::array<std::string, MAP_HEIGHT>& map_sketch) : current_wave(0), wave_timer(LONG_SCATTER_DURATION), map(map_sketch)
	{
		this->map_sketch = map_sketch;
		Position home = map.get_home();
		Position home_exit = map.get_home_exit();
		ghosts = {new Blinky(home, home_exit), new Pinky(home, home_exit), new Inky(home, home_exit), new Clyde(home, home_exit)};
		pacman.set_position(map.get_pacman_position());
	}

	GameManager(const GameManager& other) : current_wave(0), wave_timer(LONG_SCATTER_DURATION), map(other.map_sketch)
	{
		map_sketch = other.map_sketch;
		for (size_t i = 0; i < 4; i++)
			*ghosts[i] = *other.ghosts[i];
		pacman = other.pacman;
	}

	GameManager& operator=(const GameManager& other)
	{
		current_wave = other.current_wave;
		wave_timer = other.wave_timer;
		map_sketch = other.map_sketch;

		for (size_t i = 0; i < 4; i++)
			*ghosts[i] = *other.ghosts[i];

		pacman = other.pacman;
		map = other.map;

		return *this;
	}

	void draw(bool flash, sf::RenderWindow& window)
	{
		window << map;

		for (Ghost * ghost : ghosts)
			ghost->draw(flash, window);
	}

	void reset(unsigned char level)
	{
		current_wave = 0;

		wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, level));

		map.reset(map_sketch);

		for (unsigned char a = 0; a < 4; a++)
		{
			if (a != 0)
				ghosts[a]->set_position(map.get_home());
			else
				ghosts[a]->set_position(map.get_home_exit());
			ghosts[a]->reset();
		}

		pacman.set_position(map.get_pacman_position());

		switch (level)
		{
			case 0:
				++pacman;
				break;
			case 1:
				--pacman;
				break;
			case 2:
				pacman--;
				break;
			default:
				pacman++;
				break;
		}
	}

	void update(unsigned char level)
	{
		if (pacman.get_energizer_timer() == 0)
		{
			if (wave_timer == 0)
			{
				if (current_wave < 7)
				{
					current_wave++;
					for (Ghost * ghost : ghosts)
						ghost->switch_mode();
				}

				if (current_wave % 2 == 1)
					wave_timer = CHASE_DURATION;
				else if (current_wave == 2)
					wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, level));
				else
					wave_timer = static_cast<unsigned short>(SHORT_SCATTER_DURATION / pow(2, level));
			}
			else
				wave_timer--;
		}

		for (Ghost * ghost : ghosts)
			ghost->update(level, map, *ghosts[0], pacman);

		pacman.update(level, map);
	}

	Pacman& get_pacman()
	{
		return pacman;
	}

	Map& get_map()
	{
		return map;
	}

	static GameManager get_game_manager(const std::array<std::string, MAP_HEIGHT>& map_sketch)
	{
		GameManager gm(map_sketch);
		return gm;
	}

	~GameManager()
	{
		for (size_t i = 0; i < 4; i++)
			delete ghosts[i];
	}
};


int main()
{
	bool game_won = false;

	unsigned lag = 0;

	unsigned char level = 0;

	std::chrono::time_point<std::chrono::steady_clock> previous_time;

	std::array<std::string, MAP_HEIGHT> map_sketch = {
		" ################### ",
		" #........#........# ",
		" #o##.###.#.###.##o# ",
		" #.................# ",
		" #.##.#.#####.#.##.# ",
		" #....#...#...#....# ",
		" ####.### # ###.#### ",
		"    #.#   E   #.#    ",
		"#####.# ##=## #.#####",
		"     .  # H #  .     ",
		"#####.# ##### #.#####",
		"    #.#       #.#    ",
		" ####.# ##### #.#### ",
		" #........#........# ",
		" #.##.###.#.###.##.# ",
		" #o.#.....P.....#.o# ",
		" ##.#.#.#####.#.#.## ",
		" #....#...#...#....# ",
		" #.######.#.######.# ",
		" #.................# ",
		" ################### "
	};

	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);

	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

	srand(static_cast<unsigned>(time(0)));

	GameManager GM = GameManager::get_game_manager(map_sketch);
	GM.reset(level);

	previous_time = std::chrono::steady_clock::now();

	try
	{
		while (window.isOpen())
		{
			unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::steady_clock::now() - previous_time).count();

			lag += delta_time;

			previous_time += std::chrono::microseconds(delta_time);

			while (lag >= FRAME_DURATION)
			{
				lag -= FRAME_DURATION;

				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						window.close();
				}

				if (!game_won && !GM.get_pacman().get_dead())
				{
					game_won = true;

					GM.update(level);

					for (size_t i = 0; i < MAP_HEIGHT; ++i)
					{
						for (size_t j = 0; j < MAP_WIDTH; ++j)
						{
							if (GM.get_map()[i][j] == Cell::Pellet)
							{
								game_won = false;
								break;
							}
						}
						if (!game_won)
							break;
					}

					if (game_won)
						GM.get_pacman().set_animation_timer(0);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					game_won = false;
					if (GM.get_pacman().get_dead())
						level = 0;
					else
						level++;

					GM.reset(level);
				}

				if (lag < FRAME_DURATION)
				{
					window.clear();
					if (!game_won && !GM.get_pacman().get_dead())
					{
						GM.draw(GM.get_pacman().get_energizer_timer() <= GHOST_FLASH_START, window);
						draw_text(false, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(level + 1), window);
					}

					GM.get_pacman().draw(game_won, window);

					if (GM.get_pacman().get_animation_over())
					{
						if (game_won)
							draw_text(1, 0, 0, "Next level!", window);
						else
							draw_text(1, 0, 0, "Game over", window);
					}

					window.display();
				}
			}
		}
	}
	catch (...)
	{
		window.close();
	}
}