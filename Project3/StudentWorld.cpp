#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_avatar(nullptr)
{
}

int StudentWorld::init()
{
    m_crystalCount = 0;
    m_bPoints = 1000;
    m_levelActive = true;
    
    //creates string for to get the file for current level
    string curLevel = "level";
    if(getLevel() < 10)
        curLevel += "0" + to_string(getLevel());
    else
        curLevel += to_string(getLevel());
    curLevel += ".txt";
    
    //loads level into an object of the level class
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    
    //determines whether the level was loaded correctly
    if (result == Level::load_fail_file_not_found ||
    result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    //initializes each item in the level
    Level::MazeEntry item;
    for(int x = 0; x < 15; x++){
        for(int y = 0; y < 15; y++){
            item = lev.getContentsOf(x, y);
            if(item == Level::player){
                m_avatar = new Avatar(this, x, y);
            } else if(item == Level::exit){
                m_actors.push_back(new Exit(this, x, y));
            }else if(item == Level::horiz_ragebot){
                m_actors.push_back(new RageBot(this, x, y, Actor::right));
            }else if(item == Level::vert_ragebot){
                m_actors.push_back(new RageBot(this, x, y, Actor::up));
            }else if(item == Level::thiefbot_factory){
                m_actors.push_back(new ThiefFactory(this, x, y, false));
            }else if(item == Level::mean_thiefbot_factory){
                m_actors.push_back(new ThiefFactory(this, x, y, true));
            }else if(item == Level::wall){
                m_actors.push_back(new Wall(this, x, y));
            }else if(item == Level::marble){
                m_actors.push_back(new Marble(this, x, y));
            }else if(item == Level::pit){
                m_actors.push_back(new Pit(this, x, y));
            }else if(item == Level::exit){
                m_actors.push_back(new Exit(this, x, y));
            }else if(item == Level::crystal){
                m_crystalCount++;
                m_actors.push_back(new Crystal(this, x, y));
            }else if(item == Level::restore_health){
                m_actors.push_back(new RHGoodie(this, x, y));
            }else if(item == Level::extra_life){
                m_actors.push_back(new ELGoodie(this, x, y));
            }else if(item == Level::ammo){
                m_actors.push_back(new AGoodie(this, x, y));
            }
        }
    }
    
       
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //sets the stat line
    setDisplayText();
    
    //gives player a chance to do something
    m_avatar->doSomething();
    
    //gives each actor a chance to do something
    list<Actor*>::iterator i = m_actors.begin();
    if(!m_actors.empty()){
        while(i != m_actors.end()){
            (*i)->doSomething();
            
            //if the actor is dead it is removed
            if(!(*i)->isAlive()){
                delete (*i);
                i = m_actors.erase(i);
                i--;
            }
            
            //if the actor has done something that has caused the player to die, the level will end
            if(!m_avatar->isAlive())
                return GWSTATUS_PLAYER_DIED;
            
            i++;
        }
    }
    
    //if the level is no longer active (exit has been collected), it will go onto the next
    if(!m_levelActive){
        increaseScore(m_bPoints);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //decreases the bonus points
    if(m_bPoints > 0)
        m_bPoints--;
    
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //if m_avatar still holds a value it will be deleted
    if(m_avatar){
        delete m_avatar;
         m_avatar = nullptr;
    }
    
    //all actors that are still alive will be deleted
    list<Actor*>::iterator i = m_actors.begin();
    if(!m_actors.empty()){
        while(i != m_actors.end()){
            delete (*i);
            i = m_actors.erase(i);
        }
    }
}



bool StudentWorld::actorCheck(int x, int y, Actor* curr){
    list<Actor*>::iterator i = m_actors.begin();
    if(!m_actors.empty()){
        while(i != m_actors.end()){
            //checks if there is an actor
            if (*i != curr && (*i)->getX() == x && (*i)->getY() == y) {
                return true;
            }
            i++;
        }
    }
    return false;
}


bool StudentWorld::avatarCheck(int x, int y){
    return(m_avatar->getX() == x && m_avatar->getY() == y);
}


Actor* StudentWorld::getActor(int x, int y){
    list<Actor*>::iterator i = m_actors.begin();
    if(m_actors.empty())
        return nullptr;
    
    while(i != m_actors.end()){
        //returns the actor if it is at the same coordinates
        if ((*i)->getX() == x && (*i)->getY() == y && (*i)->isAlive()){
            return *i;
        }
        i++;
    }
    return nullptr;
}


Actor* StudentWorld::getActor(int x, int y, Actor* curr){
    list<Actor*>::iterator i = m_actors.begin();
    if(m_actors.empty())
        return nullptr;
    
    while(i != m_actors.end()){
        //returns the actor if it is at the same given coordinates and it is not the same given actor
        if ((*i)->getX() == x && (*i)->getY() == y && (*i)->isAlive() && (*i) != curr){
            return *i;
        }
        i++;
    }
    return nullptr;
}


Actor* StudentWorld::getGoodie(int x, int y){
    list<Actor*>::iterator i = m_actors.begin();
    if(m_actors.empty())
        return nullptr;
    
    while(i != m_actors.end()){
        //checks if the actor is at the same spot and whether it is a goodie
        if ((*i)->getX() == x && (*i)->getY() == y && (*i)->isGoodie()){
            return *i;
        }
        i++;
    }
    return nullptr;
}



bool StudentWorld::tryAttack(Actor* pea){
    list<Actor*>::iterator i = m_actors.begin();
    bool ret = false;
    if(m_actors.empty())
        return false;
    
    while(i != m_actors.end()){
        //checks if the actor is at the same spot doesn't allow the pea to pass
        //if so it is attacked
        if ((*i)->getX() == pea->getX() && (*i)->getY() == pea->getY() && (*i)->isAlive() && (*i) != pea && !(*i)->canPeaPass()){
            (*i)->attacked();
            ret = true;
        }
        i++;
    }
    return ret;
}


bool StudentWorld::canEnemyMoveTo(int x, int y){
    list<Actor*>::iterator i = m_actors.begin();
    if(m_actors.empty())
        return false;
    
    bool ret = true;
    
    while(i != m_actors.end()){
        //checks if the actor is at the same spot and allows the enemy to pass
        if ((*i)->getX() == x && (*i)->getY() == y && (*i)->isAlive() && !(*i)->canEnemyPass()){
            ret = false;
        }
        i++;
    }
    return ret;
}

bool StudentWorld::canAvatarMoveTo(int x, int y){
    list<Actor*>::iterator i = m_actors.begin();
    if(m_actors.empty())
        return false;
    
    bool ret = true;
    
    while(i != m_actors.end()){
        //checks if the actor is at the same spot and allows the player to pass
        if ((*i)->getX() == x && (*i)->getY() == y && (*i)->isAlive() && !(*i)->canAvPass()){
            ret = false;
        }
        i++;
    }
    return ret;
}

bool StudentWorld::canPeaMoveTo(int x, int y){
    list<Actor*>::iterator i = m_actors.begin();
    if(m_actors.empty())
        return false;
    
    bool ret = true;
    
    while(i != m_actors.end()){
        //checks if the actor is at the same spot and if it allows a pea to pass
        if ((*i)->getX() == x && (*i)->getY() == y && (*i)->isAlive() && !(*i)->canPeaPass()){
            ret = false;
        }
        i++;
    }
    
    return ret;
}




void StudentWorld::removeActor(Actor* act){
    list<Actor*>::iterator i = m_actors.begin();
    if(!m_actors.empty()){
        while(i != m_actors.end()){
            //checks if the actor is the same as the one given, if so deletes it
            if ((*i) == act) {
                m_actors.erase(i);
                return;
            }
            i++;
        }
    }
}

void StudentWorld::addActor(Actor* act){
    m_actors.push_back(act);
}



void StudentWorld::setDisplayText(){
    int score = getScore();
    int lives = getLives();
    int level = getLevel();
    int hp = 100 * (m_avatar->getHealth()) / 20;
    int ammo = m_avatar->getPeas();
    int bPoints = m_bPoints;
    
    string s_score, s_lives, s_level, s_hp, s_ammo, s_bPoints;
    ostringstream s, li, le, h, a, b;
    
    
    //sets each string with its set length and filler characters
    s.fill('0');
    s << setw(7) << score;
    s_score = s.str();
    
    li.fill('0');
    li << setw(2) << lives;
    s_lives = li.str();
    
    le.fill(' ');
    le << setw(2) << level;
    s_level = le.str();
    
    h.fill(' ');
    h << setw(3) << hp;
    s_hp = h.str() + "%";
    
    a.fill(' ');
    a << setw(3) << ammo;
    s_ammo = a.str();
    
    b.fill(' ');
    b << setw(4) << bPoints;
    s_bPoints = b.str();
    
    
    //puts all the string together
    string result = "Score: " + s_score + "  Level: " + s_level + "  Lives: " + s_lives + "  Health: " + s_hp + "  Ammo: " + s_ammo + "  Bonus: " + s_bPoints;
    
    setGameStatText(result);
    
}

void StudentWorld::getFactoryCensus(int x, int y, int& count){
    list<Actor*>::iterator i = m_actors.begin();
    count = 0;
    
    if(!m_actors.empty()){
        while(i != m_actors.end()){
            //checks if it is within its surrounding
            //if it is and also can be counted, it is added
            if ((*i)->countsInFactoryCensus() &&
                (*i)->getX() >= x - 3 && (*i)->getX() <= x + 3 &&
                (*i)->getY() >= y - 3 && (*i)->getY() <= y + 3) {
                count++;
            }
            i++;
        }
    }
    
}

