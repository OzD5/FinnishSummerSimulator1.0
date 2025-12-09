#include "Save.h"
unsigned int Save::highscore = 0;
const std::string Save::fileName = "Data/Saves/Save.txt";
const int Save::ENCRYPTIONKEY = 314159265;
const int Save::SECONDENCRYPTIONKEY = 27182818;

//Very Simple encryption. Won't be THAT easy to change Highscore
std::string Save::encrypt(const int& highscore, const int& encryptionKey)
{
	int xorScore = highscore ^ encryptionKey;
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

const int Save::decrypt(std::string& encryptedString, const int& encryptionKey)
{
	std::string xorString = "";
	for (char encryptedChr : encryptedString)
	{
		int tempDigit = encryptedChr - 91;
		char chrNum = tempDigit + '0';
		xorString += chrNum;
	}
	int decryptedScore = std::stoi(xorString);
	decryptedScore ^= encryptionKey;
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

	unsigned int checkSum = 0;
	bool zeroth = false;
	bool firstScore = false;
	while (std::getline(inputFile, line)) {

		std::string encryptedScore = line;
		if (line == "hs") {
			continue;
		}
		else if (!firstScore) {
			highscore = Save::decrypt(encryptedScore, ENCRYPTIONKEY);
			firstScore = true;
		}
		else {
			checkSum = Save::decrypt(encryptedScore, SECONDENCRYPTIONKEY);
		}
	}

	inputFile.close();
	//If scores don't match, we reset highscore
	if (highscore != checkSum) {
		highscore = 0;
	}
	return highscore;
}

void Save::updateHighscore(unsigned int newHighScore)
{
	if (newHighScore < highscore)
		return;
	//We return because no need to update

	std::string encryptedHighScore = Save::encrypt(newHighScore,ENCRYPTIONKEY);
	std::string encryptedVerifiedScore = Save::encrypt(newHighScore, SECONDENCRYPTIONKEY);
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
	bool highScorePlaced = false;
	bool verifiedScorePlaced = false;
	//If outputFile opens and 
	for (size_t i = 0; i < lines.size(); ++i) {
		if (lines[i] == "hs" && !highScorePlaced) {
			// Updating to the score after text highscore
			outputFile << "hs" << std::endl;
			++i;
			outputFile << encryptedHighScore << std::endl;
			highScorePlaced = true;
		}
		else if (highScorePlaced && !verifiedScorePlaced) {
			outputFile << encryptedVerifiedScore << std::endl;
			verifiedScorePlaced = true;
		}
		else {
			outputFile << lines[i] << std::endl;
		}
	}
}
