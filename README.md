# BLUECLION2026 Pitch Shifter — starter source

Windows 10/11 standalone JUCE application scaffold.

## Current status

This is a **starter project**, not a finished ELASTIQUE PRO pitch shifter.
It currently provides:
- Native desktop window and dark interface
- Audio file import using JUCE-supported formats
- Audio preview playback
- Pitch control from -24 to +24 semitones
- Formant-preservation UI control
- A clearly marked integration boundary for the official zplane SDK

The current preview is unshifted. The `ElastiqueProEngine` class intentionally does not process audio until the licensed SDK is supplied and integrated. No zplane binaries, headers, or proprietary implementation are included.

## Requirements

- Windows 10/11, 64-bit
- Visual Studio 2022 with Desktop development with C++
- CMake 3.22+
- Git
- Internet access on first configure (CMake fetches JUCE 8.0.6)

## Build

Open a Developer PowerShell for Visual Studio, then run from this folder:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

The executable will be under a path similar to:

```text
build\BLUECLION2026PitchShifter_artefacts\Release\
```

Exact output paths can vary by CMake generator and JUCE version.

## Connect the official ELASTIQUE PRO SDK

1. Obtain the ELASTIQUE PRO SDK directly from zplane under a licence that permits your intended use and distribution.
2. Read the documentation shipped with your exact SDK version.
3. Implement the `ElastiqueProEngine` methods in `Source/ElastiqueProEngine.cpp` using the official headers/API. Do not guess API signatures from another SDK version.
4. Configure the include directories and library files in `CMakeLists.txt` according to zplane's delivery package.
5. Connect the engine to the real-time audio path. Follow the SDK's buffer sizing, latency, channel-layout, sample-rate, and lifecycle requirements.
6. Test with mono/stereo files and all supported sample rates before publishing.

Do not bundle SDK files with public downloads unless the zplane agreement explicitly permits it.

## Licensing notes

- zplane says its commercial licence permits including its technology in an application distributed to customers; licensing terms and fees depend on the product/platform/market. Contact `info@zplane.de`.
- JUCE is dual-licensed (AGPLv3 or commercial). Choose a licence compatible with how you plan to distribute this application and review JUCE's current licence before publishing.
- This project contains no zplane technology and must not be advertised as ELASTIQUE-powered until the official SDK is integrated and licensed.
