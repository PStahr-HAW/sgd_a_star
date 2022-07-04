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

#ifndef A_STAR_NODE_HPP_
#define A_STAR_NODE_HPP_

#include <unordered_map>
#include <memory>

#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

#include "users.hpp"
#include "utils.hpp"

namespace a_star
{
typedef int64_t llong;

class Node
{

private:
    class path
    {
        public:
            path(llong dest_id)
            {
                dest_node = dest_id;
            }

            llong dest_node = 0;
            double angle = 0.0;
            int8_t side = 0;
            std::vector<std::string> attributes;
    };

    llong id_, pid_;    // osm id and parent id
    bool is_blocked_;
    LatLon latlon_;
    
    llong predecessor_id_ = 0;
    double g_ = 0, h_ = 0;

    std::vector<Node::path> paths;

    double cost_to_node(llong id, Users *user);

public:
    // A_Star_Node() = default;
    Node(llong id, LatLon coords)
            : id_(id), latlon_(coords), is_blocked_(false) { }
    ~Node() = default;

    // define variables
    llong pid;

    /**
     * @brief 
     * 
     * @param dest_id 
     * @param attributes 
     * @param angle 
     */
    void add_neighbour(llong dest_id, std::vector<std::string> attributes, double angle, int8_t roadside);

    /**
     * @brief Get a list containing all neighbour ids
     * 
     * @return std::vector<llong> 
     */
    std::vector<llong> get_neighbours();

    /**
     * @brief Set the value for the heuristic
     * 
     * @param h 
     */
    void set_h(double h);

    /**
     * @brief 
     * 
     * @return double 
     */
    double f();

    /**
     * @brief 
     * 
     * @return llong 
     */
    llong id();

    /**
     * @brief Set the predecessor node. Calculates g and h.
     * 
     * @param predecessor the predecessing node
     * @return g (cost to predecessor)
     */
    double set_predecessor(Node *predecessor, Users *user);

    /**
     * @brief Check if this node has the "blocked" attribute set
     * 
     * @return true if this node is blocked, otherwise false
     */
    bool is_blocked();

    /**
     * @brief 
     * 
     * @return long 
     */
    llong predecessor_id();   
    
    /**
     * @brief Get the coordinates for this node.
     * 
     * @return sgd_util::LatLon 
     */
    LatLon get_latlon();

    inline bool operator==(const Node& nd) { return id_ == nd.id_; }
    inline bool operator!=(const Node& nd) { return id_ != nd.id_; }
};

} // namespace a_star

#endif