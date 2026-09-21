#pragma once

#include "mod/Config.hpp"
#include "portal/PortalMath.hpp"
#include "game/GameAdapter.hpp"
#include <filesystem>
#include <memory>
#include <string>

namespace immersive_portals {

class PortalEngine {
public:
    using LogFn = GameAdapter::LogFn;

    PortalEngine() = default;

    bool load(const std::filesystem::path &configPath, LogFn logger);
    bool start(LogFn logger);
    bool stop(LogFn logger);

    [[nodiscard]] const Config &config() const noexcept { return mConfig; }
    [[nodiscard]] const std::string &status() const noexcept { return mStatus; }

private:
    Config mConfig;
    std::string mStatus{"not loaded"};
    std::unique_ptr<GameAdapter> mAdapter;
};

} // namespace immersive_portals
