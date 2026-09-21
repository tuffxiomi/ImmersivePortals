# Build and safety status

## Verified in this workspace

- CMake configure for the host test target succeeds.
- `portal_core_tests` builds and passes under CTest.
- The source tree contains no legacy tool runtime, headers, or source files.
- The Android build files use the public `preloader-android` SDK path and target `arm64-v8a`.

## Not claimed as locally verified

This container does not have an Android NDK installation and cannot fetch the remote Android SDK dependency, so a real `libimmersive_portals.so` link was not produced here. The repository includes a GitHub Actions workflow that installs NDK 28.2.13676358 and builds the `.levipack` in an Android-capable environment.

The target-specific game bridge is intentionally disabled because the supplied Minecraft 1.26.51.01_RC0 binary did not match any of the legacy function signatures from the audited source. No guessed function addresses or neighboring-version offsets are used.
