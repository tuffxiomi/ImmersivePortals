#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace immersive_portals {

struct Vec3 {
    double x{};
    double y{};
    double z{};
};

struct PortalDefinition {
    std::string id;
    int dimension{};
    Vec3 center{};
    Vec3 normal{1.0, 0.0, 0.0};
    Vec3 right{0.0, 0.0, 1.0};
    Vec3 up{0.0, 1.0, 0.0};
    double width{2.0};
    double height{3.0};
    std::string destination;
};

struct Config {
    bool enabled{true};
    bool seamlessTeleport{true};
    bool portalView{false};
    int maxRecursion{5};
    int crossingCooldownMs{350};
    double portalTriggerEpsilon{0.05};
    double gravityScale{1.0};
    std::vector<PortalDefinition> portals;
};

bool loadConfig(const std::filesystem::path &path, Config &out, std::string &error);

} // namespace immersive_portals
