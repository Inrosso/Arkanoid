#include "game.h"
#include <conio.h>

bool is_valid_key(char input) {
    return input == 'a' || input == 'A' || input == 'd' || input == 'D' ||
        input == 'p' || input == 'P' || input == 27 ||
        input == 75 || input == 77;
}

void process_input(GameState& state, bool& paused) {
    if (!_kbhit()) return;

    char input = _getch();

    // Игнорируем недопустимые клавиши
    if (!is_valid_key(input)) {
        return;
    }

    switch (input) {
    case 'a':
    case 'A':
    case 75:
        // Движение платформы только если не пауза
        if (!paused && state.platform.x > 2) {
            state.platform.x -= 2;
        }
        break;

    case 'd':
    case 'D':
    case 77:
        // Движение платформы только если не пауза
        if (!paused && state.platform.x + state.platform.width < state.width - 2) {
            state.platform.x += 2;
        }
        break;

    case 'p':
    case 'P':
        paused = !paused;
        break;

    case 27:
        state.game_over = true;
        break;
    }
}