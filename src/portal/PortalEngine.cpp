#include "portal/PortalEngine.hpp"

namespace immersive_portals {

bool PortalEngine::load(const std::filesystem::path &configPath, LogFn logger) {
    std::string error;
    if (!loadConfig(configPath, mConfig, error)) {
        mStatus = "config load failed: " + error;
        if (logger) logger(mStatus);
        return false;
    }

    // Validate destination references up front so malformed portal graphs fail
    // during load instead of during a live teleport.
    for (const auto &portal : mConfig.portals) {
        bool foundDestination = false;
        for (const auto &candidate : mConfig.portals) {
            if (candidate.id == portal.destination) {
                foundDestination = true;
                break;
            }
        }
        if (!foundDestination) {
            mStatus = "config load failed: portal '" + portal.id +
                      "' references unknown destination '" + portal.destination + "'";
            if (logger) logger(mStatus);
            return false;
        }
    }

    mStatus = "config loaded; " + std::to_string(mConfig.portals.size()) + " portal definitions";
    if (logger) logger(mStatus);
    return true;
}

bool PortalEngine::start(LogFn logger) {
    if (!mConfig.enabled) {
        mStatus = "disabled by config";
        if (logger) logger(mStatus);
        return true;
    }

    mAdapter = std::make_unique<GameAdapter>(std::move(logger));
    if (!mAdapter->install()) {
        mStatus = "game adapter failed";
        return false;
    }
    mStatus = mAdapter->status();
    return true;
}

bool PortalEngine::stop(LogFn logger) {
    if (mAdapter) mAdapter->uninstall();
    mAdapter.reset();
    mStatus = "stopped";
    if (logger) logger(mStatus);
    return true;
}

} // namespace immersive_portals
