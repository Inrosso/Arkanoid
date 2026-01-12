#include "records.h"
#include <algorithm>

void sort_records(RecordsManager& manager) {
    // Сортировка по убыванию очков
    for (int i = 0; i < (int)manager.records.size() - 1; ++i) {
        for (int j = i + 1; j < (int)manager.records.size(); ++j) {
            if (manager.records[i].score < manager.records[j].score) {
                HighScore temp = manager.records[i];
                manager.records[i] = manager.records[j];
                manager.records[j] = temp;
            }
        }
    }
}

void add_record(RecordsManager& manager, const std::string& name, int score) {
    // Валидация входных данных
    if (name.empty() || name.length() > 10) {
        return;  // Не добавляем при невалидном имени
    }

    if (score < 0) {
        return;  // Не добавляем отрицательный счет
    }

    manager.records.push_back({ name, score });
    sort_records(manager);

    // Оставляем только лучшие результаты
    if ((int)manager.records.size() > manager.max_records) {
        manager.records.resize(manager.max_records);
    }
}

const std::vector<HighScore>& get_records(const RecordsManager& manager) {
    return manager.records;
}

bool is_high_score(const RecordsManager& manager, int score) {
    if (score < 0) return false;

    if ((int)manager.records.size() < manager.max_records) return true;

    for (const auto& record : manager.records) {
        if (score > record.score) {
            return true;
        }
    }

    return false;
}