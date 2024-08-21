//
//  tour_generator.cpp
//  Project4
//
//  Created by Aidan Shahmaram on 3/11/24.
//

#include "tour_generator.h"
#include "stops.h"
#include "tourcmd.h"
#include "router.h"
#include "geopoint.h"
#include "base_classes.h"
using namespace std;



TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router): m_geo(geodb), m_router(router){}


std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const{
    vector<TourCommand> res;
    
    //for each stop
    for(int i = 0; i < stops.size(); i++){
        //create a command for the current stop, and add it to the returning vector
        TourCommand cmd;
        string poi, commentary;
        stops.get_poi_data(i, poi, commentary);
        
        cmd.init_commentary(poi, commentary);
        
        res.push_back(cmd);
        
        //if there is a next stop
        if(i + 1 < stops.size()){
            
            //get the name of the next location
            string poi2;
            stops.get_poi_data(i + 1, poi2, commentary);
            
            GeoPoint start;
            GeoPoint end;
            
            //assign the geopoints of the previous and next location
            m_geo.get_poi_location(poi, start);
            m_geo.get_poi_location(poi2, end);
            
            //get a route between these two locations
            std::vector<GeoPoint> path = m_router.route(start, end);
            
            //for each point within the route created
            for(int i = 0; i < path.size() - 1; i++){
                //create and add a proceed command
                TourCommand procCmd;
                procCmd.init_proceed(dirOfTravel(angle_of_line(path[i], path[i + 1])), m_geo.get_street_name(path[i], path[i + 1]), distance_earth_miles(path[i], path[i + 1]), path[i], path[i + 1]);
                
                res.push_back(procCmd);
                
                //if there is another segment to analyze after the current one
                if(i + 2 < path.size()){
                    
                    //if they are on different streets and there is a turn angle, add a turn command
                    double turn_angle = angle_of_turn(path[i], path[i + 1], path[i + 2]);
                    if((m_geo.get_street_name(path[i], path[i + 1]) != m_geo.get_street_name(path[i + 1], path[i + 2]))
                       && turn_angle != 0){
                        TourCommand turnCmd;
                        string turnSign = "right";
                        if(turn_angle < 180)
                            turnSign = "left";
                        turnCmd.init_turn(turnSign, m_geo.get_street_name(path[i + 1], path[i + 2]));
                        res.push_back(turnCmd);
                    }
                }
            }
            
            
        }
    }
    
    return res;
}


