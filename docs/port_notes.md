# Port notes

The requested Java-edition reference is an architectural inspiration rather than source code to copy. The original concept centers on seeing through portals, seamless travel, mirrors, custom portal placement, recursive rendering, and dimension/world integration.

This port separates the work into three layers:

1. `portal/PortalMath.*` is engine-independent geometry and transform logic.
2. `portal/PortalEngine.*` owns configuration/state and the game bridge lifecycle.
3. `game/GameAdapter.*` is the only layer allowed to bind to private Bedrock engine functions.

The game bridge is deliberately fail-closed. The provided binary is a 1.26.51.01 RC0 Android ARM64 library, while every legacy function pattern supplied in the source audit missed it. No hardcoded neighboring-version address or guessed offset is installed.
