# Copilot / AI Agent Instructions for FireBird

Purpose: give an AI agent the minimal, concrete knowledge needed to be productive in this repo.

Architecture (big picture)
- Core GUI app: FireBird (MFC) — solution project: FireBird.sln -> project: FireBird/FireBird.vcxproj
- Shared library: FireBirdLib (native C++ library) used by the GUI and services
- Alternative GUI: FireBirdQT (Qt based) — see FireBirdQT/main.cpp and its project
- Services/tools: Watchdog (monitor/restart), Benchmark (benchmarks), GoogleUnitTest (tests)
- Data: SQL and CSV fixtures in `DataBase/` (many .sql and supported_tickers.csv)

Build & toolchain (concrete steps)
- This is a Windows MSVC solution targeting x64. Projects use PlatformToolset `v145` and expect x64 builds.
- Open `FireBird.sln` in Visual Studio (matching toolset) or use MSBuild:

  msbuild FireBird.sln /p:Configuration=Release /p:Platform=x64

- Shared include/library macros are in `propertysheet.props`. Check the `AnalysisInclude`, `LibInclude` and `IXWebSocket` macros there if you hit missing header/library errors.
- The repo contains a local `Lib/` subtree with third-party sources (simdjson, ixwebsocket, concurrentqueue, etc.). vcpkg manifests are not enabled by default (see project property `VcpkgEnableManifest`), so builds usually rely on these local folders or your own vcpkg setup.
- Charset and source encoding: project files set `CharacterSet=Unicode` and compile with `/utf-8`. The codebase expects UTF-8 source; DB exports historically use UTF-16 (see README).
- Precompiled header: projects commonly use `pch.h` — make sure it's present when compiling single files.

Project-specific conventions & patterns
- Code organization: most C++ sources are under `src/` with subfolders like `Windows interface`, `Working thread`, `DataBase`, `Type/China Market` etc. Paths include spaces — be careful with scripts.
- Macros & flags: projects define feature flags in the project preprocessor list (examples: `AUTO_DOWNLOAD`, `HAS_REMOTE_API`, `_OPENSSL_USE_DLL`). Search `.vcxproj` for these to understand feature toggles.
- Logging and libraries: uses `spdlog`, `nlohmann-json`, `simdjson`, `concurrentqueue`, `concurrencpp`, and `IXWebSocket` — find local copies in `Lib/` if not available via your package manager.
- Database: SQL and schema migrations live in `DataBase/` (multiple Version*.sql). Tests and tools may assume these files for initial data.
- Tests: unit tests live in `GoogleUnitTest/` and use GoogleTest. Run them from Visual Studio Test Explorer or build the test project and run the produced exe.

Integration and runtime notes
- FireBird (MFC) and FireBirdQT are distinct frontends that both link to `FireBirdLib` — changes to the library affect both GUIs.
- `Watchdog` is intended to restart the main app if it exits unexpectedly. Use it to reproduce crash/restart flows.
- Network/data sources: code integrates with remote providers (Sina, Tencent, Netease, Finnhub, Tiingo). Credentials or API limits may affect behavior — look for online API keys and rate-limit handling in `src/`.

Where to look first (quick map)
- Solution and shared props: `FireBird.sln`, `propertysheet.props`
- Main GUI project: `FireBird/FireBird.vcxproj` and sources under `src/Windows interface/` (e.g. `FireBird.cpp`)
- Native library: `FireBirdLib/FireBirdLib.vcxproj`
- Qt GUI: `FireBirdQT/main.cpp` and `FireBirdQT/FireBirdQT.ui`
- Tests: `GoogleUnitTest/` and `Lib/googletest/`
- Data and DB scripts: `DataBase/`
- Docs/install: `docs/安装手册(中文版).md`

Examples of helpful searches for an agent
- "Search for TODO or FIXME": scan `src/**` for developer hints.
- "Search for AUTO_DOWNLOAD": used to enable automatic data fetching logic in `.vcxproj`.
- "Search for IXWebSocket": network stack entry points are often in `src/WebSocket` or `src/Data Source`.

What NOT to assume
- No guaranteed vcpkg manifest usage — projects often rely on `Lib/` and `propertysheet.props` — verify before adding vcpkg-only guidance.
- The main GUI is MFC (Windows-only). Cross-platform changes should consider that `FireBirdQT` is the Qt alternative.

If you change build settings
- Update `propertysheet.props` if you add global include/lib paths.
- Keep `PlatformToolset` and `CharacterSet` consistent across projects to avoid ABI/encoding mismatches.

Next steps for the agent
- When asked to modify code, include the specific project path (for example: modify `src/Windows interface/FireBird.cpp` in `FireBird/FireBird.vcxproj`).
- If adding dependencies, update `Lib/` and `propertysheet.props` and document the change in `readme.md`.

---
Please review and tell me any unclear areas (build system, a specific component, or a workflow) to expand. 
