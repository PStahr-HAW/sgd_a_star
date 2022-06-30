#include "../include/users.hpp"

namespace a_star
{

void
Users::init(std::string user_filename)
{
    user_filename_ = user_filename;
}

void
Users::load_user(const char * username)
{
    // read xml
    tinyxml2::XMLDocument doc;
    doc.LoadFile(user_filename_.c_str());
    auto root = doc.RootElement();      // <users>

    // check error
    if (doc.ErrorID())
    {
        // TODO Error handling
        std::cerr << doc.ErrorStr();
        return;
    }

    // go through xml and add users
    user_.clear();
    //bool has_user = false;
    auto usr = root->FirstChildElement("user");
    while (usr != NULL)
    {
        // check if Attribute("name") == NULL (default user) or Attribute("name") == username
        bool is_default_ = (usr->Attribute("name") == NULL);
        if (usr->Attribute("name", username) || is_default_)
        {
            // load user
            auto nd = usr->FirstChildElement();
            while (nd != NULL)
            {
                // if (nd->FirstChildElement())
                // {
                    std::string prefix = nd->Value();
                    // gehe durch alle child nodes
                    auto nn = nd->FirstChildElement();
                    while (nn != NULL)
                    {
                        std::string key = prefix + ":" + nn->Value();
                        if ((is_default_ && !user_.count(key)) || !is_default_)
                        {
                            // prevent default user from overriding user specific factors
                            try
                            {
                                std::cout << "Add factor " << key << std::endl;
                                double d = std::stod(nn->GetText());
                                user_.insert({key, d});
                            }
                            catch(const std::invalid_argument& e)
                            {
                                std::cout << "Could not add factor " << key << std::endl;
                                std::cerr << e.what() << '\n';
                            }
                        }

                        nn = nn->NextSiblingElement();
                    }
                //}
                nd = nd->NextSiblingElement();
            }
        }
        usr = usr->NextSiblingElement();
    }
}

double
Users::get_factor(std::string key)
{
    // try to get factor for current user
    if (user_.count(key) > 0)
    {
        return user_.at(key);
    }
    std::cout << "Key " << key << " not found" << std::endl;
    return 10.0;
}

double
Users::get_angle_factor(double angle)
{
    double a = get_factor("angle:a");
    double b = get_factor("angle:b");
    double c = get_factor("angle:c");

    return (a * exp(-pow(b * angle, 2)) * angle + c * tanh(5.0 * angle) + 1);
}

}       // namespace a_star