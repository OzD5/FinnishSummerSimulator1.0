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
	
public:
	static unsigned int getHighscore();
	static void updateHighscore(unsigned int newHighScore);
	static std::string encrypt(const int& highscore);
	static const int decrypt(std::string& encryptedString);
};	