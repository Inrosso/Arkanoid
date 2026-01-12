#include "game.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

void clear_screen() {
    system("cls");
}

void render_game(const GameState& state) {
    clear_screen();

    // Верхняя граница
    std::cout << '+';
    for (int i = 0; i < state.width; ++i) std::cout << '-';
    std::cout << "+\n";

    // Отрисовка игрового поля
    for (int y = 0; y < state.height; ++y) {
        std::cout << '|';

        for (int x = 0; x < state.width; ++x) {
            if (y == state.ball.y && x == state.ball.x) {
                std::cout << state.ball.symbol;
            }
            else if (y == state.height - 3 &&
                x >= state.platform.x &&
                x < state.platform.x + state.platform.width) {
                std::cout << state.platform.symbol;
            }
            else {
                std::cout << state.field[y][x];
            }
        }

        std::cout << "|\n";
    }

    // Нижняя граница
    std::cout << '+';
    for (int i = 0; i < state.width; ++i) std::cout << '-';
    std::cout << "+\n";
}

void draw_stats(const GameState& state) {
    std::cout << "\nСчет: " << std::setw(6) << state.score;
    std::cout << "  Жизни: " << state.lives << "\n";
    std::cout << "Управление: A/D или стрелки - движение, P - пауза, ESC - выход\n";
}