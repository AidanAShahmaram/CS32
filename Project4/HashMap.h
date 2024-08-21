

#ifndef HashMap_h
#define HashMap_h

#include <string>
template <typename T>


class HashMap
{
public:
    HashMap(double max_load = 0.75): m_load(max_load), m_buckets(10), m_bucketsUsed(0){
        m_container = new std::vector<Bucket>[m_buckets];
        
    } // constructor
    
    ~HashMap(){
        //clear everything out of each vector
        for(int i = 0; i < m_buckets; i++){
            m_container[i].clear();
        }
        
        //delete the array of vectors
        delete[] m_container;
    }
    
    int size() const{
        return m_bucketsUsed;
    }
    
    
    
    void insert(const std::string& key, const T& value){
        int pos = hashForArray(key);
        
        //if there is already an association with the same key, the value is changed
        for(int i = 0; i < m_container[pos].size(); i++){
            if(m_container[pos][i].m_key == key){
                m_container[pos][i].m_value = value;
                return;
            }
        }
        
        
        m_bucketsUsed++;
        
        //if the load factor is too high, then the size of the container will be doubled and the values will be reallocated to new buckets
        if(((m_bucketsUsed * 1.0) / m_buckets) > m_load){
            int size = m_buckets * 2;
            std::vector<Bucket>* tempContainer = new std::vector<Bucket>[size];
            
            int oldSize = m_buckets;
            m_buckets *= 2;
            
            //for each bucket in the old container, rehash it and put it in the new one
            for(int i = 0; i < oldSize; i++){
                for(int j = 0; j < m_container[i].size(); j++){
                    std::string ID = m_container[i][j].m_key;
                    int pos = hashForArray(ID);
                    tempContainer[pos].push_back(Bucket(ID, m_container[i][j].m_value));
                }
            }
            
            //deletes contents of old container and switches to that of the temporary one
            delete[] m_container;
            m_container = tempContainer;
            
        }
    
        //creates a new bucket with the given key
        m_container[hashForArray(key)].push_back(Bucket(key, value));
    }
    
    


    
    
    T* find(const std::string& key) const{
        //get the position of the key
        int pos = hashForArray(key);
        
        if(m_bucketsUsed > 0){
            //if a bucket has a matching key, return a pointer to its value
            for(int i = 0; i < m_container[pos].size(); i++){
                if(m_container[pos][i].m_key == key)
                    return &m_container[pos][i].m_value;
            }
        }
        
        //a matching key was not found
        return nullptr;
    }
    
    
    
    T& operator[](const std::string& key){
        //gets the position of the key
        int pos = hashForArray(key);
        
        //if a matching key is found, then a reference to its value is returned
        for(int i = 0; i < m_container[pos].size(); i++){
            if(m_container[pos][i].m_key == key)
                return m_container[pos][i].m_value;
        }
        
        //if matching key was not found, the key will be inserted with a base value
        T rand;
        insert(key, rand);
        
        
        //the matching key is returned
        pos = hashForArray(key);
        for(int i = 0; i < m_container[pos].size(); i++){
            if(m_container[pos][i].m_key == key)
                return m_container[pos][i].m_value;
        }
        
        
        //returns first value so that there is a return case in all control paths, it will never get here however
        return m_container[0][0].m_value;
    }
    
    
    
    
    
    
private:
    //struct of a bucket that holds its key and value
    struct Bucket{
        Bucket(){}
        
        Bucket(std::string key, T value){
            m_key = key;
            m_value = value;
        }
        
        Bucket(std::string key){
            m_key = key;
        }
        
        
        std::string m_key;
        T m_value;
    };
    
    //change this so that only one container is used
    double m_load;
    std::vector<Bucket>* m_container;
    int m_buckets;
    int m_bucketsUsed;
    
    //std::hash<std::string>()(key)
    
    
    
    int hashForArray(std::string key) const{
        std::size_t num = std::hash<std::string>()(key);
        int b = m_buckets;
        
        return (num % b);
            
    }
    
};

#endif /* HashMap_h */
