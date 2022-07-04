#include <vector>

#include "utils.hpp"

namespace a_star
{

class Route
{
private:
    double cost;
    double length_m;
    std::vector<LatLon> waypoints;
public:
    /**
     * @brief 
     * 
     */
    Route();

    /**
     * @brief Destroy the Route object
     * 
     */
    ~Route();

    /**
     * @brief Add a waypoint to this route
     * 
     * @param wp 
     */
    void add_waypoint(LatLon wp);

    /**
     * @brief Get the list of waypoints
     * 
     * @return std::vector<LatLon> 
     */
    std::vector<LatLon> get_waypoints();

    /**
     * @brief Returns the length of the route in meters.
     * 
     * @return double 
     */
    double length();
};
    
} // namespace a_star
