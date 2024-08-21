//
//  tour_generator.hpp
//  Project4
//
//  Created by Aidan Shahmaram on 3/11/24.
//

#ifndef tour_generator_hpp
#define tour_generator_hpp

#include <stdio.h>
#include "base_classes.h"

class TourGenerator: public TourGeneratorBase{
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator(){}
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
    
private:
    const GeoDatabaseBase& m_geo;
    const RouterBase& m_router;
    
    
    //returns direction of an angle
    std::string dirOfTravel(const double angle) const{
        if(angle < 0 || angle > 360)
            return "";
        
        if(angle < 22.5)
            return "east";
        else if(angle < 67.5)
            return "northeast";
        else if(angle < 112.5)
            return "north";
        else if(angle < 157.5)
            return "northwest";
        else if(angle < 202.5)
            return "west";
        else if(angle < 247.5)
            return "southwest";
        else if(angle < 292.5)
            return "south";
        else if(angle < 337.5)
            return "southeast";
        else
            return "east";
    }
};


#endif /* tour_generator_hpp */
