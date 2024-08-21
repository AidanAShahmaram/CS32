#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


//Order of classes
//Actor
//  Wall
//  Pit
//  Mover
//      Pea
//      Marble
//      Attacker
//          Avatar
//          Enemy
//              RageBot
//              ThiefBot
//                  RegThiefBot
//                  MeanThiefBot
//  ThiefBotFactory
//  Consumable
//      Crystal
//      Exit
//      Goodie
//          ELGoodie
//          RHGoodie
//          AGoodie
//


//anything that can take damage
class Actor: public GraphObject{
public:
    Actor(StudentWorld* world, int x, int y, int image, int hp = -1);
    
    virtual ~Actor(){
        return;
    }
    
    virtual void doSomething(){
        return;
    }
    
    virtual void attacked(){
        return;
    }
    
    //adds to health
    void changeHealth(int hp){
        m_hp += hp;
    }
    
    //sets health
    void setHealth(int hp){
        m_hp = 20;
    }
    
    //return health
    int getHealth() const{
        return m_hp;
    }
    
    // return StudentWorld member
    StudentWorld* getWorld() const{
        return m_world;
    }
    
    //sets whether actor is alive
    void setAlive(bool val){
        m_alive = val;
    }
    
    //returns whether actor is alive
    bool isAlive() const{
        return m_alive;
    }
    
    //returns whether object can share a space with a pea
    virtual bool canPeaPass() const{
        return true;
    }
    
    //returns whether object can share a space with an enemy
    virtual bool canEnemyPass() const{
        return true;
    }
    
    //returns whether object can share a space with the player
    virtual bool canAvPass(){
        return false;
    }
    
    //returns whether object can fill a pit
    virtual bool canFill() const{
        return false;
    }
    
    //returns whether object can share a space with a Marble
    virtual bool canMarPass() const{
        return false;
    }
    
    //returns whether an object is a goodie (can be stolen by a thief bot)
    virtual bool isGoodie() const{
        return false;
    }
    
    //returns whether object can be counted while near a factory
    virtual bool countsInFactoryCensus(){
        return false;
    }
    
private:
    StudentWorld* m_world;
    int m_hp;
    bool m_alive;
};



class Wall: public Actor{
public:
    Wall(StudentWorld* world, int x, int y);
    
    virtual ~Wall(){
        return;
    }
    
    virtual bool canPeaPass() const{
        return false;
    }
    
    virtual bool canEnemyPass() const{
        return false;
    }
    
    virtual void doSomething(){
        return;
    }
    
};




class Pit: public Actor{
public:
    Pit(StudentWorld* world, int x, int y);
    
    virtual bool canMarPass() const{
        return true;
    }
    
    virtual bool canEnemyPass() const{
        return false;
    }
    
    virtual void doSomething();
    
    
};




//base class for any object that can move
class Mover: public Actor{
public:
    Mover(StudentWorld* world, int x, int y, int image, int hp);
    
    virtual ~Mover(){}
    
    //moves given x and y value in the direction given
    void moveInDir(int& x, int& y, int dir) const;
    
    
    virtual bool canPeaPass() const{
        return false;
    }
    
    virtual bool canEnemyPass() const{
        return false;
    }
    
};




class Pea: public Mover{
public:
    Pea(StudentWorld* world, int x, int y, int dir);
    
    virtual ~Pea(){}
    
    virtual void doSomething();
    
    
    virtual bool canPeaPass() const{
        return true;
    }
    
    virtual bool canEnemyPass() const{
        return true;
    }
    
    //determines whether pea can continue into the next position
    bool validPos();
    
private:
    bool m_firstLap;
    
    
};




class Marble: public Mover{
public:
    Marble(StudentWorld* world, int x, int y);
    
    virtual void doSomething(){
        return;
    }
    
    virtual void attacked();
    
    //tries to push marble in the given direction
    bool tryPush(int dir);
    
    virtual bool canAvPass();
    
    virtual bool canFill() const{
        return true;
    }
    
};


//base class for any object that can attack
class Attacker: public Mover{
public:
    
    Attacker(StudentWorld* world, int x, int y, int image, int hp);
    
    virtual ~Attacker(){
        return;
    }
    
    virtual void attacked();
    
    //returns the sound of an attacker when attacked
    virtual int impactSound() const = 0;
    
    //returns the sound of an attacker upon death
    virtual int deathSound() const = 0;
    
    //checks whether the given space is out of bounds
    virtual bool checkSpace(int x, int y) const;
    
    //checks whether the attacker can move into its next position
    virtual bool checkMove() const = 0;
    
    
    void shootPea();
     
};




class Avatar: public Attacker{
public:
    Avatar(StudentWorld* world, int x, int y);
    
    
    virtual void doSomething();
    
    
    bool checkMove() const;
    
    //adds pea to pea count
    void addPeas(int pc){
        m_peaCount += pc;
    }
    
    //returns current amount of peas
    int getPeas() const{
        return m_peaCount;
    }
    
    virtual int impactSound() const{
        return SOUND_PLAYER_IMPACT;
    }
    
    virtual int deathSound() const{
        return SOUND_PLAYER_DIE;
    }

private:
    int m_peaCount;
};




//add function to see if it should fire a pea
class Enemy: public Attacker{
public:
    Enemy(StudentWorld* world, int x, int y, int image, int hp);
    
    virtual void doSomething();
    
    virtual void attacked();
    
    //determines whether there is a clear path to hit the player
    bool clearPath() const;
    
    //determines whether the enemy should move during the given tick
    bool moveDuringTick();
    
    virtual int impactSound() const{
        return SOUND_ROBOT_IMPACT;
    }
    
    virtual int deathSound() const{
        return SOUND_ROBOT_DIE;
    }
    
    //should return how mnay points should be given upon death
    virtual int deathPoints() = 0;
    
    virtual bool checkMove() const;
    
    //checks whether the enemy is able to fire peas
    virtual bool canShoot() const{
        return true;
    }
    
    //checks whether the enemy can steal goodies
    virtual bool canSteal() const = 0;
    
    //gives the enemy a stolen object
    virtual void hasStolen(Actor* act) = 0;
    
    //will try to move the enemy
    virtual void tryMove() = 0;
    
    
    
private:
    int m_tick;
    int m_currTick;
    
    
};

class RageBot: public Enemy{
public:
    RageBot(StudentWorld* world, int x, int y, int dir);
    
    
    virtual bool canSteal() const{
        return false;
    }
    
    virtual void hasStolen(Actor* act){
        //can not steal so it returns nothing
        return;
    }
    
    
    virtual int deathPoints(){
        return 100;
    }
    
    virtual void tryMove();
    
};



class ThiefBot: public Enemy{
public:
    ThiefBot(StudentWorld* world, int x, int y, int image, int hp);
    
    virtual ~ThiefBot(){
        if(m_stolenItem != nullptr)
            delete m_stolenItem;
    }
    
    
    virtual void attacked();
    
    
    virtual bool canSteal() const{
        return m_canSteal;
    }
    
    virtual void hasStolen(Actor* act);
    
    virtual void tryMove();
    
    virtual bool countsInFactoryCensus(){
        return true;
    }
    
    
    
private:
    int m_distBefTurn;
    int m_currDist;
    bool m_canSteal;
    Actor* m_stolenItem;
    
};


class RegThiefBot: public ThiefBot{
public:
    RegThiefBot(StudentWorld* world, int x, int y);
    
    virtual bool canShoot() const{
        return false;
    }
    
    virtual int deathPoints(){
        return 10;
    }
    
};


class MeanThiefBot: public ThiefBot{
public:
    MeanThiefBot(StudentWorld* world, int x, int y);
    
    virtual int deathPoints(){
        return 20;
    }
};



class ThiefFactory:  public Actor{
public:
    ThiefFactory(StudentWorld* world, int x, int y, bool mean);
    
    
    virtual void doSomething();
    
    virtual bool canPeaPass() const{
        return false;
    }
    
    virtual bool canEnemyPass() const{
        return false;
    }
    
private:
    bool m_isMean;
};





class Consumable: public Actor{
public:
    Consumable(StudentWorld* world, int x, int y, int image);
    
    bool isConsumed();
    virtual bool canAvPass(){
        return true;
    }
    
    
};


class Crystal: public Consumable{
public:
    Crystal(StudentWorld* world, int x, int y);
    
    
    virtual void doSomething();

};



class Exit: public Consumable{
public:
    Exit(StudentWorld* world, int x, int y);
    
    virtual void doSomething();
};



class Goodie: public Consumable{
public:
    Goodie(StudentWorld* world, int x, int y, int image);
    
    virtual void doSomething();
    
    virtual bool isGoodie() const{
        return true;
    }
    
    //returns amount of points that should be given after collection
    virtual int scoreValue() const = 0;
    
    //performs the effect of the specific goodie
    virtual void effect() = 0;
};


class ELGoodie: public Goodie{
public:
    ELGoodie(StudentWorld* world, int x, int y);
    
    
    virtual int scoreValue() const{
        return 1000;
    }
    
    virtual void effect();
};


class RHGoodie: public Goodie{
public:
    RHGoodie(StudentWorld* world, int x, int y);
    
    
    virtual int scoreValue() const{
        return 500;
    }
    
    virtual void effect();
};


class AGoodie: public Goodie{
public:
    AGoodie(StudentWorld* world, int x, int y);
    
    virtual int scoreValue() const{
        return 100;
    }
    
    virtual void effect();
};






#endif // ACTOR_H_
