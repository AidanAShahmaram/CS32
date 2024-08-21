//
//  geodb.hpp
//  Project4
//
//  Created by Aidan Shahmaram on 3/7/24.
//

#ifndef geodb_hpp
#define geodb_hpp

#include <stdio.h>
#include "geopoint.h"
#include "base_classes.h"
#include "HashMap.h"


class GeoDatabase: public GeoDatabaseBase{
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
    
    
private:
    //struct that holds a geopoint along with a vector of all its connected points
    struct point{
        point(){}
        
        point(GeoPoint loc): m_loc(loc){}
        
        GeoPoint m_loc;
        std::vector<GeoPoint> m_connectedPoints;
    };
    
    //holds a point struct for each pair of coordinates given
    HashMap<point> m_points;
    
    //holds the geopoint of each location
    HashMap<GeoPoint> m_locations;
    
    //holds the attaching streetname or string for each pair of connecting points
    HashMap<std::string> m_connects;
    
    
};

#endif /* geodb_hpp */
