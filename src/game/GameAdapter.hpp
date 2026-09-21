#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <pl/memory/Hook.hpp>
#include <pl/memory/Signature.hpp>

namespace immersive_portals {

class GameAdapter {
public:
    using LogFn = std::function<void(std::string_view)>;

    explicit GameAdapter(LogFn logger) : mLog(std::move(logger)) {}

    bool verifyTarget();
    bool install();
    void uninstall();

    [[nodiscard]] bool targetVerified() const noexcept { return mTargetVerified; }
    [[nodiscard]] bool hooksReady() const noexcept { return mHooksReady; }
    [[nodiscard]] const std::string &status() const noexcept { return mStatus; }

private:
    void log(std::string_view msg) {
        if (mLog) mLog(msg);
    }

    bool mTargetVerified{false};
    bool mHooksReady{false};
    std::string mStatus{"not initialized"};
    LogFn mLog;
};

} // namespace immersive_portals
