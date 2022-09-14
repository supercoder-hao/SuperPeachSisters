#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    :GameWorld(assetPath), m_peach(nullptr), m_levelcompleted(false), m_finished(false)
{
}

StudentWorld::~StudentWorld()
{
    this->cleanUp();
}

int StudentWorld::init()   
{
    string level_file;
    ostringstream oss;
    if (this->getLevel()/10 > 0)
        oss << "level" << this->getLevel() << ".txt";
    else
        oss << "level0" << this->getLevel() << ".txt";
    level_file = oss.str();

    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            Level::GridEntry ge;

            ge = lev.getContentsOf(i, j);
            switch (ge)
            {
            case Level::empty:
                break;
            case Level::peach:
                m_peach = new Peach(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                m_list.push_front(m_peach);
                break;
            case Level::block:
                m_list.push_back(new Block(this, IID_BLOCK, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::pipe:
                m_list.push_back(new Pipe(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::flower_goodie_block:
                m_list.push_back(new Flowerblock(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::mushroom_goodie_block:
                m_list.push_back(new Mushroomblock(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::star_goodie_block:
                m_list.push_back(new Starblock(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::piranha:
                m_list.push_back(new Piranha(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::goomba:
                m_list.push_back(new Goomba(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::koopa:
                m_list.push_back(new Koopa(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::flag:
                m_list.push_back(new Flag(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;
            case Level::mario:
                m_list.push_back(new Mario(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                break;

            default:
                break;
            }

        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (m_peach->getInvinTicks() > 0)
        m_peach->decInvinTicks();

    if (m_peach->getInvinTicks() <= 0 && m_peach->getStar())
        m_peach->setStar(false);

    if (m_levelcompleted) {
        m_levelcompleted = false;
        this->playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if (m_finished) {
        return GWSTATUS_PLAYER_WON;
    }
        
    if (m_peach->gethitpoints() < 1) {
        this->decLives();
        this->playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }

    ostringstream oss;
    string hasShot = "";
    string hasJump = "";
    string hasStar = "";

    hasShot = (m_peach->getShoot()) ? " ShootPower!" : "";
    hasJump = (m_peach->getJump()) ? " JumpPower!" : "";
    hasStar = (m_peach->getStar()) ? " StarPower!" : "";

    oss << "Lives: " << this->getLives() << "  Level: " << this->getLevel() << "  Points: " << this->getScore() << hasStar << hasJump << hasShot;

    string s = oss.str();
    setGameStatText(s);

    list<Actor*>::iterator it;
    it = m_list.begin();
    while (it != m_list.end()) {
        if (!(*it)->getAlive()) {
            delete *it;
            m_list.remove(*it);
            return GWSTATUS_CONTINUE_GAME;
        }
            
        this->bonked(*it);
        (*it)->doSomething();
        it++;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while (!m_list.empty()) delete m_list.front(), m_list.pop_front();      //delete pointer information and delete pointer itself
}

void StudentWorld::setcompleted(bool completed)
{
    m_levelcompleted = completed;
}

void StudentWorld::setfinished()
{
    m_finished = true;
}


bool StudentWorld::overlapYSides(Actor* actor1, Actor* actor2)
{
    if (actor1->getY() + SPRITE_HEIGHT / 2 == actor2->getY())
        return true;
    else if (actor1->getY() + SPRITE_HEIGHT / 4 == actor2->getY())
        return true;
    else if (actor1->getY() == actor2->getY())
        return true;
    else if (actor1->getY() == actor2->getY() + SPRITE_HEIGHT / 2)
        return true;
    else if (actor1->getY() == actor2->getY() + SPRITE_HEIGHT / 4)
        return true;
    else
        return false;
}

bool StudentWorld::peachOverlaps(int dir)
{
    list<Actor*>::iterator it;
    it = m_list.begin();
    Actor* peach = (*it);
    it++;

    while (it != m_list.end()) {
        if (dir == KEY_PRESS_LEFT) {
            if ((*it)->getX() + SPRITE_WIDTH == peach->getX()) {
                if (overlapYSides(*it, peach)) {
                    if (m_peach->getStar() && (*it)->bonkable())
                        (*it)->bonk(m_peach);

                    if (!(*it)->allowmoveTo() && (*it)->bonkable()) {
                        (*it)->bonk(m_peach);
                        
                    }
                    return true;
                        
                }
            }  
        }
        else if (dir == KEY_PRESS_RIGHT) {
            if ((*it)->getX() == peach->getX() + SPRITE_WIDTH ) {
                if (overlapYSides(*it, peach)) {
                    if (m_peach->getStar() && (*it)->bonkable())
                        (*it)->bonk(m_peach);

                    if (!(*it)->allowmoveTo() && (*it)->bonkable()) {
                        (*it)->bonk(m_peach);

                    }
                    return true;
                }
            }
        }
        else if (dir == KEY_PRESS_UP) {
            if ((*it)->getY() == peach->getY() + SPRITE_HEIGHT) {
                if ((*it)->getX() == peach->getX() || (*it)->getX() + SPRITE_WIDTH / 2 == peach->getX() || (*it)->getX() == peach->getX() + SPRITE_WIDTH / 2) {
                    if (m_peach->getStar() && (*it)->bonkable())
                        (*it)->bonk(m_peach);

                    if (!(*it)->allowmoveTo() && (*it)->bonkable()) {
                        (*it)->bonk(m_peach);

                    }
                    return true;
                }
                
            }
        }
        else if (dir == KEY_PRESS_DOWN) {


            if ((*it)->getY() + SPRITE_HEIGHT == peach->getY() || (*it)->getY() + SPRITE_HEIGHT == (peach->getY() - 1.0) || (*it)->getY() + SPRITE_HEIGHT == (peach->getY() - 2.0) || (*it)->getY() + SPRITE_HEIGHT == (peach->getY() - 3.0)) {
                if ((*it)->getX() == peach->getX() || (*it)->getX() + SPRITE_WIDTH/2 == peach->getX() || (*it)->getX() == peach->getX() + SPRITE_WIDTH / 2) {

                    if (!(*it)->allowmoveTo())
                        return true;

                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::peachFlagOverlap(Actor* flag)
{
    for (int i = 0; i < SPRITE_HEIGHT; i++) {
        if (flag->getX() == m_peach->getX() + SPRITE_WIDTH) {
            if (flag->getY() == m_peach->getY() || flag->getY() + i == m_peach->getY() || flag->getY() == m_peach->getY() + i) {
                return true;
            }
        }
        if (flag->getX() + SPRITE_WIDTH == m_peach->getX()) {
            if (flag->getY() == m_peach->getY() || flag->getY() + i == m_peach->getY() || flag->getY() == m_peach->getY() + i) {
                return true;
            }
        }
    }
    for (int i = 0; i < SPRITE_WIDTH; i++) {
        if (flag->getY() + SPRITE_HEIGHT == m_peach->getY()) {
            if (flag->getX() == m_peach->getX() || flag->getX() + i == m_peach->getX() || flag->getX() == m_peach->getX() + i) {
                return true;
            }
        }
        if (flag->getY() == m_peach->getY() + SPRITE_HEIGHT) {
            if (flag->getX() == m_peach->getX() || flag->getX() + i == m_peach->getX() || flag->getX() == m_peach->getX() + i) {
                return true;
            }
        }

    }
    return false;
}

bool StudentWorld::goodieMovementoverlap(Actor* goodie, int dir)
{
    list<Actor*>::iterator it;
    it = m_list.begin();

    while (it != m_list.end()) {
        if ((*it) != goodie) {
            if (dir == KEY_PRESS_DOWN) {
                for (int i = 0; i < SPRITE_WIDTH; i++) {
                    if ((*it)->getY() + SPRITE_HEIGHT == goodie->getY()) {
                        if ((*it)->getX() == goodie->getX() || (*it)->getX() + i == goodie->getX() || (*it)->getX() == goodie->getX() + i) {
                            if (m_peach->getY() + SPRITE_HEIGHT == goodie->getY()) {
                                if (m_peach->getX() == goodie->getX() || m_peach->getX() + i == goodie->getX() || m_peach->getX() == goodie->getX() + i) {
                                    goodie->bonk(m_peach);
                                }
                            }
                            if (!(*it)->allowmoveTo())
                                return true;  
                        } 
                    }
                }
            }
            else if (dir == KEY_PRESS_UP) {
                for (int i = 0; i < SPRITE_WIDTH; i++) {
                    if ((*it)->getY() == goodie->getY() + SPRITE_HEIGHT) {
                        if ((*it)->getX() == goodie->getX() || (*it)->getX() + i == goodie->getX() || (*it)->getX() == goodie->getX() + i) {
                            if (m_peach->getY()== goodie->getY() + SPRITE_HEIGHT) {
                                if (m_peach->getX() == goodie->getX() || m_peach->getX() + i == goodie->getX() || m_peach->getX() == goodie->getX() + i) {
                                    goodie->bonk(m_peach);
                                }
                            }
                            if (!(*it)->allowmoveTo())
                                return true;
                        }
                    }
                }
            }

            else if (dir == KEY_PRESS_RIGHT) {
                for (int i = 0; i < SPRITE_HEIGHT; i++) {
                    if ((*it)->getX() == goodie->getX() + SPRITE_WIDTH) {
                        if ((*it)->getY() == goodie->getY() || (*it)->getY() + i == goodie->getY() || (*it)->getY() == goodie->getY() + i) {
                            if (m_peach->getX() == goodie->getX() + SPRITE_WIDTH) {
                                if (m_peach->getY() == goodie->getY() || m_peach->getY() + i == goodie->getY() || m_peach->getY() == goodie->getY() + i) {
                                    goodie->bonk(m_peach);
                                }
                            }
                            if (!(*it)->allowmoveTo())
                                return true;
                        }
                    }
                    
                }
            }
            else if (dir == KEY_PRESS_LEFT) {
                for (int i = 0; i < SPRITE_HEIGHT; i++) {
                    if ((*it)->getX() + SPRITE_WIDTH == goodie->getX()) {
                        if ((*it)->getY() == goodie->getY() || (*it)->getY() + i == goodie->getY() || (*it)->getY() == goodie->getY() + i) {
                            if ((m_peach)->getX() + SPRITE_WIDTH == goodie->getX()) {
                                if (m_peach->getY() == goodie->getY() || m_peach->getY() + i == goodie->getY() || m_peach->getY() == goodie->getY() + i) {
                                    goodie->bonk(m_peach);
                                }
                            }
                            if (!(*it)->allowmoveTo())
                                return true;
                        }
                    }
                    
                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::elserightoverlap(Actor* actor)
{
    list<Actor*>::iterator it;
    it = m_list.begin();

    while (it != m_list.end()) {
        if (*it != actor) {
            if ((*it)->getX() == actor->getX() + SPRITE_WIDTH) {
                if (overlapYSides(*it, actor)) {

                    if (!(*it)->allowmoveTo())
                        return true;
                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::elseleftoverlap(Actor* actor)
{
    list<Actor*>::iterator it;
    it = m_list.begin();
    it++;

    while (it != m_list.end()) {
        if (*it != actor) {
            if ((*it)->getX() + SPRITE_WIDTH == actor->getX()) {
                if (overlapYSides(*it, actor)) {
                    if (!(*it)->allowmoveTo())
                        return true;
                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::elsebottomoverlap(Actor* actor)
{
    list<Actor*>::iterator it;
    it = m_list.begin();
    it++;

    while (it != m_list.end()) {
        for (int i = 0; i < SPRITE_WIDTH; i++) {
            if (*it != actor) {
                if ((*it)->getY() + SPRITE_HEIGHT == actor->getY()) {
                    //right drop
                    if (actor->getDirection() == 0) {
                        if (actor->getX() + SPRITE_WIDTH + 1 == (*it)->getX() + i) {
                            if (!(*it)->allowmoveTo())
                                return true;
                        }  
                    }
                    //left drop
                    else {
                        if (actor->getX() - 1 == (*it)->getX() + i) {
                            if (!(*it)->allowmoveTo())
                                return true;
                        }
                    }
                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::elsetopoverlap(Actor* actor)
{
    list<Actor*>::iterator it;
    it = m_list.begin();
    it++;

    while (it != m_list.end()) {
        for (int i = 0; i < SPRITE_WIDTH; i++) {
            if ((*it) != actor) {
                if ((*it)->getY() == actor->getY() + SPRITE_HEIGHT) {
                    //right drop
                    if (actor->getDirection() == 0) {
                        if (actor->getX() + SPRITE_WIDTH + 1 == (*it)->getX() + i) {
                            if (!(*it)->allowmoveTo())
                                return true;
                        }
                    }
                    //left drop
                    else {
                        if (actor->getX() - 1 == (*it)->getX() + i) {
                            if (!(*it)->allowmoveTo())
                                return true;
                        }
                    }
                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::enemyOverlap(Actor* actor)//enemy checks for peach
{
    if ((m_peach)->getX() == actor->getX() + SPRITE_WIDTH) {
        if (overlapYSides(m_peach, actor)) {
            m_peach->bonk(m_peach);
            return true;
        }
    }
    if ((m_peach)->getX() + SPRITE_WIDTH == actor->getX()) {
        if (overlapYSides(m_peach, actor)) {
            m_peach->bonk(m_peach);
            return true;
        }
    }

    if ((m_peach)->getY() + SPRITE_HEIGHT == actor->getY()) {
        if ((m_peach)->getX() == actor->getX() || (m_peach)->getX() + SPRITE_WIDTH / 2 == actor->getX() || (m_peach)->getX() == actor->getX() + SPRITE_WIDTH / 2) {
            m_peach->bonk(m_peach);
            return true;
        }
    }
    if ((m_peach)->getY() == actor->getY() + SPRITE_HEIGHT) {
        if ((m_peach)->getX() == actor->getX() || (m_peach)->getX() + SPRITE_WIDTH / 2 == actor->getX() || (m_peach)->getX() == actor->getX() + SPRITE_WIDTH / 2) {
            m_peach->bonk(m_peach);
            return true;
        }
    }

    return false;
}

bool StudentWorld::friendlyProjectileOverlap(Actor* fireball)
{


    list<Actor*>::iterator it;
    it = m_list.begin();
    it++;

    while (it != m_list.end()) {
        for (int i = 0; i < SPRITE_HEIGHT; i++) {
            if ((*it)->getX() == fireball->getX() + SPRITE_WIDTH) {
                if ((*it)->getY() == fireball->getY() || (*it)->getY() + i == fireball->getY() || (*it)->getY() == fireball->getY() + i) {
                    if ((*it)->allowmoveTo() && (*it)->bonkable()) {
                        (*it)->bonk(m_peach);
                        return true;
                    }
                }
            }

            if ((*it)->getX() + SPRITE_WIDTH == fireball->getX()) {
                if ((*it)->getY() == fireball->getY() || (*it)->getY() + i == fireball->getY() || (*it)->getY() == fireball->getY() + i) {
                    if ((*it)->allowmoveTo() && (*it)->bonkable()) {
                        (*it)->bonk(m_peach);
                        return true;
                    }
                }
            }
        }

        for (int i = 0; i < SPRITE_WIDTH; i++) {
            if ((*it)->getY() + SPRITE_HEIGHT == fireball->getY()) {
                if ((*it)->getX() == fireball->getX() || (*it)->getX() + i == fireball->getX() || (*it)->getX() == fireball->getX() + i) {
                    if ((*it)->allowmoveTo() && (*it)->bonkable()) {
                        (*it)->bonk(m_peach);
                        return true;
                    }
                }
            }

            if ((*it)->getY() == fireball->getY() + SPRITE_HEIGHT) {
                if ((*it)->getX() == fireball->getX() || (*it)->getX() + i == fireball->getX() || (*it)->getX() == fireball->getX() + i) {
                    if ((*it)->allowmoveTo() && (*it)->bonkable()) {
                        (*it)->bonk(m_peach);
                        return true;
                    }
                }
            }

        }


        it++;
    }
    return false;
}

bool StudentWorld::enemydropTraversable(Actor* enemy)
{
    list<Actor*>::iterator it;
    it = m_list.begin();

    while (it != m_list.end()) {
        for (int i = 0; i < SPRITE_WIDTH; i++) {
            if (*it != enemy) {
                if ((*it)->getY() + SPRITE_HEIGHT == enemy->getY()) {
                    //right drop
                    if (enemy->getDirection() == 0) {
                        if (enemy->getX() + SPRITE_WIDTH + 1 == (*it)->getX() + i) 
                            return true;
                    }
                    //left drop
                    else {
                        if (enemy->getX() - 1 == (*it)->getX() + i) 
                            return true;    
                    }
                }
            }
        }
        it++;
    }

    return false;
}


void StudentWorld::bonked(Actor* it)
{
    Actor* goodie = nullptr;

    if ((it)->getBonk() && !(it)->getRel()) {
        if ((it)->addGoodie((it)->getX(), (it)->getY(), goodie) != false) {
            it->setRel();
            m_list.push_back(goodie);
        }
    }
}



void StudentWorld::spawnPeachFireball() //creastes fireball with appropiate coordiantes
{
    if (m_peach->getDirection() == 0) {
        if (!elserightoverlap(m_peach))
            m_list.push_back(new PeachFireball(this, m_peach->getX() + SPRITE_WIDTH / 2 + 4, m_peach->getY(), m_peach->getDirection()));
    }
    else if (m_peach->getDirection() == 180) {
        if (!elseleftoverlap(m_peach))
            m_list.push_back(new PeachFireball(this, m_peach->getX() - 4 - SPRITE_WIDTH / 2, m_peach->getY(), m_peach->getDirection()));
    }
}

void StudentWorld::spawnPiranhaFireball(Actor* piranha) //creastes fireball with appropiate coordiantes
{
    m_list.push_back(new PiranhaFireball(this, piranha->getX(), piranha->getY(), piranha->getDirection()));
}

void StudentWorld::spawnShell(Actor* koopa) //creastes shell with appropiate coordiantes
{
    m_list.push_back(new Shell(this, koopa->getX(), koopa->getY(), koopa->getDirection()));
}

bool StudentWorld::peachTargetingInfo(Actor* a, double yDeltaLimit, int& xDeltaFromActor) const //piranaha AI
{
    if (m_peach->getY() <= a->getY() && m_peach->getY() >= a->getY() - (yDeltaLimit * SPRITE_HEIGHT)) {
        xDeltaFromActor = m_peach->getX() - a->getX();
        return true;
    }
    return false;
}