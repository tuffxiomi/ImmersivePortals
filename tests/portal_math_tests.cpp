#include "portal/PortalMath.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace immersive_portals;
using namespace immersive_portals::portal_math;

static void near(double a, double b) { assert(std::abs(a - b) < 1e-9); }

int main() {
    PortalDefinition a{"a", 0, {0,0,0}, {1,0,0}, {0,0,1}, {0,1,0}, 2, 4, "b"};
    PortalDefinition b{"b", 0, {10,0,0}, {-1,0,0}, {0,0,-1}, {0,1,0}, 2, 4, "a"};

    assert(crossesPortal({-1,0,0}, {1,0,0}, a, 0.05));
    assert(!crossesPortal({-1,3,0}, {1,3,0}, a, 0.05));

    // Source local coordinates: right=+Z, up=+Y, normal=+X.
    // Destination frame: right=-Z, up=+Y, normal=-X, with forward reversed.
    auto p = mapPoint({0.5, 1.0, 2.0}, a, b);
    near(p.x, 10.5); near(p.y, 1.0); near(p.z, -2.0);

    auto v = mapVelocity({2,3,4}, a, b, 1.0);
    near(v.x, 2.0); near(v.y, 3.0); near(v.z, -4.0);

    std::cout << "portal math tests passed\n";
    return 0;
}
