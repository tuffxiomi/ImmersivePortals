#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LEGACY_PATTERN="$(printf 'bedrock%s' 'tools')|$(printf 'bedrock_%s' 'tools')"
if find "$ROOT" -type f | grep -Eiq "${LEGACY_PATTERN}"; then
  echo "legacy runtime files are not permitted in this mod project" >&2
  exit 1
fi
if grep -RniE "${LEGACY_PATTERN}" "$ROOT" --exclude-dir=.git --exclude-dir=build-arm64-v8a >/dev/null; then
  echo "legacy runtime references are not permitted in this mod project" >&2
  exit 1
fi
NDK="${ANDROID_NDK_HOME:-${ANDROID_NDK_ROOT:-}}"
if [[ -z "$NDK" && -n "${ANDROID_HOME:-}" ]]; then NDK="$ANDROID_HOME/ndk/28.2.13676358"; fi
if [[ -z "$NDK" && -n "${ANDROID_SDK_ROOT:-}" ]]; then NDK="$ANDROID_SDK_ROOT/ndk/28.2.13676358"; fi
[[ -d "$NDK" ]] || { echo "NDK 28.2.13676358 not found" >&2; exit 1; }
cmake -S "$ROOT" -B "$ROOT/build-arm64-v8a" -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$NDK/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-28 -DANDROID_STL=c++_shared -DCMAKE_BUILD_TYPE=Release
cmake --build "$ROOT/build-arm64-v8a" --target levi_package
