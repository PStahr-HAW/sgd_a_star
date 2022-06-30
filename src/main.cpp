#include "../include/a_star.hpp"

int main(int argc, char const *argv[])
{
    a_star::A_Star a;
    std::string log = "/home/pascal/cpp_dev_ws/log/a_star_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".csv";
    a.init_logging(log.c_str(), "I");
    a.load_map("/home/pascal/cpp_dev_ws/src/sgd_a_star/maps/lohmuehlenpark.nav");

    // load user
    a_star::Users usr;
    usr.init("/home/pascal/cpp_dev_ws/src/sgd_a_star/maps/0_users.xml");
    usr.load_user("default");
    a.set_user(&usr);

    // set start and destination
    a.set_start(a_star::LatLon(53.555355, 10.0219594));

    // Berliner Tor 21 (2x), U-Bahn Lohmuehlenstrasse, Wismarer Straße 3, Staftstrasse 69 (hinten), U-Bahn Berliner Tor
    long long dest[] = {3693927703, 313339524, 788416436, 5139452007, 7156477916, 788416750};
    for (auto d : dest)
    {
        a.set_dest(d);
        auto r = a.compute_path();
        std::cout << "Computed a route with " << r.get_waypoints().size() << " waypoints\n";
    }

    return 0;
}