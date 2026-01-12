#pragma once

#include <string>
#include <vector>

struct HighScore {
	std::string name;
	int score;
};

struct RecordsManager {
	std::vector<HighScore> records;
	const int max_records = 3;
};

void add_record(RecordsManager& manager, const std::string& name, int score);
const std::vector<HighScore>& get_records(const RecordsManager& manager);
bool is_high_score(const RecordsManager& manager, int score);