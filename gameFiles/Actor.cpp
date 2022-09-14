#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

#include <iostream>
#include <string>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size)
	:GraphObject(imageID, startX, startY, dir, depth, size), m_world(sw), m_alive(true), m_bonked(false)
{
}

Block::Block(StudentWorld* sw, int imageID, int startX, int startY)
	:Actor(sw, imageID, startX, startY, 0, 2, 1.0)
{
}

Pipe::Pipe(StudentWorld* sw, int startX, int startY)
	:Block(sw, IID_PIPE, startX, startY)
{
}

Specialblock::Specialblock(StudentWorld* sw, int startX, int startY)
	:Block(sw, IID_BLOCK, startX, startY), m_released(false)
{
}

Flowerblock::Flowerblock(StudentWorld* sw, int startX, int startY)
	:Specialblock(sw, startX, startY)
{
}

Mushroomblock::Mushroomblock(StudentWorld* sw, int startX, int startY)
	:Specialblock(sw, startX, startY)
{
}

Starblock::Starblock(StudentWorld* sw, int startX, int startY)
	:Specialblock(sw, startX, startY)
{
}

Goodie::Goodie(StudentWorld* sw, int imageID, int startX, int startY)
	:Actor(sw, imageID, startX, startY, 0, 1, 1.0)
{
}

Flower::Flower(StudentWorld* sw, int startX, int startY)
	:Goodie(sw, IID_FLOWER, startX, startY)
{
}

Mushroom::Mushroom(StudentWorld* sw, int startX, int startY)
	:Goodie(sw, IID_MUSHROOM, startX, startY)
{
}

Star::Star(StudentWorld* sw, int startX, int startY)
	:Goodie(sw, IID_STAR, startX, startY)
{
}

Projectile::Projectile(StudentWorld* sw, int imageID, int startx, int starty, int dir)
	:Actor(sw, imageID, startx, starty, dir, 1, 1.0)
{
}

PeachFireball::PeachFireball(StudentWorld* sw, int startx, int starty, int dir)
	:Projectile(sw, IID_PEACH_FIRE, startx, starty, dir)
{
}

PiranhaFireball::PiranhaFireball(StudentWorld* sw, int startx, int starty, int dir)
	:Projectile(sw, IID_PIRANHA_FIRE, startx, starty, dir)
{
}

Shell::Shell(StudentWorld* sw, int startx, int starty, int dir)
	:Projectile(sw, IID_SHELL, startx, starty, dir)
{
}

Enemies::Enemies(StudentWorld* sw, int imageID, int startx, int starty)
	:Actor(sw, imageID, startx, starty, randInt(0, 1) * 180, 0, 1.0)
{
}

Goomba::Goomba(StudentWorld* sw, int startx, int starty)
	:Enemies(sw, IID_GOOMBA, startx, starty)
{
}

Koopa::Koopa(StudentWorld* sw, int startx, int starty)
	:Enemies(sw, IID_KOOPA, startx, starty)
{
}

Piranha::Piranha(StudentWorld* sw, int startx, int starty)
	:Enemies(sw, IID_PIRANHA, startx, starty), m_firingdelay(0)
{
}

LevelEnder::LevelEnder(StudentWorld* sw, int imageID, int startX, int startY)
	:Actor(sw, imageID, startX, startY, 0, 1, 1.0)
{
}

Flag::Flag(StudentWorld* sw, int startX, int startY)
	:LevelEnder(sw, IID_FLAG, startX, startY)
{
}

Mario::Mario(StudentWorld* sw, int startX, int startY)
	:LevelEnder(sw, IID_MARIO, startX, startY)
{
}


Peach::Peach(StudentWorld* sw, int startX, int startY)
	:Actor(sw, IID_PEACH, startX, startY, 0, 0, 1.0), m_shootpower(false), m_mushpower(false), m_starpower(false), m_countrecharge(0), m_remainingtimejump(8)
{
	m_invinticks = 0;

	m_hitpoints = 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Actor::doSomething() { return; }
void Actor::bonk(Actor* peach) { return; }
bool Actor::addGoodie(int x, int y, Actor*& goodie) { return false; }

bool Actor::allowmoveTo() { return true; }
bool Block::allowmoveTo() { return false; }

bool Actor::bonkable() { return true; }
bool PeachFireball::bonkable() { return false; }
bool Shell::bonkable() { return false; }


StudentWorld* Actor::getsw() const
{
	return m_world;
}

void Actor::setDead() { m_alive = false; }
bool Actor::getAlive() const { return m_alive; }
void Actor::setBonk() { m_bonked = true; }
bool Actor::getBonk() const { return m_bonked; }

void Actor::setRel() { return; }
bool Actor::getRel() const { return false; }
void Specialblock::setRel() { m_released = true; }
bool Specialblock::getRel() const { return m_released; }

void Actor::inchitpoints() { return; }
void Actor::dechitpoints() { return; }
int Actor::gethitpoints() const { return -1; }
void Peach::inchitpoints() { m_hitpoints++; }
void Peach::dechitpoints() { m_hitpoints--; }
int Peach::gethitpoints() const { return m_hitpoints; }

void Actor::setShoot(bool set) { return; }
void Actor::setJump(bool set) { return; }
void Actor::setStar(bool set) { return; }
bool Actor::getShoot() const { return false; }
bool Actor::getJump() const { return false; }
bool Actor::getStar() const { return false; }

void Peach::setShoot(bool set) { m_shootpower = set; }
void Peach::setJump(bool set) { m_mushpower = set; }
void Peach::setStar(bool set) { m_starpower = set; }
bool Peach::getShoot() const { return m_shootpower; }
bool Peach::getJump() const { return m_mushpower; }
bool Peach::getStar() const { return m_starpower; }

void Actor::setInvinTicks(int set) { return; }
void Actor::decInvinTicks() { return; }
int  Actor::getInvinTicks() const { return 0; }
void Peach::setInvinTicks(int set) { m_invinticks = set; }
void Peach::decInvinTicks() { m_invinticks--; }
int  Peach::getInvinTicks() const { return m_invinticks; }

void Block::bonk(Actor* peach)
{
	this->getsw()->playSound(SOUND_PLAYER_BONK);
}

bool Flowerblock::addGoodie(int x, int y, Actor* &goodie)
{
	goodie = new Flower(this->getsw(), x, y + 8.0);
	return true;
}

bool Mushroomblock::addGoodie(int x, int y, Actor*& goodie)
{
	goodie = new Mushroom(this->getsw(), x, y + 8.0);
	return true;
}

bool Starblock::addGoodie(int x, int y, Actor*& goodie)
{
	goodie = new Star(this->getsw(), x, y + 8.0);
	return true;
}

void Specialblock::bonk(Actor* peach)
{
	if (!this->getBonk()) {
		this->getsw()->playSound(SOUND_POWERUP_APPEARS);
		this->setBonk();
	}
	else
		this->getsw()->playSound(SOUND_PLAYER_BONK);
}

void Flower::bonk(Actor* peach)
{
	this->getsw()->increaseScore(50);
	this->getsw()->playSound(SOUND_PLAYER_POWERUP);

	peach->setShoot(true);
	if (peach->gethitpoints() != 2) {
		peach->inchitpoints();
	}
		

	this->setDead();

	return;
}

void Mushroom::bonk(Actor* peach)
{
	this->getsw()->increaseScore(75);
	this->getsw()->playSound(SOUND_PLAYER_POWERUP);

	peach->setJump(true);
	if (peach->gethitpoints() != 2)
		peach->inchitpoints();

	this->setDead();

	return;
}

void Star::bonk(Actor* peach)
{
	peach->setInvinTicks(150);
	this->getsw()->increaseScore(100);
	this->getsw()->playSound(SOUND_PLAYER_POWERUP);

	peach->setStar(true);
	if (peach->gethitpoints() != 2)
		peach->inchitpoints();

	this->setDead();
}

void PeachFireball::bonk(Actor* peach)
{
	this->setDead();
	return;
}

void PiranhaFireball::bonk(Actor* peach)
{
	this->setDead();
	return;
}

void Shell::bonk(Actor* peach)
{
	this->setDead();
	return;
}

void Goomba::bonk(Actor* peach)
{
	this->getsw()->increaseScore(100);
	this->setDead();
	return;
}

void Koopa::bonk(Actor* peach)
{
	this->getsw()->increaseScore(100);
	this->setDead();
	this->getsw()->spawnShell(this);
	return;
}

void Piranha::bonk(Actor* peach)
{

	this->getsw()->increaseScore(100);
	this->setDead();

	return;
}

void Flag::bonk(Actor* peach)
{
	this->getsw()->increaseScore(1000);
	this->setDead();

	
	this->getsw()->setcompleted(true);
}

void Mario::bonk(Actor* peach)
{
	this->getsw()->increaseScore(1000);
	this->setDead();

	this->getsw()->setfinished();
}

void Peach::bonk(Actor* peach)
{
	//enemy bonked her
	
	if (this->getShoot() && !this->getStar())
		this->setShoot(false);
	if (this->getJump() && !this->getStar())
		this->setJump(false);

	if (this->getStar()) {
		if (this->getsw()->peachOverlaps(KEY_PRESS_RIGHT) || this->getsw()->peachOverlaps(KEY_PRESS_LEFT) || this->getsw()->peachOverlaps(KEY_PRESS_UP) || this->getsw()->peachOverlaps(KEY_PRESS_DOWN)) {
			this->getsw()->playSound(SOUND_PLAYER_KICK);
			return;
		}
	}

	if (peach->getInvinTicks() <= 0) {
		peach->setInvinTicks(10);
		this->getsw()->playSound(SOUND_PLAYER_HURT);
		m_hitpoints--;
	}
	return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Block::doSomething()
{
	return;
}
void Peach::doSomething()
{
	m_countrecharge--;

	if (!this->getsw()->peachOverlaps(KEY_PRESS_DOWN) && m_remainingtimejump == 0) {
		this->moveTo(this->getX() + 0.0, this->getY() - 4.0);
	}
	else {
		if (!this->getsw()->peachOverlaps(KEY_PRESS_DOWN) && m_remainingtimejump > 0) {
			if (!this->getsw()->peachOverlaps(KEY_PRESS_UP)) {

				this->moveTo(this->getX() + 0.0, this->getY() + 4.0);
				m_remainingtimejump--;
			}
			else {
				m_remainingtimejump = 0;
			}
		}
	}

	int key;

	if (this->getsw()->getKey(key)) {
		switch (key)
		{
		case KEY_PRESS_LEFT:
			if (!this->getsw()->peachOverlaps(KEY_PRESS_LEFT)) {
				this->setDirection(180);
				this->moveTo(this->getX() - 4.0, this->getY() + 0.0);
			}
			break;
		case KEY_PRESS_RIGHT:	
			if (!this->getsw()->peachOverlaps(KEY_PRESS_RIGHT)) {
				this->setDirection(0);
				this->moveTo(this->getX() + 4.0, this->getY() + 0.0);
			}	
			break;
		case KEY_PRESS_UP:
			if (m_remainingtimejump == 0 && this->getsw()->peachOverlaps(KEY_PRESS_DOWN)) {
				if (m_mushpower)
					m_remainingtimejump = 12;
				else
					m_remainingtimejump = 8;
			}

			if (this->getsw()->peachOverlaps(KEY_PRESS_DOWN) && m_remainingtimejump > 0) {
				if (!this->getsw()->peachOverlaps(KEY_PRESS_UP)) {
					this->getsw()->playSound(SOUND_PLAYER_JUMP);
					this->moveTo(this->getX() + 0.0, this->getY() + 4.0);
					m_remainingtimejump--;
				}
				else {
					m_remainingtimejump = 0;
				}
			}

			break;
		case KEY_PRESS_SPACE:
			if (m_shootpower) {
				if (m_countrecharge <= 0) {
					this->getsw()->playSound(SOUND_PLAYER_FIRE);
					m_countrecharge = 8;
					this->getsw()->spawnPeachFireball();
				}
			}
			break;
		default:
			break;
		}
	}

	return;
}

void Goodie::doSomething()
{
	this->getsw()->goodieMovementoverlap(this, KEY_PRESS_UP);

	if (!this->getsw()->goodieMovementoverlap(this, KEY_PRESS_DOWN)) {
		this->moveTo(this->getX(), this->getY() - 2.0);
	}
	
	if (this->getDirection() == 0) {
		if (!this->getsw()->goodieMovementoverlap(this, KEY_PRESS_RIGHT)) {
			this->moveTo(this->getX() + 2.0, this->getY());
		}
		else {
			if (this->getsw()->goodieMovementoverlap(this, KEY_PRESS_RIGHT)) {
				this->setDirection(180);
			}
		}
	}
	
	else if (this->getDirection() == 180) {
		if (!this->getsw()->goodieMovementoverlap(this, KEY_PRESS_LEFT)) {
			this->moveTo(this->getX() - 2.0, this->getY());
		}
		else {
			if (this->getsw()->goodieMovementoverlap(this, KEY_PRESS_LEFT)) {
				this->setDirection(0);
			}
		}
	}
	
	return;
}

void PeachFireball::doSomething()
{
	if (this->getsw()->friendlyProjectileOverlap(this)) {
		this->bonk(this);
	}

	if (!this->getsw()->elsebottomoverlap(this))
		this->moveTo(this->getX(), this->getY() - 2.0);

	if (this->getDirection() == 0) {
		if (!this->getsw()->elserightoverlap(this))
			this->moveTo(this->getX() + 2.0, this->getY());
		else {
			this->bonk(this);
		}
	}

	if (this->getDirection() == 180) {
		if (!this->getsw()->elseleftoverlap(this))
			this->moveTo(this->getX() - 2.0, this->getY());
		else {
			this->bonk(this);
		}
	}

	return;
}

void PiranhaFireball::doSomething()
{
	if (this->getsw()->enemyOverlap(this)) {
		this->bonk(this);
	}
	
	if (!this->getsw()->elsebottomoverlap(this))
		this->moveTo(this->getX(), this->getY() - 2.0);

	if (this->getDirection() == 0) {
		if (!this->getsw()->elserightoverlap(this))
			this->moveTo(this->getX() + 2.0, this->getY());
		else {
			this->bonk(this);
		}
	}

	if (this->getDirection() == 180) {
		if (!this->getsw()->elseleftoverlap(this))
			this->moveTo(this->getX() - 2.0, this->getY());
		else {
			this->bonk(this);
		}
	}

	return;
}

void Shell::doSomething()
{
	if (this->getsw()->friendlyProjectileOverlap(this)) {
		this->bonk(this);
	}

	if (!this->getsw()->elsebottomoverlap(this))
		this->moveTo(this->getX(), this->getY() - 2.0);

	if (this->getDirection() == 0) {
		if (!this->getsw()->elserightoverlap(this))
			this->moveTo(this->getX() + 2.0, this->getY());
		else {
			this->bonk(this);
		}
	}

	if (this->getDirection() == 180) {
		if (!this->getsw()->elseleftoverlap(this))
			this->moveTo(this->getX() - 2.0, this->getY());
		else {
			this->bonk(this);
		}
	}
}

void Piranha::doSomething()
{
	this->increaseAnimationNumber();

	this->getsw()->enemyOverlap(this);

	int xDiff;
	if (this->getsw()->peachTargetingInfo(this, 1.5, xDiff)) {
		if (xDiff > 0)
			this->setDirection(0);
		else
			this->setDirection(180);
	}

	if (m_firingdelay <= 0) {
		if (abs(xDiff) <= 8 * SPRITE_WIDTH) {
			this->getsw()->spawnPiranhaFireball(this);
			this->getsw()->playSound(SOUND_PIRANHA_FIRE);
			m_firingdelay = 40;
		}
	}
	else
		m_firingdelay--;
		
	return;
}

void Goomba::doSomething()
{
	this->getsw()->enemyOverlap(this);

	if (this->getsw()->enemydropTraversable(this)) {
		if (this->getDirection() == 0) {
			if (!this->getsw()->elserightoverlap(this))
				this->moveTo(this->getX() + 1, this->getY());
			else
				this->setDirection(180);
		}
		else {
			if (!this->getsw()->elseleftoverlap(this))
				this->moveTo(this->getX() - 1, this->getY());
			else 
				this->setDirection(0);
		}	
	}
	else {
		if (this->getDirection() == 0) {
			this->setDirection(180);
		}
		else {
			this->setDirection(0);
		}
	}
	return;
}

void Koopa::doSomething()
{
	this->getsw()->enemyOverlap(this);

	if (this->getsw()->enemydropTraversable(this)) {
		if (this->getDirection() == 0) {
			if (!this->getsw()->elserightoverlap(this))
				this->moveTo(this->getX() + 1, this->getY());
			else
				this->setDirection(180);
		}
		else {
			if (!this->getsw()->elseleftoverlap(this))
				this->moveTo(this->getX() - 1, this->getY());
			else
				this->setDirection(0);
		}
	}
	else {
		if (this->getDirection() == 0) {
			this->setDirection(180);
		}
		else {
			this->setDirection(0);
		}
	}
	return;
}

void LevelEnder::doSomething()
{
	if (this->getsw()->peachFlagOverlap(this))
		this->bonk(this);
}