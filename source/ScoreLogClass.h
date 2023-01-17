#pragma once

#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using std::set;
using std::vector;


class ScoreLogClass
{
public:
	ScoreLogClass();
	~ScoreLogClass();

	bool ReadSavedScoreboard(const char* fileWithScores);
	bool SaveScoreboard(const char* fileWithScores, vector<double>& scoresToSave);

	void AddScore(double seconds);
	size_t GetBestScores(int numOfScores, vector<double>& bestScores);

private:
	set<double> m_Scores_seconds;
};

