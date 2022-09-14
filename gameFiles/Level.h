#ifndef LEVEL_H_
#define LEVEL_H_

#include "GameConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

class Level
{
public:
	enum GridEntry {
		empty, peach, koopa, goomba, piranha, block, star_goodie_block,
		mushroom_goodie_block, flower_goodie_block, pipe, flag, mario
	};
	enum LoadResult {
		load_success, load_fail_file_not_found, load_fail_bad_format
	};

	Level(std::string assetPath)
	 : m_pathPrefix(assetPath)
	{
		for (int i = 0; i < GRID_HEIGHT; i++)
			for (int j = 0; j < GRID_WIDTH; j++)
				m_grid[i][j] = empty;

		if (!m_pathPrefix.empty())
			m_pathPrefix += '/';
	}

	LoadResult loadLevel(std::string filename)
	{
		std::ifstream levelFile(m_pathPrefix + filename);
		if (!levelFile)
			return load_fail_file_not_found;

		  // get the grid

		std::string line;
		int numPeach = 0;
		int numMario = 0;
		bool foundFlag = false;

		for (int gy = GRID_HEIGHT-1; std::getline(levelFile, line); gy--)
		{
			if (gy < 0)	// too many grid lines?
			{
				if (line.find_first_not_of(" \t\r") != std::string::npos)
					return load_fail_bad_format;  // non-blank line
				char dummy;
				if (levelFile >> dummy)  // non-blank rest of file
					return load_fail_bad_format;
				break;
			}
			if (line.size() < GRID_WIDTH  ||
					line.find_first_not_of(" \t\r", GRID_WIDTH) != std::string::npos)
				return load_fail_bad_format;
			for (int gx = 0; gx < GRID_WIDTH; gx++)
			{
				GridEntry ge;
				switch (toupper(line[gx]))
				{
					default:	return load_fail_bad_format;
					case ' ':	ge = empty; break;
					case '@':	ge = peach; numPeach++; break;
					case 'G':	ge = goomba; break;
					case 'K':	ge = koopa; break;
					case 'P':	ge = piranha; break;
					case '#':	ge = block; break;
					case '*':	ge = star_goodie_block; break;
					case '^':	ge = mushroom_goodie_block; break;
					case '%':	ge = flower_goodie_block; break;
					case 'I':	ge = pipe; break;
					case 'F':	ge = flag; foundFlag = true; break;
					case 'M':	ge = mario; numMario++; break;
				}
				m_grid[gy][gx] = ge;
			}
		}
		if (numPeach != 1  ||  numMario > 1  ||  (numMario == 1) == foundFlag)
			return load_fail_bad_format;

		  // edges must be blocks

		for (int gy = 0; gy < GRID_HEIGHT; gy++)
			if (m_grid[gy][0] != block  ||  m_grid[gy][GRID_WIDTH-1] != block)
				return load_fail_bad_format;

		for (int gx = 0; gx < GRID_WIDTH; gx++)
			if (m_grid[0][gx] != block  ||  m_grid[GRID_HEIGHT-1][gx] != block)
				return load_fail_bad_format;

		return load_success;
	}

	GridEntry getContentsOf(int gx, int gy)
	{
		if (gx < 0  ||  gx >= GRID_WIDTH  ||  gy < 0  ||  gy >= GRID_HEIGHT)
			return empty;

		return m_grid[gy][gx];
	}

private:
	GridEntry   m_grid[GRID_HEIGHT][GRID_WIDTH];  // indexed by [gy][gx]
	std::string m_pathPrefix;
};

#endif // LEVEL_H_
