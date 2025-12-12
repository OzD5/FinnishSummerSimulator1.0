#include "Save.h"
#include "prints.cpp"
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
	int decryptedScore = 0;
	for (char encryptedChr : encryptedString)
	{
		int tempDigit = encryptedChr - 91;
		char chrNum = tempDigit + '0';
		xorString += chrNum;
	}
	try
	{
		decryptedScore = std::stoi(xorString);
	}
	catch (const std::exception&)
	{
	}
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
		std::ofstream temp(fileName);
		temp.close();
	}

	unsigned int checkSum = 0;
	bool zeroth = false;
	bool firstScore = false;
	bool secondScore = false;
	while (std::getline(inputFile, line)) {

		std::string encryptedScore = line;
		if (!firstScore) {
			highscore = Save::decrypt(encryptedScore, ENCRYPTIONKEY);
			firstScore = true;
		}
		else if (!secondScore) {
			checkSum = Save::decrypt(encryptedScore, SECONDENCRYPTIONKEY);
			secondScore = true;
		}
	}

	inputFile.close();
	using namespace OZ::PRINTS;
	print(highscore);
	print(checkSum);
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
	outputFile << encryptedHighScore << std::endl;
	outputFile << encryptedVerifiedScore << std::endl;
}
