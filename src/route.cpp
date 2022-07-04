#include "../include/sgd_a_star/route.hpp"

namespace a_star
{

Route::Route()
{
    length_m = 0.0;
}

Route::~Route() {}

void
Route::add_waypoint(LatLon wp)
{
    

    if (waypoints.size() > 0)
    {
        // at least 1 waypoint is already in the list
        length_m += waypoints.back().distance_to(wp);
    }

    waypoints.push_back(wp);
}

std::vector<LatLon>
Route::get_waypoints()
{
    return waypoints;
}

double
Route::length()
{
    return length_m;
}

} // namespace a_star
