#include "Save.h"
unsigned int Save::highscore = 0;
const std::string Save::fileName = "Data/Saves/Save.txt";


// TODO: YAMLI FILE
unsigned int Save::getHighscore()
{
	std::ifstream inputFile(fileName);
	std::string line;
	if (inputFile.is_open())
	{
		std::cerr << "Unable to open file 1: " << fileName << std::endl;
	}
	while (std::getline(inputFile, line)) {
		if (!(line == "highscore")) continue;
		// Get highscore from file
		if (std::getline(inputFile, line)) {
			try {
				highscore = std::stoi(line);
			}
			catch (const std::invalid_argument&) { highscore = 69; }
			catch (const std::out_of_range&) { highscore = 69; }
		}
		break;
	}

	inputFile.close();
	return highscore;
}

void Save::updateHighscore(unsigned int newHighScore)
{
	if (newHighScore > highscore)
	{
		highscore = newHighScore;
		std::ifstream inputFile(fileName);
		std::string line;
		std::vector<std::string> lines;
		if (inputFile.is_open())
			std::cerr << "Unable to open file 1: " << fileName << std::endl;
		while (std::getline(inputFile, line)) {
			lines.push_back(line);
		}
		inputFile.close();
		std::ofstream outputFile(fileName);
		if (outputFile.is_open()) {
			for (size_t i = 0; i < lines.size(); ++i) {
				if (lines[i] == "highscore") {
					// Päivitetään oikeaan kohtaan highscore
					outputFile << "highscore" << std::endl;
					++i;
					outputFile << highscore;
				}
				else {
					outputFile << lines[i] << std::endl;
				}
			}
		}
		else std::cerr << "Unable to write file 1: " << fileName << std::endl;
	}
}
