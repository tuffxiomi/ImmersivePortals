#include "portal/PortalMath.hpp"
#include <cmath>

namespace immersive_portals::portal_math {

double dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
Vec3 add(Vec3 a, Vec3 b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
Vec3 sub(Vec3 a, Vec3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
Vec3 mul(Vec3 a, double s) { return {a.x*s, a.y*s, a.z*s}; }
double length(Vec3 a) { return std::sqrt(dot(a, a)); }
Vec3 normalize(Vec3 a) {
    const double n = length(a);
    return n > 1e-12 ? mul(a, 1.0/n) : Vec3{};
}

Vec3 projectOntoFrame(Vec3 point, const PortalDefinition &portal) {
    const Vec3 local = sub(point, portal.center);
    return {dot(local, normalize(portal.right)),
            dot(local, normalize(portal.up)),
            dot(local, normalize(portal.normal))};
}

Vec3 mapPoint(Vec3 point, const PortalDefinition &from, const PortalDefinition &to) {
    const Vec3 local = projectOntoFrame(point, from);
    const Vec3 r = normalize(to.right);
    const Vec3 u = normalize(to.up);
    const Vec3 n = normalize(to.normal);
    // Preserve the signed distance but reverse the portal-forward component so
    // a traveller exiting the destination moves away from its portal plane.
    return add(to.center, add(add(mul(r, local.x), mul(u, local.y)), mul(n, -local.z)));
}

Vec3 mapVelocity(Vec3 velocity, const PortalDefinition &from, const PortalDefinition &to, double scale) {
    const Vec3 r0 = normalize(from.right), u0 = normalize(from.up), n0 = normalize(from.normal);
    const Vec3 r1 = normalize(to.right), u1 = normalize(to.up), n1 = normalize(to.normal);
    const double x = dot(velocity, r0), y = dot(velocity, u0), z = dot(velocity, n0);
    return mul(add(add(mul(r1, x), mul(u1, y)), mul(n1, -z)), scale);
}

bool crossesPortal(Vec3 previous, Vec3 current, const PortalDefinition &portal, double epsilon) {
    const Vec3 n = normalize(portal.normal);
    const double a = dot(sub(previous, portal.center), n);
    const double b = dot(sub(current, portal.center), n);
    if (!((a > epsilon && b < -epsilon) || (a < -epsilon && b > epsilon))) return false;
    const double t = a / (a - b);
    const Vec3 hit = add(previous, mul(sub(current, previous), t));
    const Vec3 local = projectOntoFrame(hit, portal);
    return std::abs(local.x) <= portal.width * 0.5 &&
           std::abs(local.y) <= portal.height * 0.5;
}

} // namespace immersive_portals::portal_math
