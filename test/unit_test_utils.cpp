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

#include <string>
#include "gtest/gtest.h"

#include "sgd_a_star/utils.hpp"

#define eps 1E-3

TEST(TestUtils, Distance)
{
    a_star::LatLon ll(53.5554544, 10.0218787);

    // Test various distances; JOSM was taken as reference
    EXPECT_NEAR(ll.distance_to(a_star::LatLon(53.5557262, 10.0215752)), 36.308, eps);
    EXPECT_NEAR(ll.distance_to(a_star::LatLon(53.5562882, 10.0216364)), 94.191, eps);
    EXPECT_NEAR(ll.distance_to(a_star::LatLon(53.5539962, 10.0238513)), 208.246, eps);
    EXPECT_NEAR(ll.distance_to(a_star::LatLon(53.5574830, 10.0248699)), 300.201, eps);
}

TEST(TestUtils, Bearing)
{
    a_star::LatLon ll_base(53.5554544, 10.0218787);

    a_star::LatLon ll1(53.5554544, 10.0318787);
    EXPECT_NEAR(ll_base.bearing(ll1), M_PI_2, eps);

    a_star::LatLon ll2(53.5654544, 10.0218787);
    EXPECT_NEAR(ll_base.bearing(ll2), 0.0, eps);
}

TEST(TestUtils, InnerAngle)
{
    a_star::LatLon ll_base(53.5554544, 10.0218787);

    a_star::LatLon ll1(53.5554544, 10.0318787);
    a_star::LatLon ll2(53.5654544, 10.0218787);
    EXPECT_NEAR(ll_base.innerAngle(ll1, ll2), M_PI_2, eps);

    ll_base.set_global_coordinates(53.555686, 10.21379);

    ll1.set_global_coordinates(53.5554544, 10.0318787);
    ll2.set_global_coordinates(53.5557064, 10.213855);
    EXPECT_NEAR(ll_base.innerAngle(ll1, ll2), 45.2/180*M_PI, 0.1);
}

int main(int argc, char **argv)
{
    auto exe = std::string(argv[0]);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}