#include "game/GameAdapter.hpp"

namespace immersive_portals {
namespace {
// This is deliberately only a build/version fingerprint. It does not provide a
// game-function address. The legacy source signatures did not match the supplied
// 1.26.51.01_RC0 library, so no guessed hook target is installed.
constexpr std::string_view kModule = "libminecraftpe.so";
constexpr std::string_view kVersionPattern =
    "31 2E 32 36 2E 35 31 2E 30 31 5F 52 43 30";
}

bool GameAdapter::verifyTarget() {
    const std::uintptr_t marker = pl::memory::resolveSignature(kVersionPattern, kModule);
    mTargetVerified = marker != 0;
    if (mTargetVerified) {
        mStatus = "Minecraft 1.26.51.01_RC0 marker found";
        log(mStatus);
    } else {
        mStatus = "target version marker not found; keeping game hooks disabled";
        log(mStatus);
    }
    return mTargetVerified;
}

bool GameAdapter::install() {
    if (!verifyTarget()) return true;
#if IP_ENABLE_EXPERIMENTAL_GAME_HOOKS
    // Intentionally fail closed until a signature profile containing verified
    // 1.26.51.01_RC0 function patterns is supplied. Never infer an address from
    // an adjacent game version.
    mHooksReady = false;
    mStatus = "verified target detected, but no function-hook profile is enabled";
    log(mStatus);
    return true;
#else
    mHooksReady = false;
    mStatus = "target detected; portal game bridge is dormant (no unsafe guessed offsets)";
    log(mStatus);
    return true;
#endif
}

void GameAdapter::uninstall() {
    mHooksReady = false;
}

} // namespace immersive_portals
