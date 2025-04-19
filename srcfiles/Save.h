#pragma once
//P‰ivitet‰‰n Highscorea yms. jos tulee
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Save
{
private:
	static unsigned int highscore;
	static const std::string fileName;
public:
	static unsigned int getHighscore();
	static void updateHighscore(unsigned int newHighScore);
};