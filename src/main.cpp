#include <pl/Mod.hpp>
#include "portal/PortalEngine.hpp"
#include <exception>
#include <filesystem>
#include <memory>

class ImmersivePortalsMod {
public:
    static ImmersivePortalsMod &instance() {
        static ImmersivePortalsMod mod;
        return mod;
    }

    ImmersivePortalsMod() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod &getSelf() const { return mSelf; }

    bool load() {
        try {
            auto &self = getSelf();
            std::filesystem::create_directories(self.getConfigDir());
            const auto configPath = self.getConfigDir() / "config.json";
            if (!std::filesystem::exists(configPath)) {
                self.getLogger().error("ImmersivePortals: config.json is missing");
                return false;
            }

            const auto logger = [&self](std::string_view message) {
                self.getLogger().info("{}", message);
            };

            mEngine = std::make_unique<immersive_portals::PortalEngine>();
            if (!mEngine->load(configPath, logger)) return false;
            self.getLogger().info(
                "ImmersivePortals loaded with {} configured portals",
                mEngine->config().portals.size());
            return true;
        } catch (const std::exception &e) {
            getSelf().getLogger().error("ImmersivePortals load exception: {}", e.what());
            return false;
        }
    }

    bool enable() {
        try {
            if (!mEngine) return false;
            auto &self = getSelf();
            const auto logger = [&self](std::string_view message) {
                self.getLogger().info("{}", message);
            };
            return mEngine->start(logger);
        } catch (const std::exception &e) {
            getSelf().getLogger().error("ImmersivePortals enable exception: {}", e.what());
            return false;
        }
    }

    bool disable() {
        try {
            if (!mEngine) return true;
            auto &self = getSelf();
            const auto logger = [&self](std::string_view message) {
                self.getLogger().info("{}", message);
            };
            return mEngine->stop(logger);
        } catch (const std::exception &e) {
            getSelf().getLogger().error("ImmersivePortals disable exception: {}", e.what());
            return false;
        }
    }

    bool unload() {
        mEngine.reset();
        return true;
    }

private:
    ll::mod::NativeMod &mSelf;
    std::unique_ptr<immersive_portals::PortalEngine> mEngine;
};

PL_REGISTER_MOD(ImmersivePortalsMod, ImmersivePortalsMod::instance());
