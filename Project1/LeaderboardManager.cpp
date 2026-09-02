#include "LeaderboardManager.h"

LeaderboardManager* LeaderboardManager::Ins = nullptr;

void LeaderboardManager::load() {
    scores.clear();
    std::ifstream file(filename);
    //최초 실행시에는 파일이 없으므로 바로 리턴
    if (!file.is_open()) return;
    
    std::string name;
    int score = 0;
    while (file >> name >> score) {
        scores.push_back(std::make_pair(name, score));
    }

    sortScores();

}
void LeaderboardManager::save() {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const auto& entry : scores) {
        file << entry.first << " " << entry.second << "\n";
    }
}
void LeaderboardManager::AddScore(std::string nickname, int score) {
    scores.push_back(std::make_pair(nickname, score));
    sortScores();  

    if (scores.size() > maxEntries) {
        scores.resize(maxEntries);
    }

    save();
}	

LeaderboardManager::LeaderboardManager()
{
	load();
}
LeaderboardManager::~LeaderboardManager()
{
	save();
}
void LeaderboardManager::sortScores()
{
    std::sort(scores.begin(), scores.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    });
}