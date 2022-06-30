#include "../include/a_star.hpp"

namespace a_star
{
    void
    A_Star::init()
    {

    }

    void
    A_Star::init_logging(const char * log_file, const char *severity)
    {
        plog::init(plog::severityFromString(severity), log_file);
    }

    void
    A_Star::reset()
    {
        // destructor
    }

    void
    A_Star::load_map(std::string nav_filename)
    {
        PLOGI << "Load map file: " << nav_filename;
        nodelist_.clear();

        tinyxml2::XMLDocument doc;
        doc.LoadFile(nav_filename.c_str());

        // check if errors occured
        if (doc.ErrorID())
        {
            PLOGE << doc.ErrorStr();
            return;
        }

        if (doc.RootElement() == NULL)
        {
            PLOGE << "Error reading map file.";
            return;
        }

        // load all nodes to unordered map
        PLOGV << "Message: id; <lat, lon>; pid";
        tinyxml2::XMLElement *node = doc.RootElement()->FirstChildElement("node");
        while (node != NULL)
        {
            auto id = strtoll(node->Attribute("id"), NULL, 10);
            Node n(id, LatLon(strtod(node->Attribute("lat"), NULL), strtod(node->Attribute("lon"), NULL)));

            // parent osm id
            auto pid = node->Attribute("pid");
            n.pid = pid != NULL ? strtoll(pid, NULL, 10) : 0;
            PLOGV << "Imported node " << id << "; " << n.get_latlon().to_string() << "; " << pid;

            nodelist_.insert({id, n});
            node = node->NextSiblingElement();
        }

        // add neighbours and paths to nodes
        node = doc.RootElement()->FirstChildElement("node");
        PLOGV << "Message: node id; ref id; roadside; angle to ref";
        while (node != NULL)
        {
            auto id = strtoll(node->Attribute("id"), NULL, 10);     // osm id
            auto current_node = &nodelist_.at(id);

            tinyxml2::XMLElement *nd1 = node->FirstChildElement("nd"); // get first neighbor node
            while (nd1)
            {
                // get ref from <nd ref="xxxx">
                auto ref_id1 = strtoll(nd1->Attribute("ref"), NULL, 10);
                if (nodelist_.count(ref_id1) > 0)
                {
                    // ref_id is contained in nodelist
                    auto ref_node = &nodelist_.at(ref_id1);
                    
                    // get tags from nd and add to path
                    auto tag = nd1->FirstChildElement();
                    std::vector<std::string> attributes;
                    while (tag)
                    {
                        attributes.push_back(std::string(tag->Value()) + ":" + std::string(tag->GetText()));
                        tag = tag->NextSiblingElement();
                    }

                    // calculate angle
                    double angle = 0.0;
                    if ((current_node->pid > 0) == (ref_node->pid > 0))
                    {
                        // angle is 0
                        angle = 0.0;
                    }
                    else if (current_node->pid > 0)
                    {
                        auto ll_parent = nodelist_.at(current_node->pid).get_latlon();
                        angle = ref_node->get_latlon().innerAngle(ll_parent, current_node->get_latlon());
                    }
                    else if (ref_node->pid > 0)
                    {
                        auto ll_parent = nodelist_.at(ref_node->pid).get_latlon();
                        angle = current_node->get_latlon().innerAngle(ll_parent, ref_node->get_latlon());
                    }

                    // side of road
                    std::string ids = std::to_string(id) + ";" + std::to_string(ref_id1) + ";";
                    int8_t side = 0;
                    if (angle != 0.0)
                    {
                        side = angle > 0 ? 1 : -1;
                    }
                    else if (current_node->pid < 1 && ref_node->pid < 1)
                    {
                        // both nodes are in the middle of the road
                        side = 0;
                    }
                    else
                    {
                        // way on the side
                        auto ll_parent = nodelist_.at(current_node->pid).get_latlon();
                        side = (current_node->get_latlon().innerAngle(ll_parent, ref_node->get_latlon())) > 0 ? 2 : -2;
                        PLOGV << "Angle: " << current_node->get_latlon().innerAngle(ll_parent, ref_node->get_latlon());
                    }

                    PLOGV << "Add neighbour: " << ids << std::to_string(side) << "; " << angle;
                    nodelist_.at(id).add_neighbour(ref_id1, attributes, angle, side);
                }
                else
                {
                    PLOGI << "Node "  << std::to_string(ref_id1) << " is specified as reference but could not be found in nodelist.";
                }

                nd1 = nd1->NextSiblingElement();
            }

            node = node->NextSiblingElement();
        }
        PLOGI << "Map file successfully loaded!";
    }

    void
    A_Star::set_start(LatLon start)
    {
        start_node_ = node_near_lat_lon(start);
    }

    void 
    A_Star::set_dest(LatLon dest)
    {
        dest_node_ = node_near_lat_lon(dest);
    }

    void
    A_Star::set_dest(llong node_id)
    {
        if (nodelist_.count(node_id) > 0)
        {
            dest_node_ = node_id;
        }
        else
        {
            PLOGI << "Node with id " << std::to_string(node_id) << " not found in nodelist.";
        }
    }

    void
    A_Star::set_user(Users *usr)
    {
        // set user
        user_ = usr;
    }

    Route
    A_Star::compute_path()
    {
        PLOGI << "Compute path to node " << std::to_string(dest_node_);
        if (nodelist_.count(start_node_) < 1 || nodelist_.count(dest_node_) < 1)
        {
            throw std::invalid_argument("Start or destination node not set!");
        }

        auto t_beg = std::chrono::system_clock::now();
        // define comparator for openlist and initialize closed list
        auto compare = [](Node a, Node b) { return a.f() > b.f(); };
        std::priority_queue<Node, std::vector<Node>, decltype(compare)> openList(compare);
        closedList.clear();

        // push start node to openList
        PLOGD << "Expand message: next_id; distance; cost; angle; ang_cost; side; side_cost; g; h; f";
        PLOGI << "Push start node to openlist: " << std::to_string(nodelist_.at(start_node_).id());
        Node start(nodelist_.at(start_node_));
        openList.push(start);
        auto dest_ll = nodelist_.at(dest_node_).get_latlon();   // position of destination node

        // initially the destination is not reached
        while (!openList.empty())
        {
            // get node with lowest f score from openlist and remove node afterwards
            auto current_node = openList.top();
            PLOGD << "Expand node " << std::to_string(current_node.id());
            openList.pop();
            if (closedList.count(current_node.id()) > 0)   continue;
            PLOGD << "Best path: " << log_path(closedList, current_node);

            // if current node is destination, terminate algorithm and reconstruct path
            if (current_node.id() == dest_node_)
            {
                PLOGI << "Found destination node.";
                closedList.insert({current_node.id(), current_node});

                auto t_end = std::chrono::system_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_beg);
                PLOGI << "Path computation took " << std::to_string(duration.count()) << "ms";
                return trace_path(closedList, current_node);
            }

            // get all paths from current node
            // add destination nodes to openList
            for (auto p : current_node.get_neighbours())
            {
                // set next node to path destination
                Node next_node(nodelist_.at(p));
                if (closedList.count(p) < 1 && !next_node.is_blocked())
                {
                    // node is not already on closed list
                    double h = next_node.get_latlon().distance_to(dest_ll);
                    next_node.set_h(h);

                    auto g = next_node.set_predecessor(&current_node, user_);
                    openList.push(next_node);
                }
            }
            
            PLOGD << "Add " << std::to_string(current_node.id()) << " to closedList";
            closedList.insert({current_node.id(), current_node});
        }
        PLOGW << "Failed to compute path to destination.";

        Route route;

        auto t_end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_beg);
        PLOGI << "Path computation took " << std::to_string(duration.count()) << "ms";
        return route;
    }

    llong
    A_Star::node_near_lat_lon(LatLon latlon)
    {
        PLOGI << "Try to find node near " << latlon.to_string();
        double lastDist = 10000.1;
        llong id_;
        for (auto nd : nodelist_)
        {
            double lastDist_ = latlon.distance_to(nd.second.get_latlon());
            if (lastDist_ < lastDist)
            {
                lastDist = lastDist_;
                PLOGD << "Found node " << std::to_string(nd.first) << " (" << nd.second.get_latlon().to_string() << ") in " << std::to_string(lastDist) << "m distance.";
                id_ = nd.first;
            }
        }
        if (lastDist >= 100.0)  // maximum distance to nearest node
        {
            throw std::logic_error("Could not find node near " + latlon.to_string());
        }
        PLOGI << "Found node " << std::to_string(id_) << " in " << std::to_string(lastDist) << "m distance.";
        return id_;
    }

    Route
    A_Star::trace_path(std::unordered_map<llong, Node> closedList, Node dest)
    {
        std::stack<Node> path;

        llong id = dest.id();
        path.push(dest);

        while (id > 0)
        {
            path.push(closedList.at(id));
            id = closedList.at(id).predecessor_id();
        }

        PLOGI << "waypoints: lat; lon";
        Route route;
        while (!path.empty())
        {
            auto ll = path.top().get_latlon();
            
            route.add_waypoint(ll);
            PLOGI << ll.to_string();

            path.pop();
        }

        return route;
    }

    std::string
    A_Star::log_path(std::unordered_map<llong, Node> closedList, Node dest)
    {
        
        std::string route(std::to_string(dest.id()));

        llong id = dest.predecessor_id();
        while (id > 0)
        {
            route = std::to_string(id) + " -> " + route;
            id = closedList.at(id).predecessor_id();
        }
        return route;
    }

} // namespace a_star