#include "ScoreLogClass.h"


ScoreLogClass::ScoreLogClass()
{

}

ScoreLogClass::~ScoreLogClass()
{

}

bool ScoreLogClass::ReadSavedScoreboard(const char* fileWithScores)
{
	std::ifstream fin;

	fin.open(fileWithScores);
	if (fin.fail())
	{
		return false;
	}

	m_Scores_seconds.clear();
	double score = 0;
	while (fin >> score)
	{
		m_Scores_seconds.insert(score);
	}

	fin.close();

	return true;
}

bool ScoreLogClass::SaveScoreboard(const char* fileWithScores, vector<double>& scoresToSave)
{
	std::ofstream fout;

	fout.open(fileWithScores, std::ofstream::out | std::ofstream::trunc);
	if (fout.fail())
	{
		return false;
	}

	for (auto score : scoresToSave)
	{
		fout << score << "\n";
	}

	fout.close();

	return true;
}

void ScoreLogClass::AddScore(double score_seconds)
{
	m_Scores_seconds.insert(score_seconds);
}

size_t ScoreLogClass::GetBestScores(int numOfScores, vector<double>& bestScores)
{
	size_t numOfScoresToReturn = (numOfScores > m_Scores_seconds.size()) ? m_Scores_seconds.size() : numOfScores;
	
	auto setIt = m_Scores_seconds.begin();
	for (int i = 0; i < numOfScoresToReturn; i++, setIt++)
	{
		bestScores.push_back(*setIt);
	}

	return numOfScoresToReturn;
}