//
//  router.hpp
//  Project4
//
//  Created by Aidan Shahmaram on 3/10/24.
//

#ifndef router_hpp
#define router_hpp

#include <stdio.h>
#include "base_classes.h"
#include "geodb.h"
#include "geopoint.h"
#include "geotools.h"

class Router: public RouterBase{
public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router() {}
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
    
private:
    const GeoDatabaseBase& m_geo;
    
    bool aStarComp(const GeoPoint& start, const GeoPoint& end, const GeoPoint& pt1, const GeoPoint pt2) const{
        double val1 = distance_earth_miles(start, pt1) + distance_earth_miles(pt1, end);
        double val2 = distance_earth_miles(start, pt2) + distance_earth_miles(pt2, end);
        
        if(val1 < val2)
            return true;
         else
             return false;
    }
    
    struct Compare{
        bool operator() (const GeoPoint& a, const GeoPoint& b);
    };
    
    
};

#endif /* router_hpp */
