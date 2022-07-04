// Copyright 2022 HAW Hamburg
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#ifndef A_STAR_UTILS_HPP_
#define A_STAR_UTILS_HPP_

#include <string>
#include <vector>
#include <math.h>
#include <iostream>

namespace a_star
{

class LatLon
{
private:
    static constexpr double LATLON_TO_METER = M_PI / 180.0 * 6378137.0;
    static constexpr double METER_TO_LATLON = 1.0 / LATLON_TO_METER;

    double lat_, lon_;

public:
    /**
     * @brief Construct a new Lat Lon object
     * 
     */
    LatLon();

    /**
     * @brief Construct a new Lat Lon object
     * 
     * @param lat 
     * @param lon 
     */
    LatLon(double lat, double lon);

    /**
     * @brief Destroy the Lat Lon object
     * 
     */
    ~LatLon();

    /**
     * @brief 
     * 
     * @return double 
     */
    double lat();

    /**
     * @brief 
     * 
     * @return double 
     */
    double lon();

    /**
     * @brief Set the global coordinates object
     * 
     * @param lat 
     * @param lon 
     */
    void set_global_coordinates(double lat, double lon);

    /**
     * @brief Calculate the euclidian distance from this Lat/Lon to the other Lat/Lon and returns the
     * distance in meters.
     * 
     * @param another_latlon the other Lat/Lon
     * @return double the distance
     */
    double distance_to(LatLon another_latlon);

    /**
     * @brief Calculate the euclidian distance from this Lat/Lon to the other Lat/Lon and returns the
     * distance in meters.
     * 
     * @param lat 
     * @param lon 
     * @return double 
     */
    double distance_to(double lat, double lon);

    /**
     * @brief Calculates the bearing between the path from this position to the other position. The north axis is defined as 0,
     * z-axis is pointing downwards
     * 
     * @param other 
     * @return bearing in the interval [0;2*PI]
     */
    double bearing(LatLon other);

    /**
     * @brief Calculates the inner angle between the paths from the current position to the specified positions.
     * Z-axis is pointing downwards
     * 
     * @param other1
     * @param other2 description
     * @return angle in the interval [-PI;PI]
     */
    double innerAngle(LatLon other1, LatLon other2);

    /**
     * @brief Create string <lat, lon> with 7 digits precision
     * 
     * @return string
     */
	std::string to_string();
};

}   // namespace a_star

#endif  // A_STAR_UTILS_HPP_