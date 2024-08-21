//
//  router.cpp
//  Project4
//
//  Created by Aidan Shahmaram on 3/10/24.
//

#include "router.h"
#include "HashMap.h"
#include "base_classes.h"
#include <queue>
#include <stack>


using namespace std;

Router::Router(const GeoDatabaseBase& geo_db): m_geo(geo_db){}

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const{
    //HashMap containing each points previous point
    HashMap<GeoPoint> path;
    
    //queue holding points which need to be checked
    queue<GeoPoint> sort;
    sort.push(pt1);
    
    //vector which holds the connected points for the current point in the loop
    vector<GeoPoint> connections;
    
    GeoPoint curr;
    
    //while there are still points to sort
    while(!sort.empty()){
        
        //get the current point
        curr = sort.front();
        
        //if it is equal to the ending point, break the loop
        if(curr.sLatitude == pt2.sLatitude && curr.sLongitude == pt2.sLongitude){
            break;
        }
        
        //remove the top value
        sort.pop();
        
        //get the current connected points
        connections = m_geo.get_connected_points(curr);
        
        
        //sort the connected points based on their proximity to the end
        bool madeAMove;
        do{
            madeAMove = false;
            for(int i = 0; i < connections.size() - 1; i++){
                if(aStarComp(pt1, pt2, connections[i + 1], connections[i])){
                    
                    GeoPoint temp = connections[i];
                    connections[i] = connections[i + 1];
                    connections[i + 1] = temp;
                    madeAMove = true;
                }
            }
            
        } while (madeAMove);
        
        
        //add the connected points to the search queue
        for(int i = 0; i < connections.size(); i++){
            if(!path.find(connections[i].to_string())){
                sort.push(connections[i]);
                path[connections[i].to_string()] = curr;
            }
        }
    }
    
    stack<GeoPoint> pointReorder;
    GeoPoint* p = &curr;
    
    //add all the points into the stack until the beginning is reached
    while(p->sLatitude != pt1.sLatitude && p->sLongitude != pt1.sLongitude){
        pointReorder.push(*p);
        p = path.find((*p).to_string());
    }
    
    //empty the stack into the vector to get the proper order
    std::vector<GeoPoint> r;
    while(!pointReorder.empty()){
        r.push_back(pointReorder.top());
        pointReorder.pop();
    }
    
    //return the vector
    return r;
    
    
    
}
