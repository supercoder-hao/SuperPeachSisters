#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* sw, int imageID, int startX, int startY, int dir, int depth, double size); 
	virtual void doSomething();
	virtual void bonk(Actor* peach);
	virtual bool addGoodie(int x, int y, Actor* &goodie);
	
	virtual bool allowmoveTo();	//only blocks block moveto by peach
	virtual bool bonkable();	//allows for specific actor to interactor or not interactor with peach

	StudentWorld* getsw() const;
	void setDead();
	bool getAlive() const;
	void setBonk();
	bool getBonk() const;

	virtual void setRel();
	virtual bool getRel() const;

	virtual void setShoot(bool set);
	virtual void setJump(bool set);
	virtual void setStar(bool set);
	virtual bool getShoot() const;
	virtual bool getJump() const;
	virtual bool getStar() const;

	virtual void setInvinTicks(int set);
	virtual void decInvinTicks();
	virtual int  getInvinTicks() const;

	virtual void inchitpoints();
	virtual void dechitpoints();
	virtual int gethitpoints() const;

private:
	StudentWorld* m_world;
	bool m_alive;
	bool m_bonked;
};


class Block : public Actor
{
public:
	Block(StudentWorld* sw, int imageID, int startX, int startY);
	virtual void doSomething();
	virtual void bonk(Actor* peach);
	virtual bool allowmoveTo();

private:
};

class Pipe : public Block
{
public :
	Pipe(StudentWorld* sw, int startX, int startY);
private:
};
//////////////////////////////////////////////////////////////////////////
class Specialblock : public Block
{
public:
	Specialblock(StudentWorld* sw, int startx, int starty);
	virtual void bonk(Actor* peach);
	virtual bool addGoodie(int x, int y, Actor*& goodie) = 0;

	virtual void setRel();
	virtual bool getRel() const;
private:
	bool m_released;
};

class Flowerblock : public Specialblock
{
public:
	Flowerblock(StudentWorld* sw, int startx, int starty);
	virtual bool addGoodie(int x, int y, Actor*& goodie);						
																
private:
};

class Mushroomblock : public Specialblock
{
public:
	Mushroomblock(StudentWorld* sw, int startx, int starty);
	virtual bool addGoodie(int x, int y, Actor*& goodie);

private:
};

class Starblock : public Specialblock
{
public:
	Starblock(StudentWorld* sw, int startx, int starty);
	virtual bool addGoodie(int x, int y, Actor*& goodie);

private:
};

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* sw, int imageID, int startx, int starty);
	virtual void doSomething();
private:
};

class Flower : public Goodie
{
public:
	Flower(StudentWorld* sw, int startx, int starty);
	virtual void bonk(Actor* peach);

private:
};

class Mushroom : public Goodie
{
public:
	Mushroom(StudentWorld* sw, int startx, int starty);
	virtual void bonk(Actor* peach);
private:
};

class Star : public Goodie
{
public:
	Star(StudentWorld* sw, int startx, int starty);
	virtual void bonk(Actor* peach);

private:
};

class Projectile : public Actor
{
public:
	Projectile(StudentWorld* sw, int imageID, int startx, int starty, int dir);
	virtual void doSomething() = 0;
private:
};

class PeachFireball : public Projectile
{
public:
	PeachFireball(StudentWorld* sw, int startx, int starty, int dir);
	virtual void bonk(Actor* peach);
	virtual void doSomething();
	virtual bool bonkable();
private:
};

class PiranhaFireball : public Projectile
{
public:
	PiranhaFireball(StudentWorld* sw, int startx, int starty, int dir);
	virtual void bonk(Actor* peach);
	virtual void doSomething();
private:
};

class Shell : public Projectile
{
public:
	Shell(StudentWorld* sw, int startx, int starty, int dir);
	virtual void bonk(Actor* peach);
	virtual void doSomething();
	virtual bool bonkable();
private:
};

class Enemies : public Actor
{
public:
	Enemies(StudentWorld* sw, int imageID, int startx, int starty);
private:
};

class Goomba : public Enemies
{
public:
	Goomba(StudentWorld* sw, int startx, int starty);
	virtual void doSomething();
	virtual void bonk(Actor* peach);
private:
};

class Koopa : public Enemies
{
public:
	Koopa(StudentWorld* sw, int startx, int starty);
	virtual void doSomething();
	virtual void bonk(Actor* peach);
private:
};

class Piranha : public Enemies
{
public:
	Piranha(StudentWorld* sw, int startx, int starty);
	virtual void doSomething();
	virtual void bonk(Actor* peach);

private:
	int m_firingdelay;
};

class LevelEnder : public Actor
{
public:
	LevelEnder(StudentWorld* sw, int imageID, int startX, int startY);
	virtual void doSomething();
};

class Flag : public LevelEnder
{
public:
	Flag(StudentWorld* sw, int startX, int startY);
	virtual void bonk(Actor* peach);
};

class Mario : public LevelEnder
{
public:
	Mario(StudentWorld* sw, int startX, int startY);
	virtual void bonk(Actor* peach);
};

//////////////////////////////////////////////////////////////////////////
class Peach : public Actor
{
public:
	Peach(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
	virtual void bonk(Actor* peach);

	virtual void setShoot(bool set);
	virtual void setJump(bool set);
	virtual void setStar(bool set);
	virtual bool getShoot() const;
	virtual bool getJump() const;
	virtual bool getStar() const;

	virtual void setInvinTicks(int set);
	virtual void decInvinTicks();
	virtual int  getInvinTicks() const;

	void inchitpoints();
	void dechitpoints();
	int  gethitpoints() const;

private:
	int m_countrecharge;
	int m_remainingtimejump;

	bool m_shootpower;
	bool m_starpower;
	bool m_mushpower;

	int m_hitpoints;

	int m_invinticks;
};

#endif // ACTOR_H_