// Use c++20 standart

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <sstream>
#include <random>
#include <Windows.h>
#include <conio.h>

typedef unsigned char uchar;

typedef std::vector<uchar> vc;
typedef std::vector<vc> vvc;

struct coord {
	int x, y;

	const coord& operator=(const coord& from) {
		x = from.x;
		y = from.y;
		return from;
	}
	coord operator+(const coord& from) {
		return { x + from.x, y + from.y };
	}
	bool operator!=(const coord& from) {
		return x != from.x || y != from.y;
	}
};

void play(const int width, const int height);

/*
	arguement:
		1 -> width (max 100)
		2 -> height (max 100)
*/
int main(int argv, char* argc[])
{
	if (argv != 3)
	{
		std::cerr << "Invalid count of arguements";
		return ERROR_INVALID_PARAMETER;
	}
	std::stringstream str;
	int width, height;
	std::cout << argc[1] << "<>" << argc[2];
	str << argc[1] << ' ' << argc[2];
	str >> width >> height;
	if (width > 5 && height > 5 && width <= 100 && height <= 100) {
		play(width, height);
		system("pause >nul");
		return 0;
	}
	else {
		std::cerr << "Invalid data";
		return ERROR_INVALID_DATA;
	}

}

void setcur(int x, int y)
{
	COORD coort;
	coort.X = x;
	coort.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coort);
}

void show(const vvc& field, const int reward) {
	static std::string res;
	const size_t x_size = field.size();
	setcur(0, 0);
	res = "";
	for (int y = field[0].size() - 1; y >= 0; --y) {
		for (int x = 0; x < x_size; ++x) {
			res.push_back(field[x][y]);
		}
		res.append("|\n");
	}

	for (int x = field.size(); x >= 0; --x) {
		res.push_back('=');
	}
	std::cout << res << ' ' << reward;
}


void set_berry(vvc& field, const char empty, const char berry) {
	static std::mt19937 gen{ std::random_device()() };
	static std::uniform_int_distribution<int> dist(0, 99);
	coord point;
	do {
		point = { dist(gen) % (int)field.size(), dist(gen) % (int)field[0].size() };
	} while (field[point.x][point.y] != empty);
	field[point.x][point.y] = berry;
}

// Pred: width > 5 && height > 5 && width <= 100 && height <= 100
void play(const int width, const int height)
{
	static const uchar empty = ' ', body = 219, head = 2, berry = 248; // see: https://www.asciitable.com/
	static const std::map<char, coord> direction_map = {
		{'w', {0, 1}},
		{'d', {1, 0}},
		{'s', {0, -1}},
		{'a', {-1, 0}}
	};

	const int square = width * height;
	const int rate = 100;

	vvc field(width, vc(height, empty));
	std::queue<coord> snake;
	coord step, direction = direction_map.at('d'); // answer why we use at() instead operator[], see: https://www.systutorials.com/how-to-operator-access-element-in-a-const-map-in-c/
	int reward = 0;
	char in;

	field[0][0] = field[1][0] = body;
	field[2][0] = head;

	snake.push({ 0, 0 });
	snake.push({ 1, 0 });
	snake.push({ 2, 0 });

	set_berry(field, empty, berry);

	while (snake.size() < square) {
		Sleep(rate);
		show(field, reward);

		if (_kbhit()) {
			in = _getch();
			if (in == '0') {
				return;
			}
			else if (in == 'r') {
				play(width, height);
				return;
			}
			if (direction_map.contains(in) && direction + direction_map.at(in) != coord{ 0, 0 }) {
				direction = direction_map.at(in);
			}
		}
		step = direction + snake.back(); //snake.back() is head of snake

		if (step.x < 0) {
			step.x += width;
		}
		else if (step.x >= width) {
			step.x -= width;
		}
		if (step.y < 0) {
			step.y += height;
		}
		else if (step.y >= height) {
			step.y -= height;
		}

		if (field[step.x][step.y] == berry) {
			field[step.x][step.y] = head;
			field[snake.back().x][snake.back().y] = body;
			snake.push(step);
			++reward;

			if (snake.size() < square) {
				set_berry(field, empty, berry);
			}
		}
		else {
			field[snake.front().x][snake.front().y] = empty;
			snake.pop();
			if (field[step.x][step.y] == empty) {
				field[step.x][step.y] = head;
				field[snake.back().x][snake.back().y] = body;
				snake.push(step);
			}
			else if (field[step.x][step.y] == body) {
				std::cout << " Game over\n";
				return;
			}
		}
	}
}