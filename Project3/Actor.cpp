#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor functions
Actor::Actor(StudentWorld* world, int x, int y, int image, int hp): GraphObject(image, x, y), m_world(world), m_hp(hp), m_alive(true){}





//Wall Functions
Wall::Wall(StudentWorld* world, int x, int y): Actor(world, x, y, IID_WALL, -1){
    setVisible(true);
}





//Pit Functions
Pit::Pit(StudentWorld* world, int x, int y): Actor(world, x, y, IID_PIT){
    setVisible(true);
}


void Pit::doSomething(){
    if(!isAlive()){
        return;
    }
    
    //checks if it has been filled by a marble
    if(getWorld()->actorCheck(getX(), getY(), this) && getWorld()->getActor(getX(), getY(), this)->canFill() && getWorld()){
        setAlive(false);
        getWorld()->getActor(getX(), getY(), this)->setAlive(false);
    }
}







//Mover functions
Mover::Mover(StudentWorld* world, int x, int y, int image, int hp): Actor(world, x, y, image, hp){}


void Mover::moveInDir(int& x, int& y, int dir) const{
    switch(dir){
        case(up):
            y++;
            break;
        case(down):
            y--;
            break;
        case(left):
            x--;
            break;
        case(right):
            x++;
            break;
    }
}




//Pea Functions
Pea::Pea(StudentWorld* world, int x, int y, int dir): Mover(world, x, y, IID_PEA, -1), m_firstLap(true){
    setVisible(true);
    setDirection(dir);
}



bool Pea::validPos(){
    int x = getX(), y = getY();
    
    //checks if it goes out of bounds or if it hits an actor
    //if it hits an actor it will do damage
    if(x < 0 || x > 14 || y < 0 || y > 14){
        return false;
    } else if(!getWorld()->canPeaMoveTo(x, y)){
        return !getWorld()->tryAttack(this);
    }
    
    //if it hits the player it will do damage
    if(getWorld()->avatarCheck(x, y)){
        getWorld()->getAvatar()->attacked();
        return false;
    }
    
    return true;
}


void Pea::doSomething(){
    //allows the pea to appear in its first block
    if(m_firstLap){
        m_firstLap = false;
        return;
    }
    
    if(!isAlive()){
        return;
    }
    
    //checks position before its move
    if(!validPos()){
        setAlive(false);
        return;
    }
    
    //moves
    int x = getX(), y = getY();
    int dir = getDirection();
    
    moveInDir(x, y, dir);
    
    moveTo(x, y);
    
    
    //checks position after movement
    if(!validPos()){
        setAlive(false);
        return;
    }
}








//Marble Functions
Marble::Marble(StudentWorld* world, int x, int y): Mover(world, x, y, IID_MARBLE, 10){
    setVisible(true);
}

void Marble::attacked(){
    changeHealth(-2);
    if(getHealth() <= 0)
        setAlive(false);
    
}


bool Marble::tryPush(int dir){
    int x = getX(), y = getY();
    
    moveInDir(x, y, dir);
    
    //checks if it is moving out of bounds
    //if space is not empty, check id marble can still move into it
    if(y > 14 || y < 0 || x > 14 || x < 0)
        return false;
    else if(getWorld()->actorCheck(x, y, this)){
        if(!getWorld()->getActor(x, y)->canMarPass())
            return false;
    }
    
    moveTo(x, y);
    return true;
    
    
}




bool Marble::canAvPass(){
    //returns whether avatar can move the marble and take its place
    return tryPush(getWorld()->getAvatar()->getDirection());
}






//Attacker Functions
Attacker::Attacker(StudentWorld* world, int x, int y, int image, int hp): Mover(world, x, y, image, hp){}


void Attacker::shootPea(){
    int x = getX(), y = getY();
    int dir = getDirection();
    
    //sets pea's postion one in front of actor shooting
    moveInDir(x, y, dir);
    
    //adds pea as an actor into studentworld
    getWorld()->getActorList().push_back(new Pea(getWorld(), x, y, dir));
    
    
}




bool Attacker::checkSpace(int x, int y) const{
    
    if(y > 14 || y < 0 || x > 14 || x < 0)
        return false;
    else if(getWorld()->avatarCheck(x, y))
        return false;
    
    
    return true;
}


void Attacker::attacked(){
    changeHealth(-2);
    if(getHealth() > 0){
        getWorld()->playSound(impactSound());
    } else {
        getWorld()->playSound(deathSound());
        setAlive(false);
    }
}





//Avatar Functions
Avatar::Avatar(StudentWorld* world, int x, int y): Attacker(world, x, y, IID_PLAYER, 20), m_peaCount(20){
    setDirection(right);
    setVisible(true);
}

bool Avatar::checkMove() const{
    
    int x = getX(), y = getY(), dir = getDirection();
    moveInDir(x, y, dir);
    
    if(!Attacker::checkSpace(x, y))
        return false;
    
    //checks whether the Avatar can move to the position
    if(!getWorld()->canAvatarMoveTo(x, y)){
        return false;
    }
    
    return true;
}


void Avatar::doSomething(){
    if(!isAlive())
        return;
    
    //remember to add function to check whether it can move
    
    
    int ch;
    int x = getX(), y = getY();
    
    //commands based on what key has been pressed
    if(getWorld()->getKey(ch)){
        bool move = false;
        switch(ch){
            case(KEY_PRESS_UP):
                setDirection(up);
                move = true;
                break;
            case(KEY_PRESS_DOWN):
                setDirection(down);
                move = true;
                break;
            case(KEY_PRESS_LEFT):
                setDirection(left);
                move = true;
                break;
            case(KEY_PRESS_RIGHT):
                setDirection(right);
                move = true;
                break;
            case(KEY_PRESS_ESCAPE):
                setAlive(false);
                getWorld()->decLives();
                break;
            case(KEY_PRESS_SPACE):
                if(m_peaCount > 0){
                    shootPea();
                    m_peaCount--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
        }
        
        //if player wants to move, checks if move is valid before doing so
        if(move){
            if(checkMove()){
                moveInDir(x, y, getDirection());
                moveTo(x, y);
            }
        }
        
        
        
    }
    
}




//Enemy Functions
Enemy::Enemy(StudentWorld* world, int x, int y, int image, int hp): Attacker(world, x, y, image, hp), m_currTick(0){
    
    //sets how many ticks an enemy must wait before making a move
    m_tick = (28 - getWorld()->getLevel()) / 4;
    
    if(m_tick < 3)
        m_tick = 3;
}

void Enemy::doSomething(){
    if(!isAlive())
        return;
    
    //if it can not move during this tick, it will return
    if(!moveDuringTick())
        return;
    
    //if it is able to steal, it retrieves the goodie it is on and stores it
    if(canSteal()){
        Actor* temp = getWorld()->getGoodie(getX(), getY());
        if(temp != nullptr && randInt(1, 10) == 10){
            hasStolen(temp);
        }
    }
    
    //if there is a clear path and it is able to shoot, it will do so
    if(canShoot() && clearPath()){
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        shootPea();
        return;
    }
    
    //it will try to move and will turn/rotate if it has to
    tryMove();
    
    
    
    
}

bool Enemy::moveDuringTick(){
    //checks whether it moves on the current tick
    if(m_currTick % m_tick == 0){
        m_currTick++;
        return true;
    }
    
    m_currTick++;
    return false;
}


bool Enemy::clearPath() const{
    int dir = getDirection();
    int x = getX(), y = getY();
    moveInDir(x, y, dir);
    
    //depending on its direction, checks whether it has a clear path to hit the player
    switch(dir){
        case(up):
            while(y < 15){
                if(!getWorld()->canPeaMoveTo(x, y)){
                    return false;
                } else if(getWorld()->avatarCheck(x, y))
                    return true;
                y++;
            }
            return false;
            break;
        case(down):
            while(y >= 0){
                if(!getWorld()->canPeaMoveTo(x, y)){
                    return false;
                } else if(getWorld()->avatarCheck(x, y))
                    return true;
                y--;
            }
            return false;
            break;
        case(left):
            while(x >= 0){
                if(!getWorld()->canPeaMoveTo(x, y)){
                    return false;
                } else if(getWorld()->avatarCheck(x, y))
                    return true;
                x--;
            }
            return false;
            break;
        case(right):
            while(x < 15){
                if(!getWorld()->canPeaMoveTo(x, y)){
                    return false;
                } else if(getWorld()->avatarCheck(x, y))
                    return true;
                x++;
            }
            return false;
            break;
        default:
            return false;
    }
}

void Enemy::attacked(){
    Attacker::attacked();
    //if no longer alive, will increase points based on type of enemy
    if(!isAlive())
        getWorld()->increaseScore(deathPoints());
}

bool Enemy::checkMove() const{
    int x = getX(), y = getY(), dir = getDirection();
    
    moveInDir(x, y, dir);
    
    if(!Attacker::checkSpace(x, y))
        return false;
    
    //checks with the StudentWorld if it can move to the given location
    if(!getWorld()->canEnemyMoveTo(x, y))
        return false;
    
    return true;
}



//RageBot functions

RageBot::RageBot(StudentWorld* world, int x, int y, int dir): Enemy(world, x, y, IID_RAGEBOT, 10){
    //sets it either as a horiz or vert bot
    if(dir == right || dir == left)
        setDirection(right);
    else
        setDirection(up);
    
    setVisible(true);
    
}




void RageBot::tryMove(){
    int x = getX(), y = getY(), dir = getDirection();
    moveInDir(x, y, dir);
    
    
    
    //if it can't move in its current direction, it rotates
    
        if(!checkMove()){
            switch(getDirection()){
                case(right):
                    setDirection(left);
                    break;
                case(left):
                    setDirection(right);
                    break;
                case(up):
                    setDirection(down);
                    break;
                case(down):
                    setDirection(up);
                    break;
            }
            
            return;
        }
    
    //moves
    moveTo(x, y);
    return;
}


//ThiefBot functions

ThiefBot::ThiefBot(StudentWorld* world, int x, int y, int image, int hp): Enemy(world, x, y, image, hp), m_currDist(0),  m_canSteal(true), m_stolenItem(nullptr){
    setDirection(right);
    m_distBefTurn = randInt(1, 6);
}


void ThiefBot::tryMove(){
    int x = getX(), y = getY(), dir = getDirection();
    moveInDir(x, y, dir);
    
    //if it has not yet reached its distance, it will try to move forward
    if(m_currDist != m_distBefTurn){
        if(checkMove()){
            moveTo(x, y);
            return;
        }
    }
    
    //if it reached its turning distance or can no longer move, it will turn to a random direction
    int rand = randInt(1, 4);
    switch(rand){
        case(1):
            setDirection(right);
            break;
        case(2):
            setDirection(left);
            break;
        case(3):
            setDirection(up);
            break;
        case(4):
            setDirection(down);
            break;
    }
        
    
}

void ThiefBot::attacked(){
    Enemy::attacked();
    
    //if it is no longer alive and holds an item, it will drop it at its current location and add it back to the StudentWorld member
    if(!isAlive()){
        if(m_stolenItem != nullptr){
            m_stolenItem->moveTo(getX(), getY());
            m_stolenItem->setVisible(true);
            getWorld()->addActor(m_stolenItem);
            m_stolenItem = nullptr;
        }
    }
}

void ThiefBot::hasStolen(Actor* act){
    //stores the given paramater into the stolen item variable
    m_canSteal = false;
    getWorld()->playSound(SOUND_ROBOT_MUNCH);
    m_stolenItem = act;
    m_stolenItem->setVisible(false);
    getWorld()->removeActor(act);
    return;
}



//RegThiefBot Functions

RegThiefBot::RegThiefBot(StudentWorld* world, int x, int y): ThiefBot(world, x, y, IID_THIEFBOT, 5){
    setVisible(true);
}


//MeanThiefBot Functions

MeanThiefBot::MeanThiefBot(StudentWorld* world, int x, int y): ThiefBot(world, x, y, IID_MEAN_THIEFBOT, 8){
    setVisible(true);
}



//ThiefFactory Functions

ThiefFactory::ThiefFactory(StudentWorld* world, int x, int y, bool mean): Actor(world, x, y, IID_ROBOT_FACTORY), m_isMean(mean){
    setVisible(true);
}

void ThiefFactory::doSomething(){
    int count = 0, x = getX(), y = getY();
    getWorld()->getFactoryCensus(x, y, count);
    //checks whether the factory is elegible to create another thiefbot
    if(count < 3 && !getWorld()->actorCheck(getX(), getY(), this)){
        if(randInt(1, 50) == 50){
            //based on the type of factory, it will either produce a mean or regular thiefbot
            if(m_isMean){
                getWorld()->getActorList().push_back(new MeanThiefBot(getWorld(), x, y));
            } else {
                getWorld()->getActorList().push_back(new RegThiefBot(getWorld(), x, y));
            }
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}




//Consumable
Consumable::Consumable(StudentWorld* world, int x, int y, int image): Actor(world, x, y, image){}


bool Consumable::isConsumed(){
    //checks if it is in the same position as the player
    return (getWorld()->avatarCheck(getX(), getY()));
}



//Crystal Functions
Crystal::Crystal(StudentWorld* world, int x, int y): Consumable(world, x, y, IID_CRYSTAL){
    setVisible(true);
}

void Crystal::doSomething(){
    if(!isAlive())
        return;
    
    if(isConsumed()){
        getWorld()->crystalObtained();
        setAlive(false);
    }
    
}


//Exit Functions
Exit::Exit(StudentWorld* world, int x, int y): Consumable(world, x, y, IID_EXIT){
    setVisible(false);
}

void Exit::doSomething(){
    //if all the crystals have been found, it makes itself appear
    if(getWorld()->crystalCount() == 0){
        getWorld()->playSound(SOUND_REVEAL_EXIT);
        setVisible(true);
        
        //sets crystal count to -1 so that reveal sound isn't continuously played
        getWorld()->crystalObtained();
    }
    
    //ends the level if the player has collected the exit
    if(isConsumed() && isVisible()){
        getWorld()->increaseScore(2000);
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->endLevel();
    }
}





//Goodie functions

Goodie::Goodie(StudentWorld* world, int x, int y, int image): Consumable(world, x, y, image){}

void Goodie::doSomething(){
    if(!isAlive())
        return;
    
    //if consumed, it will play the sound, increase the score, and perform its specified effect
    if(isConsumed()){
        setAlive(false);
        getWorld()->increaseScore(scoreValue());
        getWorld()->playSound(SOUND_GOT_GOODIE);
        effect();
    }
}


//EL Goodie functions

ELGoodie::ELGoodie(StudentWorld* world, int x, int y): Goodie(world, x, y, IID_EXTRA_LIFE){
    setVisible(true);
}

void ELGoodie::effect(){
    getWorld()->incLives();
}

//RH Goodie Funtions

RHGoodie::RHGoodie(StudentWorld* world, int x, int y): Goodie(world, x, y, IID_RESTORE_HEALTH){
    setVisible(true);
}


void RHGoodie::effect(){
    getWorld()->getAvatar()->setHealth(20);
}


//AGoodie Functions
AGoodie::AGoodie(StudentWorld* world, int x, int y): Goodie(world, x, y, IID_AMMO){
    setVisible(true);
}

void AGoodie::effect(){
    getWorld()->getAvatar()->addPeas(20);
}
