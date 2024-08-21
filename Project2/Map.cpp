//
//  Map.cpp
//  CS32_Project2
//
//  Created by Aidan Shahmaram on 1/24/24.
//

#include "Map.h"

using namespace std;



Map::Map(): m_size(0){
    //sets member variables
    m_head.prev = nullptr;
    m_head.next = &m_tail;
    m_tail.prev = &m_head;
    m_tail.next = nullptr;
}



Map::~Map(){
    //deletes all remaining nodes
    Node* temp = m_head.next;
    if(!this->empty()){
        while(temp->next != nullptr){
            temp = temp->next;
            delete temp->prev;
        }
    }
}

Map::Map(const Map& old){
    //declares member variables
    m_head.prev = nullptr;
    m_head.next = &m_tail;
    m_tail.prev = &m_head;
    m_tail.next = nullptr;
    m_size = 0;
    
    Node* temp = m_head.next;
    
    //copies and inserts all nodes from "old" into current Map
    temp = old.m_head.next;
    while(temp->next != nullptr){
        this->insert(temp->key, temp->value);
        temp = temp->next;
    }
    
    
}

Map& Map::operator=(const Map &src){
    //if it tries to copy itself, it will do nothing and return itself;
    if(&src == this)
        return *this;
    
    
    //deletes all remaining nodes in current map
    Node* temp = m_head.next;
    if(!this->empty()){
        while(temp->next != nullptr){
            temp = temp->next;
            delete temp->prev;
        }
    }
    
    //resets all member variables
    m_head.prev = nullptr;
    m_head.next = &m_tail;
    m_tail.prev = &m_head;
    m_tail.next = nullptr;
    m_size = 0;
    
    
    //copies and inserts all nodes from "src"
    temp = src.m_head.next;
    
    while(temp->next != nullptr){
        this->insert(temp->key, temp->value);
        temp = temp->next;
    }
    
    
    
    
    //returns itself
    return *this;
    
}

bool Map::empty() const{
    //returns whether size is 0
    if(m_size == 0){
        return true;
    }
    return false;
}



int Map::size() const{
    //returns size
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value){
    
    Node* temp = m_head.next;
    
    //if there is an existing node with the same key, the function will return false and do nothing
    while(temp->next != nullptr){
        if(temp->key == key)
            return false;
        temp = temp->next;
    }
    
    //creates new node and sets its values
    temp = m_tail.prev;
    Node* net = new Node;
    net->key = key;
    net->value = value;
    
    //changes all necessary pointers which reference the previous and next nodes
    temp->next = net;
    net->prev = temp;
    m_tail.prev = net;
    net->next = &m_tail;
    m_size++;
    return true;
    
    
    
}


bool Map::update(const KeyType& key, const ValueType& value){
    Node* temp = m_head.next;
    
    //if there is an existing node with the same key, it will update its value
    while(temp->next != nullptr){
        if(temp->key == key){
            temp->value = value;
            return true;
        }
        temp = temp->next;
    }
    return false;
}





bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    //the function will try to update, and if there is no node to update it will insert a new one with the given key and value.
    if(this->update(key, value))
        return true;
    
    if(this->insert(key, value))
        return true;
    
    
    //If it is unable to update or create a new node it will return false.
    return false;
    
}

bool Map::erase(const KeyType& key){
    //if there is a node with the same key as the input, it will delete that node
    Node* temp = m_head.next;
    while(temp->next != nullptr){
        if(temp->key == key){
            //change the previous and next pointers of the surrounding nodes
            Node* p = temp->prev;
            Node* n = temp->next;
            p->next = n;
            n->prev = p;
            
            //delete the node and return
            delete temp;
            m_size--;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

bool Map::contains(const KeyType& key) const{
    //goes through each node and checks for a matching key
    Node* temp = m_head.next;
    while(temp->next != nullptr){
        if(temp->key == key){
            //if there is a matching key it returns true
            return true;
        }
        temp = temp->next;
    }
    //if there was no matching key it will return false
    return false;
    
}

bool Map::get(const KeyType& key, ValueType& value) const{
    //goes through each node and checks for a matching key
    Node* temp = m_head.next;
    while(temp->next != nullptr){
        if(temp->key == key){
            //if there is a matching key it sets the given value the value paired with the given key and returns true
            value = temp->value;
            return true;
        }
        temp = temp->next;
    }
    //if there is no matching key it returns false
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const{
    
    //for each node in the map (starting at the node after head until reaching the tail), it
    Node* temp = m_head.next;
    int count = 0;
    while(temp->next != nullptr){
        //compares the key from the "temp" node with all other nodes ("temp2") in the map (starting at the node after head until it reaches the tail)
        Node* temp2 = m_head.next;
        while(temp2->next != nullptr){
            if(temp->key > temp2->key)
                count++;
            temp2 = temp2->next;
        }
        //count now holds the number of nodes "temp" is greater than
        //if it matches the input, then the key and value will be copied and it will return true
        if(count == i){
            key = temp->key;
            value = temp->value;
            return true;
        }
        temp = temp->next;
        count = 0;
        
    }
    //if there is no corresponding pair, the function returns "false" without changing any variables
    return false;
}

void Map::swap(Map& other){
    //if it tries swapping with itself, it will return
    if(&other == this)
        return;
    
    
    
    //the head nodes of each Map now point to the first pair of eachother's map
    //the "prev" value of those nodes are also switched
    Node* temp = m_head.next;
    m_head.next = other.m_head.next;
    other.m_head.next = temp;
    m_head.next->prev = &m_head;
    other.m_head.next->prev = &other.m_head;
    
    //the tail nodes of each Map now point to the last pair of eachother's map
    //the "next" value of those nodes are also switched
    temp = m_tail.prev;
    m_tail.prev = other.m_tail.prev;
    other.m_tail.prev = temp;
    m_tail.prev->next = &m_tail;
    other.m_tail.prev->next = &other.m_tail;
    
    int s = this->m_size;
    this->m_size = other.m_size;
    other.m_size = s;
    
    
}



bool merge(const Map& m1, const Map& m2, Map& result){
    Map output;
    KeyType key;
    ValueType val, val2;
    
    bool ret = true;
    
    //copies m1 into output
    output = m1;
    
    
    //goes through the values of m2
    for(int i = 0; i < m2.size(); i++){
        m2.get(i, key, val);

        //if there is already a pair in output with the same key (coming from m1), it checks if it has the same value.
        if(!output.insert(key, val)){
            m1.get(key, val2);
            //if the value is not the same it erases the original and sets the return value to false
            if(val != val2){
                ret = false;
                output.erase(key);
            }
        }
    }
    //set result as the output
    result = output;
    return ret;
}

void reassign(const Map& m, Map& result){
    KeyType key, trashKey;
    ValueType val, firstVal;
    
    //copies m1 into result
    result = m;
    
    //if m is empty it returns the current result
    if(m.size() <= 1){
        return;
    }
    
    //stores the value of the first node in the loop
    m.get(0, key, firstVal);
    
    //for each node in the loop, starting with lowest alphabetically, it will replace its value with the next node's and stop before the last one
    for(int i = 0; i < m.size() - 1; i++){
        m.get(i, key, val);
        m.get(i + 1, trashKey, val);
        result.update(key, val);
    }
    
    //sets the last key with the first value which was saved
    m.get(m.size() - 1, key, val);
    result.update(key, firstVal);
    
    
}
