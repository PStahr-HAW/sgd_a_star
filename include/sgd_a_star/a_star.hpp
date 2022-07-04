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

#ifndef NAV_SGD_A_STAR_HPP_
#define NAV_SGD_A_STAR_HPP_

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <stack>
#include <map>
#include <unordered_map>
#include <queue>

#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

#include "tinyxml2.h"
#include "users.hpp"
#include "node.hpp"
#include "utils.hpp"
#include "route.hpp"

namespace a_star
{

typedef int64_t llong;  // format used to store the ids

class A_Star
{

public:

    /**
     * @brief 
     * 
     * @param nav_filename 
     */
    void load_map(std::string nav_filename);

    /**
     * @brief Initialize a*
     */
    void init();

    /**
     * @brief Initialize logging
     */
    void init_logging(const char * log_file, const char *severity);

    /**
     * @brief 
     * 
     */
    void reset();

    /**
     * @brief Set the start
     * 
     * @param start 
     */
    void set_start(LatLon start);

    /**
     * @brief Set the destination
     * 
     * @param dest 
     */
    void set_dest(LatLon dest);

    /**
     * @brief Set the destination by id
     * 
     * @param node_id 
     */
    void set_dest(llong node_id);

    /**
     * @brief Set the user
     * 
     * @param usr 
     */
    void set_user(Users *usr);

    /**
     * @brief Compute the optimal path from a starting point to a destination point.
     * 
     * @return true if path computation was succesful, false otherwise
     */
    Route compute_path();

private:
    std::unordered_map<llong, Node> nodelist_;
    Users *user_;

    // start and destination nodes
    llong start_node_, dest_node_;
    std::unordered_map<llong, Node> closedList;

    /**
     * @brief Find the nearest node to the specified lat/lon
     * 
     * @param latlon 
     * @return node id
     */
    llong node_near_lat_lon(LatLon latlon);

    /**
     * @brief Traces the path and outputs all nodes in the correct order
     * 
     * @param closedList computed nodes from a star
     * @param dest destination node
     */
    Route trace_path(std::unordered_map<llong, Node> closedList, Node dest);

    /**
     * @brief Helper function for logging
     * 
     * @param closedList 
     * @param dest 
     * @return std::string 
     */
    std::string log_path(std::unordered_map<llong, Node> closedList, Node dest);

};

}   // namespace a_star

#endif