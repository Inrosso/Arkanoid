#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <locale>
#include <windows.h>
#include <string>
#include <conio.h>

#include "game.h"
#include "records.h"

GameState init_game(int width, int height);
void update_game(GameState& state);
void process_input(GameState& state, bool& paused);
void render_game(const GameState& state);
void draw_stats(const GameState& state);
void clear_screen();
void show_high_scores(RecordsManager& records);
void ask_save_score(RecordsManager& records, int score);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::setlocale(LC_ALL, "Russian");

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    const int field_width = 38;
    const int field_height = 20;
    RecordsManager records_manager;
    bool exit_program = false;

    while (!exit_program) {
        clear_screen();

        std::cout << "============================\n";
        std::cout << "        АРКАНОИД\n";
        std::cout << "============================\n";
        std::cout << "1. Новая игра\n";
        std::cout << "2. Таблица рекордов\n";
        std::cout << "3. Выход\n";
        std::cout << "============================\n";
        std::cout << "Выберите действие (1-3): ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        std::cin.ignore(10000, '\n');

        if (choice < 1 || choice > 3) {
            continue;
        }

        switch (choice) {
        case 1: {
            GameState game = init_game(field_width, field_height);
            bool paused = false;

            while (!game.game_over && !game.level_complete) {
                process_input(game, paused);

                if (!paused) {
                    update_game(game);
                }

                render_game(game);
                draw_stats(game);

                Sleep(45);
            }

            clear_screen();

            if (game.level_complete) {
                std::cout << "ПОЗДРАВЛЯЕМ! Уровень пройден!\n";
            }
            else {
                std::cout << "ИГРА ОКОНЧЕНА\n";
            }

            std::cout << "Ваш счет: " << game.score << "\n\n";

            ask_save_score(records_manager, game.score);

            std::cout << "\nНажмите Enter для возврата в меню...";
            while (_getch() != '\r') {}

            break;
        }

        case 2:
            show_high_scores(records_manager);
            std::cout << "Нажмите Enter для продолжения...";
            while (_getch() != '\r') {}
            break;

        case 3:
            exit_program = true;
            break;
        }
    }

    return 0;
}

void show_high_scores(RecordsManager& records) {
    clear_screen();
    std::cout << "============================\n";
    std::cout << "    ТАБЛИЦА РЕКОРДОВ\n";
    std::cout << "============================\n";
    std::cout << "(Сохраняются 3 лучших результата)\n\n";

    const std::vector<HighScore>& highscores = get_records(records);

    if (highscores.empty()) {
        std::cout << "Пока нет рекордов. Будьте первым!\n";
    }
    else {
        for (size_t i = 0; i < highscores.size(); ++i) {
            std::cout << i + 1 << ". " << highscores[i].name
                << " - " << highscores[i].score << " очков\n";
        }
    }

    std::cout << "\n============================\n";
}

void ask_save_score(RecordsManager& records, int score) {
    if (!is_high_score(records, score)) {
        std::cout << "К сожалению, этот результат не попадает в таблицу рекордов.\n";
        return;
    }

    std::cout << "Ваш результат может попасть в таблицу рекордов!\n";
    std::cout << "Хотите сохранить результат?\n";

    int choice = 0;
    bool valid_choice = false;

    while (!valid_choice) {
        std::cout << "1. Да\n";
        std::cout << "2. Нет\n";
        std::cout << "Выберите (1-2): ";

        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Ошибка! Введите 1 или 2.\n\n";
            continue;
        }

        std::cin.ignore(10000, '\n');

        if (choice == 1 || choice == 2) {
            valid_choice = true;
        }
        else {
            std::cout << "Ошибка! Введите 1 или 2.\n\n";
        }
    }

    if (choice == 1) {
        std::string name;
        bool name_valid = false;

        while (!name_valid) {
            std::cout << "Введите ваше имя (без пробелов, от 1 до 10 символов): ";

            // Используем getline для корректного чтения пустых строк
            std::getline(std::cin, name);

            // Проверка на пустую строку
            if (name.empty()) {
                std::cout << "Ошибка! Имя не может быть пустым.\n";
                std::cout << "Введите другое имя.\n";
                continue;
            }

            // Проверка длины имени
            if (name.length() > 10) {
                std::cout << "Ошибка! Имя должно содержать от 1 до 10 символов.\n";
                std::cout << "Введите другое имя.\n";
                continue;
            }

            // Проверка на наличие пробелов
            bool has_space = false;
            for (char c : name) {
                if (c == ' ') {
                    has_space = true;
                    break;
                }
            }

            if (has_space) {
                std::cout << "Ошибка! Имя не должно содержать пробелов.\n";
                std::cout << "Введите другое имя.\n";
                continue;
            }

            name_valid = true;
        }

        add_record(records, name, score);
        std::cout << "Рекорд сохранен!\n";
    }
}
