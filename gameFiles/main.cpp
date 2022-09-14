#include "GameController.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#ifdef _WIN32
#include <windows.h>
bool is_directory(string path)
{
    DWORD result = GetFileAttributesA(path.c_str());
    return result != INVALID_FILE_ATTRIBUTES  &&  (result & FILE_ATTRIBUTE_DIRECTORY);
}
#else
#include <sys/stat.h>
bool is_directory(string path)
{
    struct stat statbuf;
    return stat(path.c_str(), &statbuf) == 0  &&  S_ISDIR(statbuf.st_mode);
}
#endif

  // If your program is having trouble finding the Assets directory,
  // replace the string literal with a full path name to the directory,
  // e.g., "C:/CS32/SuperPeachSisters/Assets" or "/Users/fred/cs32/SuperPeachSisters/Assets"

const string assetDirectory = "Assets";

class GameWorld;

GameWorld* createStudentWorld(string assetPath = "");

int main(int argc, char* argv[])
{
    string assetPath = assetDirectory;
    if (!assetPath.empty())
    {
        if (!is_directory(assetPath))
        {
            cout << "Cannot find directory " << assetPath << endl;
            return 1;
        }
        assetPath += '/';
    }
    {
		const string someAsset = "star.tga";
		ifstream ifs(assetPath + someAsset);
		if (!ifs)
		{
			cout << "Cannot find " << someAsset << " in ";
			cout << (assetDirectory.empty() ? "current directory" : assetDirectory) << endl;
			return 1;
		}
	}

	GameWorld* gw = createStudentWorld(assetPath);
	Game().run(argc, argv, gw, "Super Peach Sisters");
}
