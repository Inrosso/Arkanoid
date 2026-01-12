#include "game.h"
#include <cstdlib>

// Сложная логика определения попадания в кирпич
bool check_brick_hit(const Brick& brick, int ball_x, int ball_y) {
    for (int offset = 0; offset < brick.width; ++offset) {
        if (ball_x == brick.x + offset && ball_y == brick.y) {
            return true;
        }
    }
    return false;
}

GameState init_game(int width, int height) {
    // Защита от слишком маленьких размеров
    if (width < 20) width = 20;
    if (height < 10) height = 10;

    GameState state;
    state.width = width;
    state.height = height;

    // Создание пустого поля
    state.field.resize(height, std::vector<char>(width, ' '));

    // Инициализация платформы
    state.platform.x = width / 2 - 4;
    state.platform.width = 8;
    state.platform.symbol = '=';

    // Инициализация мяча
    state.ball.x = width / 2;
    state.ball.y = height - 2;
    state.ball.dx = (std::rand() % 2 == 0) ? 1 : -1;
    state.ball.dy = -1;
    state.ball.symbol = 'O';

    // Создание кирпичей
    for (int row = 2; row < 7 && row < height; ++row) {
        for (int col = 2; col < width - 4; col += 4) {
            Brick brick;
            brick.x = col;
            brick.width = 4;
            brick.y = row;
            brick.destroyed = false;

            state.bricks.push_back(brick);

            // Рисование кирпича на поле
            if (col < width && col + 3 < width) {
                state.field[row][col] = '|';
                state.field[row][col + 1] = '#';
                state.field[row][col + 2] = '#';
                state.field[row][col + 3] = '|';
            }
        }
    }

    state.score = 0;
    state.lives = 3;
    state.game_over = false;
    state.level_complete = false;

    return state;
}

// Сложная логика определения угла отскока от платформы
void handle_platform_collision(GameState& state, int height) {
    int hit_pos = state.ball.x - state.platform.x;
    int section = state.platform.width / 3;

    if (hit_pos < section) {
        state.ball.dx = -1;  // Левая часть платформы
    }
    else if (hit_pos > 2 * section) {
        state.ball.dx = 1;   // Правая часть платформы
    }
}

// Сложная логика определения направления отскока от кирпича
void handle_brick_collision(GameState& state, Brick& brick, int prev_x, int prev_y) {
    brick.destroyed = true;
    state.score += 10;

    // Очистка кирпича на поле
    for (int offset = 0; offset < brick.width; ++offset) {
        if (brick.x + offset < state.width && brick.y < state.height) {
            state.field[brick.y][brick.x + offset] = ' ';
        }
    }

    // Определение направления отскока
    if (prev_y > brick.y) {
        state.ball.dy = 1;  // Мяч пришел сверху
    }
    else if (prev_y < brick.y) {
        state.ball.dy = -1;  // Мяч пришел снизу
    }
    else if (prev_x < brick.x) {
        state.ball.dx = 1;   // Мяч пришел слева
        state.ball.dy = -state.ball.dy;
    }
    else if (prev_x > brick.x + brick.width - 1) {
        state.ball.dx = -1;  // Мяч пришел справа
        state.ball.dy = -state.ball.dy;
    }
}

// Проверка и корректировка столкновения с платформой с учетом стен
void check_platform_with_wall_collision(GameState& state, int width, int height) {
    // Если мяч находится в зоне платформы и движется вниз
    if (state.ball.y == height - 3 && state.ball.dy > 0) {
        // Проверяем, находится ли мяч над платформой
        if (state.ball.x >= state.platform.x &&
            state.ball.x < state.platform.x + state.platform.width) {

            // Отскок от платформы
            state.ball.dy = -state.ball.dy;
            handle_platform_collision(state, height);
            state.ball.y = height - 4;
        }
    }
}

void handle_collisions(GameState& state) {
    int width = state.width;
    int height = state.height;

    // Предыдущая позиция для определения направления
    int prev_x = state.ball.x - state.ball.dx;
    int prev_y = state.ball.y - state.ball.dy;

    // ПЕРВОЕ: Проверка столкновения с платформой (до проверки стен)
    // Это предотвращает прохождение сквозь платформу при отскоке от стены
    if (state.ball.y == height - 3 && state.ball.dy > 0) {
        if (state.ball.x >= state.platform.x &&
            state.ball.x < state.platform.x + state.platform.width) {

            state.ball.dy = -state.ball.dy;
            handle_platform_collision(state, height);
            state.ball.y = height - 4;
            return;
        }
    }

    // ВТОРОЕ: Столкновение со стенами
    if (state.ball.x <= 1) {
        state.ball.x = 2;
        state.ball.dx = -state.ball.dx;

        // После отскока от стены проверяем, не оказался ли мяч в платформе
        check_platform_with_wall_collision(state, width, height);
        return;
    }

    if (state.ball.x >= width - 2) {
        state.ball.x = width - 3;
        state.ball.dx = -state.ball.dx;

        // После отскока от стены проверяем, не оказался ли мяч в платформе
        check_platform_with_wall_collision(state, width, height);
        return;
    }

    if (state.ball.y <= 1) {
        state.ball.y = 2;
        state.ball.dy = -state.ball.dy;
        return;
    }

    // ТРЕТЬЕ: Промах
    if (state.ball.y >= height - 1) {
        --state.lives;
        if (state.lives <= 0) {
            state.game_over = true;
            return;
        }

        // Сброс мяча
        state.ball.x = state.platform.x + state.platform.width / 2;
        state.ball.y = height - 3;
        state.ball.dx = (std::rand() % 2 == 0) ? 1 : -1;
        state.ball.dy = -1;
        return;
    }

    // ЧЕТВЕРТОЕ: Столкновение с кирпичами
    for (auto& brick : state.bricks) {
        if (!brick.destroyed && check_brick_hit(brick, state.ball.x, state.ball.y)) {
            handle_brick_collision(state, brick, prev_x, prev_y);
            return;
        }
    }

    // ПЯТОЕ: Дополнительная проверка платформы для случаев,
    // когда мяч мог пролететь через зону платформы из-за отскока от стены
    if (state.ball.y == height - 3 && state.ball.dy > 0) {
        if (state.ball.x >= state.platform.x &&
            state.ball.x < state.platform.x + state.platform.width) {

            state.ball.dy = -state.ball.dy;
            handle_platform_collision(state, height);
            state.ball.y = height - 4;
            return;
        }
    }
}

void update_game(GameState& state) {
    // Обновление позиции мяча
    state.ball.x += state.ball.dx;
    state.ball.y += state.ball.dy;

    handle_collisions(state);

    // Проверка завершения уровня
    if (!state.game_over) {
        state.level_complete = true;
        for (const auto& brick : state.bricks) {
            if (!brick.destroyed) {
                state.level_complete = false;
                break;
            }
        }
    }
}