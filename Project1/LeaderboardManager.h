#pragma once
#include<vector>
#include <fstream>
#include <algorithm>
#include <string>

class LeaderboardManager
{
	//싱글톤으로 만들것..
public:
	static LeaderboardManager* GetInstance() {
		if (Ins == nullptr) {
			Ins = new LeaderboardManager();
		}
		return  Ins; 
	}
	~LeaderboardManager();
	void load();
	void save();
	void AddScore(std::string nickname, int score);
	std::vector<std::pair<std::string, int>>& GetScores() { return scores; }
private:
	static LeaderboardManager* Ins;
	LeaderboardManager();
	void sortScores();

	//std::vector<int> scores;
	std::vector<std::pair<std::string, int>> scores;
	int maxEntries = 10; // 최대 랭킹 개수
	std::string filename = "Resources/leaderboard.txt";
};
