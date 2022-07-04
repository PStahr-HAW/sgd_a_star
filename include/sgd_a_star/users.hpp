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

#ifndef A_STAR_USERS_HPP_
#define A_STAR_USERS_HPP_

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "tinyxml2.h"

namespace a_star
{

class Users
{

private:
    std::string user_filename_;

    // map holding factors for path computation
    std::unordered_map<std::string, double> user_;

public:

    void init(std::string user_filename);

    /**
     * @brief Set current user to username. If username is not a valid user set default user.
     * 
     * @param username The username of the user
     */
    void load_user(const char * username);

    /**
     * @brief Calculate factor for given key
     * 
     * @param key 
     * @param value 
     * @return double the factor
     */
    double get_factor(std::string key);

    /**
     * @brief Calculate the factor for the specified angle
     * 
     * @param angle 
     * @return double 
     */
    double get_angle_factor(double angle);
};

} // namespace a_star


#endif