//
//  Map.hpp
//  CS32_Project2
//
//  Created by Aidan Shahmaram on 1/24/24.
//


#ifndef Map_hpp
#define Map_hpp

#include <string>

using KeyType = std::string;
using ValueType = double;





class Map
{
  public:
    Map();         // Create an empty map (i.e., one whose size() is 0).
    
    ~Map();        //Destructor
    
    Map(const Map& old);  //copy constructor
    
    Map& operator=(const Map &src); //assignment operator

    bool empty() const;  // Return true if the map is empty, otherwise false.

    int size() const;    // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);

    bool update(const KeyType& key, const ValueType& value);

    bool insertOrUpdate(const KeyType& key, const ValueType& value);

    bool erase(const KeyType& key);
     
    bool contains(const KeyType& key) const;
     
    bool get(const KeyType& key, ValueType& value) const;
     
    bool get(int i, KeyType& key, ValueType& value) const;

    void swap(Map& other);
    
 private:
    struct Node{
        KeyType key;
        ValueType value;
        Node* prev;
        Node* next;
    };
    
    Node m_head;
    Node m_tail;
    int m_size;
    
};

bool merge(const Map& m1, const Map& m2, Map& result);

void reassign(const Map& m, Map& result);


#endif /* Map_hpp */
