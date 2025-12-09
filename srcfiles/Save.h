#pragma once
//Updating Highscore and getting highscore for everygame
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Save
{
private:
	static unsigned int highscore;
	static const std::string fileName;
	static const int ENCRYPTIONKEY;
	static const int SECONDENCRYPTIONKEY;
	
public:
	static unsigned int getHighscore();
	static void updateHighscore(unsigned int newHighScore);
	static std::string encrypt(const int& highscore, const int& encryptionKey);
	static const int decrypt(std::string& encryptedString, const int& encryptionKey);
};	