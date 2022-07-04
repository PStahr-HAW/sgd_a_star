#include "../include/sgd_a_star/utils.hpp"

namespace a_star
{

LatLon::LatLon()
{
    lat_ = 0.0;
    lon_ = 0.0;
}

LatLon::LatLon(const double lat, const double lon) :
    lat_(lat), lon_(lon)
{}

LatLon::~LatLon() {}

void
LatLon::set_global_coordinates(const double lat, const double lon)
{
    lat_ = lat;
    lon_ = lon;
}

double
LatLon::distance_to(const LatLon another_latlon)
{
    double x = (lon_ - another_latlon.lon_) * LATLON_TO_METER * cos((another_latlon.lat_ + lat_) * M_PI/360);
    double y = (lat_ - another_latlon.lat_) * LATLON_TO_METER;
    //return std::hypot(xy.first, xy.second);
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

double
LatLon::distance_to(const double lat, const double lon)
{
    return distance_to(LatLon(lat, lon));
}

double
LatLon::lat() {
    return lat_;
}

double
LatLon::lon() {
    return lon_;
}

double
LatLon::bearing(const LatLon other)
{
    auto ang = atan2((other.lat_-lat_)/180*M_PI, (other.lon_-lon_)/180*M_PI*cos(lat_/180*M_PI)) * -1.0 + M_PI_2;
    if (ang < 0)
    {
        ang += 2*M_PI;
    } else if (ang > 2*M_PI)
    {
        ang -= 2*M_PI;
    }

    return ang;
}

double
LatLon::innerAngle(LatLon other1, LatLon other2)
{
    // calculate bearings
    double b = bearing(other1) - bearing(other2);

    if (abs(b) > M_PI)
    {
        b += (b > 0 ? -2.0 : 2.0) * M_PI;
    }
    return b;
}

std::string
LatLon::to_string() {
    const char *out = "%3.7f; %3.7f";
    int sz = std::snprintf(nullptr, 0, out, lat_, lon_);
    char buf[sz + 1]; // note +1 for null terminator
    std::snprintf(&buf[0], sz+1, out, lat_, lon_);
    return std::string(buf);
}

}   // namespace a_star