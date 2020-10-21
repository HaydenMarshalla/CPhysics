#include "Cereal/archives/binary.hpp"

#include "Settings.h"

#include <fstream>
#include <sys/stat.h>

std::string fileName = "../CPhysics/testbed/settings.bin";
Settings settings;

unsigned int screenWidth = 1920;
unsigned int screenHeight = 1080;

//Loads settings via serializatin
void loadSettings(const std::string& fileName) {
	std::ifstream os(fileName, std::ios::binary);
	{
		cereal::BinaryInputArchive ar(os);
		ar(settings);
	}
}

//Saves settings via serializatin
void saveSettings(const std::string& fileName) {
	std::ofstream is(fileName, std::ios::binary);
	{
		cereal::BinaryOutputArchive ar(is);
		ar(settings);
	}
}

inline bool checkFileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main(void)
{
	if (checkFileExists(fileName)) {
		loadSettings(fileName);
	}

	saveSettings(fileName);

	return 0;
}
