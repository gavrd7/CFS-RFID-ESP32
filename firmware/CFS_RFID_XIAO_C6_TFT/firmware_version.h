#pragma once

// Single source of truth for the firmware release version.
// Update FW_VERSION when preparing the next tagged release.
// An ordinary local Arduino build is marked as development firmware.
#define FW_VERSION "1.3.2"
#define FW_BUILD_SUFFIX "-dev"

// GitHub Actions replaces FW_BUILD_SUFFIX in its build workspace:
//   pull requests -> -dev+<short commit SHA>
//   tags (vX.Y.Z) -> empty string, after verifying FW_VERSION.
// Adjacent string literals are concatenated by the C++ compiler.
#define FW_DISPLAY_VERSION FW_VERSION FW_BUILD_SUFFIX
