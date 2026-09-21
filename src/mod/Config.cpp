#include "mod/Config.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace immersive_portals {
namespace {

Vec3 readVec3(const nlohmann::json &j, const char *key, Vec3 fallback) {
    if (!j.contains(key) || !j.at(key).is_array() || j.at(key).size() != 3) return fallback;
    return {j.at(key)[0].get<double>(), j.at(key)[1].get<double>(), j.at(key)[2].get<double>()};
}

} // namespace

bool loadConfig(const std::filesystem::path &path, Config &out, std::string &error) {
    try {
        std::ifstream in(path);
        if (!in) {
            error = "cannot open config: " + path.string();
            return false;
        }
        const auto j = nlohmann::json::parse(in);
        Config cfg;
        cfg.enabled = j.value("enabled", true);
        cfg.seamlessTeleport = j.value("seamlessTeleport", true);
        cfg.portalView = j.value("portalView", false);
        cfg.maxRecursion = j.value("maxRecursion", 5);
        cfg.crossingCooldownMs = j.value("crossingCooldownMs", 350);
        cfg.portalTriggerEpsilon = j.value("portalTriggerEpsilon", 0.05);
        cfg.gravityScale = j.value("gravityScale", 1.0);
        if (j.contains("portals") && j.at("portals").is_array()) {
            for (const auto &p : j.at("portals")) {
                PortalDefinition d;
                d.id = p.value("id", "");
                d.dimension = p.value("dimension", 0);
                d.center = readVec3(p, "center", d.center);
                d.normal = readVec3(p, "normal", d.normal);
                d.right = readVec3(p, "right", d.right);
                d.up = readVec3(p, "up", d.up);
                d.width = p.value("width", d.width);
                d.height = p.value("height", d.height);
                d.destination = p.value("destination", "");
                if (d.id.empty() || d.destination.empty() || d.width <= 0 || d.height <= 0) {
                    error = "invalid portal definition";
                    return false;
                }
                cfg.portals.push_back(std::move(d));
            }
        }
        out = std::move(cfg);
        return true;
    } catch (const std::exception &e) {
        error = e.what();
        return false;
    }
}

} // namespace immersive_portals
