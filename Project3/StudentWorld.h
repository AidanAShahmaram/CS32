#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp\


class Avatar;
class Actor;

class StudentWorld: public GameWorld{
public:
    StudentWorld(std::string assetPath);
    
    //make sure to implement a destructor
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    //checks if an actor, excluding the one given, is in the given coordinates
    bool actorCheck(int x, int y, Actor* curr);
    
    //checks if the avatar is in the given coordinates
    bool avatarCheck(int x, int y);
    
    
    //returns the actor with the same x and y value
    Actor* getActor(int x, int y);
    
    
    //returns the actor with the same x and y value, excluding the one that was passed
    Actor* getActor(int x, int y, Actor* curr);
    
    
    //returns the player object
    Avatar* getAvatar(){
        return m_avatar;
    }
    
    //gets goodie in the given coordinates
    //if there isn't one it will return "nullptr"
    Actor* getGoodie(int x, int y);
    
    bool tryAttack(Actor* pea);
    
    
    //returns whether enemy can move to give spot
    bool canEnemyMoveTo(int x, int y);
    
    //returns whether avatar can move to give spot
    bool canAvatarMoveTo(int x, int y);
    
    //returns whether pea can move to give spot
    bool canPeaMoveTo(int x, int y);
    
    
    
    //returns the list holding the actor
    std::list<Actor*>& getActorList(){
        return m_actors;
    }
    
    //removes actor from actor list (does not delete the pointer however)
    void removeActor(Actor* act);
    
    //adds an actor to the list
    void addActor(Actor* act);
    
    //creates and sets the stat line
    void setDisplayText();
    
    
    void crystalObtained(){
        m_crystalCount--;
    }
    
    //signals game to end the level
    void endLevel(){
        m_levelActive = false;
    }
    
    //returns the amount of crystals left in the game
    int crystalCount(){
        return m_crystalCount;
    }
    
    //changes "count" variable to the census of thiefbots in the surrounding area
    void getFactoryCensus(int x, int y, int& count);
    
    

private:
    Avatar* m_avatar;
    std::list<Actor*> m_actors;
    int m_crystalCount;
    bool m_levelActive;
    int m_bPoints;
};

#endif // STUDENTWORLD_H_


//add a check for the pea to make sure it hits an enemy
