#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

#include <random>
#include <utility>

// image IDs for the game objects

const int IID_PEACH = 0;
const int IID_KOOPA = 1;
const int IID_GOOMBA = 2;
const int IID_SHELL = 3;
const int IID_PIRANHA = 4;
const int IID_MARIO = 5;
const int IID_BLOCK = 6;
const int IID_PIPE = 7;
const int IID_STAR = 8;
const int IID_FLOWER = 9;
const int IID_MUSHROOM = 10;
const int IID_FLAG = 11;
const int IID_PIRANHA_FIRE = 12;
const int IID_PEACH_FIRE = 13;

// sounds

const int SOUND_PLAYER_DIE = 0;
const int SOUND_PLAYER_FIRE = 1;
const int SOUND_PLAYER_HURT = 2;
const int SOUND_PLAYER_JUMP = 3;
const int SOUND_PLAYER_BONK = 4;
const int SOUND_PLAYER_POWERUP = 5;
const int SOUND_POWERUP_APPEARS = 6;
const int SOUND_PLAYER_KICK = 7;
const int SOUND_FINISHED_LEVEL = 8;
const int SOUND_GAME_OVER = 9;
const int SOUND_PIRANHA_FIRE = 10;
const int SOUND_THEME = 11;
const int SOUND_NONE = -1;

// keys the user can hit

const int KEY_PRESS_LEFT   = 1000;
const int KEY_PRESS_RIGHT  = 1001;
const int KEY_PRESS_UP     = 1002;
const int KEY_PRESS_DOWN   = 1003;
const int KEY_PRESS_SPACE  = ' ';
const int KEY_PRESS_ESCAPE = '\x1b';
const int KEY_PRESS_TAB    = '\t';
const int KEY_PRESS_ENTER  = '\r';

// board and sprite dimensions

const int VIEW_WIDTH = 256;
const int VIEW_HEIGHT = 256;

const int SPRITE_WIDTH = 8;
const int SPRITE_HEIGHT = 8;

const int GRID_WIDTH = VIEW_WIDTH / SPRITE_WIDTH;
const int GRID_HEIGHT = VIEW_HEIGHT / SPRITE_HEIGHT;

const double SPRITE_WIDTH_GL = .3; // note - this is tied implicitly to SPRITE_WIDTH due to carey's sloppy openGL programming
const double SPRITE_HEIGHT_GL = .25; // note - this is tied implicitly to SPRITE_HEIGHT due to carey's sloppy openGL programming

// status of each tick (did the player die?)

const int GWSTATUS_PLAYER_DIED    = 0;
const int GWSTATUS_CONTINUE_GAME  = 1;
const int GWSTATUS_PLAYER_WON     = 2;
const int GWSTATUS_FINISHED_LEVEL = 3;
const int GWSTATUS_LEVEL_ERROR    = 4;

// test parameter constants

const int NUM_TEST_PARAMS = 1;

// Return a uniformly distributed random int from min to max, inclusive

inline
int randInt(int min, int max)
{
    if (max < min)
        std::swap(max, min);
    static std::random_device rd;
    static std::default_random_engine generator(rd());
    std::uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

#endif // GAMECONSTANTS_H_
