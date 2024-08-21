//
//  geodb.cpp
//  Project4
//
//  Created by Aidan Shahmaram on 3/7/24.
//

#include "geodb.h"
#include <iostream>
#include <fstream>
#include "geopoint.h"
#include "geotools.h"
using namespace std;


GeoDatabase::GeoDatabase(){}
GeoDatabase::~GeoDatabase(){}

bool GeoDatabase::load(const std::string& map_data_file){
    //loads in map file
    ifstream infile(map_data_file);
    
    //if it did not load correctly, leaves an error message and returns
    if(!infile){
        return false;
    }
    
    
    string street;
    
    //for each street segment
    while(getline(infile, street)){
        string lat1, lon1, lat2, lon2;
        
        //take in all the coordinates
        infile >> lat1;
        infile >> lon1;
        infile >> lat2;
        infile >> lon2;
        
        GeoPoint first(lat1, lon1);
        GeoPoint second(lat2, lon2);
        
        //adds both points into map, using their string form as the key
        m_points[first.to_string()].m_loc = first;
        m_points[second.to_string()].m_loc = second;
        
        //adds points of the segments as connections of eachother
        m_points[first.to_string()].m_connectedPoints.push_back(second);
        m_points[second.to_string()].m_connectedPoints.push_back(first);
        
        //attatches a street name to the segment
        m_connects[first.to_string() + " " + second.to_string()] = street;
        
        //takes in the amount of locations
        int count;
        
        infile >> count;
        infile.ignore(10000, '\n');
        
        string poiLat, poiLon;
        
        //if there are locations within the segment
        if(count > 0){
            
            //creates a midpoiont within the street segment
            GeoPoint path = midpoint(first, second);
            
            //adds the midpoint to the map of coordinates
            m_points[path.to_string()].m_loc = path;
            
            //creates a bi-directional connection between the midpoint and the street segment points
            m_points[path.to_string()].m_connectedPoints.push_back(second);
            m_points[path.to_string()].m_connectedPoints.push_back(first);
            
            m_points[first.to_string()].m_connectedPoints.push_back(path);
            m_points[second.to_string()].m_connectedPoints.push_back(path);
            
            //attatches a street name to the segment
            m_connects[first.to_string() + " " + path.to_string()] = street;
            m_connects[path.to_string() + " " + second.to_string()] = street;
            
            //for each location
            for(int i = 0; i < count; i++){
                
                //collect the name of the location
                char c;
                string name = "";
                
                while(infile.get(c)){
                    if(c == '|')
                        break;
                    
                    name += c;
                }
                
                //collect the coordinates
                infile >> poiLat;
                infile >> poiLon;
                
                infile.ignore(10000, '\n');
                
                GeoPoint poi(poiLat, poiLon);
                
                
                //connects location to the midpoint
                m_points[path.to_string()].m_connectedPoints.push_back(poi);
                m_points[poi.to_string()].m_connectedPoints.push_back(path);
                
                //adds location to map of locations
                m_locations[name] = poi;
                
                //labels connection between location and midpoint as "a path"
                m_connects[path.to_string() + " " + poi.to_string()] = "a path";
                
            }
            
            
        }
        
    
    }
    
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const{
    //tries to get point of the location
    GeoPoint* p = m_locations.find(poi);
    
    //if a point was gotten, change given "point" to its value
    if(p){
        point = *p;
        return true;
    }
    return false;
}


std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const{
    std::vector<GeoPoint> ret;
    //adds all connected points in the database to the vector
    for(int i = 0; i < m_points.find(pt.to_string())->m_connectedPoints.size(); i++){
        ret.push_back(m_points.find(pt.to_string())->m_connectedPoints[i]);
    }
    
    return ret;
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const{
    //creates the two different strings for what could be the key, depending on the order
    string key1 = pt1.to_string() + " " + pt2.to_string();
    string key2 = pt2.to_string() + " " + pt1.to_string();
    
    
    string* ret = m_connects.find(key1);
    
    //if the first key matched, then it returns its value
    if(ret){
        return *ret;
    }
    
    ret = m_connects.find(key2);
    
    //if the second key matched, then it returns its value
    if(ret){
        return *ret;
    }
    
    //if neither key had a match, then an empty string is returned
    return "";
}
