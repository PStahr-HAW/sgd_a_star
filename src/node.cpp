#include "../include/node.hpp"

namespace a_star
{

double
Node::cost_to_node(llong id, Users *user)
{
    for (Node::path p : paths)
    {
        if (p.dest_node == id)
        {
            double c = 1.0;
            for (auto att : p.attributes)
            {
                // calculate cost
                c *= user->get_factor(att);
            }
            c *= (1.0+p.side*0.25) * user->get_angle_factor(p.angle);

            PLOGV << p.angle << ";" << user->get_angle_factor(p.angle) << ";" << std::to_string(p.side) << ";"
                    << std::to_string(1.0+p.side*0.125) << ";" << c;
            return c;
        }
    }
    return 10.0;
}

void
Node::add_neighbour(llong dest_id, std::vector<std::string> attributes, double angle, int8_t roadside)
{
    auto p = Node::path(dest_id);
    for (auto a : attributes)   // warum funktioniert std::copy nicht?
    {
        p.attributes.push_back(a);
    }
    p.angle = angle;
    p.side = roadside;
    paths.push_back(p);
}

std::vector<llong>
Node::get_neighbours()
{
    std::vector<llong> n;
    for (auto p : paths)
    {
        n.push_back(p.dest_node);
    }
    return n;
}

void
Node::set_h(double h)
{
    h_ = h;
}

double
Node::f()
{
    return g_ + h_;
}

llong
Node::id()
{
    return id_;
}

double
Node::set_predecessor(Node *predecessor, Users *user)
{
    // get predecessor from neighbours
    predecessor_id_ = predecessor->id();

    double cost = predecessor->cost_to_node(id(), user);
    g_ = predecessor->g_ + latlon_.distance_to(predecessor->latlon_) * cost;

    // log message: next_id; distance; cost; g; h; f;
    PLOGD << id() << ";" << latlon_.distance_to(predecessor->latlon_) << ";" << cost << ";" << g_ << ";" << h_ << ";" << f();

    return g_;
}

bool
Node::is_blocked()
{
    return is_blocked_;
}

llong
Node::predecessor_id()
{
    return predecessor_id_;
}

LatLon
Node::get_latlon()
{
    return latlon_;
}

} // namespace a_star