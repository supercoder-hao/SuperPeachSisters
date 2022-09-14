#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  void setcompleted(bool completed);
  void setfinished();

  bool peachOverlaps(int dir);
  bool peachFlagOverlap(Actor* flag);
  bool overlapYSides(Actor* actor1, Actor* actor2);
  bool goodieMovementoverlap(Actor* goodie, int dir);
  bool enemyOverlap(Actor* enemy);
  bool friendlyProjectileOverlap(Actor* fireball);
  bool enemydropTraversable(Actor* enemy);

  bool elserightoverlap(Actor* actor);					//an overlap for basic actors
  bool elseleftoverlap(Actor* actor);					//an overlap for basic actors
  bool elsebottomoverlap(Actor* actor);					//an overlap for basic actors
  bool elsetopoverlap(Actor* actor);					//an overlap for basic actors

  void bonked(Actor* it);
  void spawnPeachFireball();
  void spawnPiranhaFireball(Actor* piranha);
  void spawnShell(Actor* shell);

  bool peachTargetingInfo(Actor* a, double yDeltaLimit, int& xDeltaFromActor) const;

private:
	std::list<Actor*> m_list;
	Actor* m_peach;

	bool m_levelcompleted;
	bool m_finished;
};

#endif // STUDENTWORLD_H_
