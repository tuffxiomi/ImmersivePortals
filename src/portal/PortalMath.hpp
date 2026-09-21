#pragma once

#include "mod/Config.hpp"

namespace immersive_portals::portal_math {

double dot(Vec3 a, Vec3 b);
Vec3 add(Vec3 a, Vec3 b);
Vec3 sub(Vec3 a, Vec3 b);
Vec3 mul(Vec3 a, double s);
double length(Vec3 a);
Vec3 normalize(Vec3 a);
Vec3 projectOntoFrame(Vec3 point, const PortalDefinition &portal);
Vec3 mapPoint(Vec3 point, const PortalDefinition &from, const PortalDefinition &to);
Vec3 mapVelocity(Vec3 velocity, const PortalDefinition &from, const PortalDefinition &to, double scale);
bool crossesPortal(Vec3 previous, Vec3 current, const PortalDefinition &portal, double epsilon);

} // namespace immersive_portals::portal_math
