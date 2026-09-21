# ImmersivePortals for LeviLaunchroid

A native C++/Android port scaffold for LeviLaunchroid, inspired by the feature model of Immersive Portals: configurable portal pairs, portal-frame math, seamless position/velocity transforms, cross-dimension destinations, and a recursion-ready render model.

## Important compatibility note

The supplied `libminecraftpe.so` is `arm64-v8a`, identifies as Minecraft `1.26.51.01_RC0`, and has SHA-256 `b8a6351503d330628335a80e8131acd45291fa9a747465f0f34a31b2346847b4`. The uploaded legacy source's 124 function signatures were scanned against that binary's `.text` section and produced **0/124 matches**. The source also depends on legacy object offsets that are not verified for this target.

For that reason this project intentionally ships **no guessed game offsets and no copied legacy runtime**. The target adapter only verifies the binary's version marker and then stays dormant until a verified function-signature profile is supplied. This keeps the native mod load path safe instead of making an unverified hook that can crash Minecraft.

The pure portal engine and math are independent of those game hooks and are unit-tested on the host.

## Build

LeviLaunchroid's current native-mod guide uses the public `preloader-android` SDK, `PL_REGISTER_MOD`, `NativeMod::current()`, C++20, ARM64, and a pinned SDK release. This project follows that layout and pins preloader-android `0.2.2`.

Windows PowerShell:

```powershell
./scripts/package.ps1 -Clean
```

Linux/macOS:

```bash
./scripts/package.sh
```

The Android package is emitted as `build-arm64-v8a/dist/arm64-v8a/immersive_portals.levipack`.

## What is implemented now

- LeviLaunchroid native lifecycle using the public SDK.
- Typed JSON configuration using nlohmann/json.
- Portal geometry representation (center, normal, right, up, width, height).
- Crossing detection against the portal plane.
- Seamless coordinate and momentum transforms.
- Recursion depth and gravity-scale configuration reserved for the render/physics bridge.
- A target fingerprint record for the supplied Minecraft binary.
- Fail-closed hook installation when function signatures are not verified.
- Host unit tests for the portal mathematics.

## What requires a verified 1.26.51 engine profile

The supplied target binary uses newer ECS-oriented names/components and its legacy signatures do not match, so the following cannot be safely activated from the supplied inputs alone:

- locating the local-player update callback;
- obtaining the live local-player transform at runtime;
- calling or intercepting the engine's dimension transition pipeline;
- recursive secondary-world rendering and stencil/state setup;
- entity/block passthrough across the portal surface.

Those pieces belong in `src/game/GameAdapter.cpp` after a real 1.26.51 signature/offset audit. The adapter is structured so adding that profile does not require bringing any legacy source tree into the mod.

## Package contents

```text
immersive_portals/
├── manifest.json
├── libimmersive_portals.so
└── config/
    ├── config.json
    └── config.schema.json
```

The source repository contains no legacy dependency tree and the package contains no external game library.
