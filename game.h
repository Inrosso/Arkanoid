#pragma once

#include <vector>

struct Ball {
	int x;
	int y;
	int dx;
	int dy;
	char symbol;
};

struct Platform {
	int x;
	int width;
	char symbol;
};

struct Brick {
	int x;
	int width;
	int y;
	bool destroyed;
};

struct GameState {
	std::vector<std::vector<char>> field;
	std::vector<Brick> bricks;
	Platform platform;
	Ball ball;
	int score;
	int lives;
	bool game_over;
	bool level_complete;
	int width;
	int height;
};