#include "Save.h"
unsigned int Save::highscore = 0;
const std::string Save::fileName = "Data/Saves/Save.txt";
const int Save::ENCRYPTIONKEY = 314159265;


//Very Simple encryption. Won't be THAT easy to change Highscore
std::string Save::encrypt(const int& highscore)
{
	int xorScore = highscore ^ ENCRYPTIONKEY;
	std::string xorString = std::to_string(xorScore);
	std::string encryptedScore = "";
	for (char& chrNum : xorString)
	{
		int tempDigit = chrNum - '0';
		char digChr = 91 + tempDigit;
		encryptedScore += digChr;
	}
	return encryptedScore;
}

const int Save::decrypt(std::string& encryptedString)
{
	std::string xorString = "";
	for (char encryptedChr : encryptedString)
	{
		int tempDigit = encryptedChr - 91;
		char chrNum = tempDigit + '0';
		xorString += chrNum;
	}
	int decryptedScore = std::stoi(xorString);
	decryptedScore ^= ENCRYPTIONKEY;
	return decryptedScore;
}

// TODO: YAMLI FILE
unsigned int Save::getHighscore()
{
	std::ifstream inputFile(fileName);
	std::string line;
	if (!inputFile.is_open())
	{
		std::cerr << "Unable to open SAVE.TXT: " << fileName << std::endl;
		return 0;
	}
	while (std::getline(inputFile, line)) {
		if (!(line == "hs")) continue;
		// Get highscore from file
		//In case that after "hs", we can't get line
		if (!std::getline(inputFile, line)) 
			break;

		try {
			std::string encryptedScore = line;
			highscore = Save::decrypt(encryptedScore);
		}
		catch (const std::invalid_argument&) { highscore = 0; }
		catch (const std::out_of_range&) { highscore = 0; }
	}

	inputFile.close();
	return highscore;
}

void Save::updateHighscore(unsigned int newHighScore)
{
	if (newHighScore < highscore)
		return;
	//We return because no need to update

	std::string encryptedHighScore = Save::encrypt(newHighScore);

	std::ifstream inputFile(fileName);
	std::string line;
	std::vector<std::string> lines;
	if (!inputFile.is_open())
		std::cerr << "Unable to open SAVE.TXT: " << fileName << std::endl;

	while (std::getline(inputFile, line)) {
		lines.push_back(line);
	}
	inputFile.close();

	std::ofstream outputFile(fileName);
	if (!outputFile.is_open())
	{
		std::cerr << "Unable to write SAVE.TXT: " << fileName << std::endl;
		return;
	}
	//If outputFile opens and 
	for (size_t i = 0; i < lines.size(); ++i) {
		if (lines[i] == "hs") {
			// Updating to the score after text highscore
			outputFile << "hs" << std::endl;
			++i;
			outputFile << encryptedHighScore;
		}
		else {
			outputFile << lines[i] << std::endl;
		}
	}
}
