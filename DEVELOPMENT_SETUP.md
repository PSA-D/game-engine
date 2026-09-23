# Development Setup

## 1. Doel

Dit document beschrijft hoe je het project lokaal aan de praat krijgt en hoe je met de gebruikte tools werkt. Het bevat:

- installatie-instructies voor Windows, macOS en Linux;
- de commands om het project te configureren, te bouwen en te testen;
- een uitleg van vcpkg, clang-format en clang-tidy en hoe je ze gebruikt.

Voor de afspraken over hoe het team samenwerkt, zie **CODING_GUIDELINES.md**.

## 2. Benodigde tools

| Tool | Minimale versie | Doel |
|---|---|---|
| Git | — | Versiebeheer |
| C++20-compiler | C++20-ondersteuning | Compileren |
| CMake | 3.25 | Buildsysteem |
| Ninja | recent | Buildgenerator |
| vcpkg | recent | Third-party dependencies (manifest mode) |
| clang-format | recent | Codeformattering |
| clang-tidy | recent | Statische analyse |

clang-format en clang-tidy worden geleverd door LLVM. De installatie verschilt per platform; zie paragraaf 3.

## 3. Installatie per platform

## 3.1 Windows

Ondersteunde compilers:

| Compiler | Installatie |
|---|---|
| MSVC | Visual Studio 2022 met de workload "Desktop development with C++" |
| Clang | LLVM-installer of de optionele component "C++ Clang tools for Windows" in Visual Studio |

Installeer de overige tools met winget:

```powershell
winget install Git.Git
winget install Kitware.CMake
winget install Ninja-build.Ninja
winget install LLVM.LLVM
```

De LLVM-installer zet de `bin`-map doorgaans zelf op het PATH. Controleer dat `clang-format` en `clang-tidy` bereikbaar zijn.

Installeer vcpkg:

```powershell
git clone https://github.com/microsoft/vcpkg C:\dev\vcpkg
C:\dev\vcpkg\bootstrap-vcpkg.bat
setx VCPKG_ROOT "C:\dev\vcpkg"
```

Sluit en heropen de terminal en/of IDE na `setx`.

## 3.2 macOS

Installeer de Xcode Command Line Tools (bevat Apple Clang):

```bash
xcode-select --install
```

Installeer de overige tools met Homebrew:

```bash
brew install cmake ninja git vcpkg
brew install llvm
```

De `llvm`-formula is keg-only; voeg het pad handmatig toe:

```bash
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
echo 'export VCPKG_ROOT="/opt/homebrew/opt/vcpkg"' >> ~/.zshrc
```

## 3.3 Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build git clang clang-tools
```

- `build-essential` bevat GCC/g++;
- `clang` en `clang-tools` bevatten clang, clang-format en clang-tidy.

Installeer vcpkg:

```bash
git clone https://github.com/microsoft/vcpkg ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.bashrc
```

Afhankelijk van je IDE zou dit command ook nodig kunnen zijn. Bij VS Code is dit namelijk handig:
```bash
echo 'VCPKG_ROOT=$HOME/vcpkg' >> ~/.config/environment.d/10-vcpkg.conf
```

Op andere Linux-distributies gebruik je het eigen pakketbeheer; de pakketnamen kunnen afwijken.

## 4. Project binnenhalen

```bash
git clone https://github.com/PSA-D/game-engine
cd game-engine
```

## 5. vcpkg (manifest mode)

De directe dependencies staan in `vcpkg.json` in de projectroot. Bij elke CMake-build leest vcpkg dit bestand en installeert wijzigingen automatisch. Een handmatige `vcpkg install` is in manifest mode niet nodig.

Configureer je het project met `cmake --preset debug` terwijl `VCPKG_ROOT` niet is ingesteld (zie paragraaf 3), dan faalt de configuratie direct met een duidelijke foutmelding.

## 6. Configureren en bouwen

**Waarschuwing voor CLion**: installeer zelf een compiler, zoals MSVC of MinGW, of gebruik geen Terminal, maar de build- en load-knoppen in CLion zelf.

Debug-build:

Eenmalig:
```bash
cmake --preset debug
```

Daarna om te bouwen:
```bash
cmake --build --preset debug
```

Release-build:

Eenmalig:
```bash
cmake --preset release
```

Daarna om te bouwen:
```bash
cmake --build --preset release
```

De gegenereerde bestanden komen in `build/debug` en `build/release` te staan, inclusief `compile_commands.json` (basis voor clang-tidy en editors die IntelliSense gebruiken).

### Compiler kiezen

Standaard kiest CMake de compiler van het platform: GCC op Linux, MSVC of Clang op Windows, Apple Clang op macOS. Wil je lokaal een andere ondersteunde compiler gebruiken, stel die dan in `CMakeUserPresets.json` in, bijvoorbeeld:

```json
{
    "version": 6,
    "configurePresets": [
        {
            "name": "debug-clang",
            "inherits": "debug",
            "cacheVariables": {
                "CMAKE_CXX_COMPILER": "/usr/bin/clang++"
            }
        }
    ]
}
```

`CMakePresets.json` zelf blijft platformonafhankelijk. Lokale aanpassingen aan generator of compiler horen altijd in `CMakeUserPresets.json`.

## 7. Testen

Testen worden meegebouwd zolang de optie `ENGINE_BUILD_TESTS` aan staat (standaard aan). Voer alle tests uit met:

```bash
ctest --test-dir build/debug --output-on-failure
```

De afspraken over wat er getest moet worden staan in CODING_GUIDELINES.md.

## 8. Werken met de tools

### 8.1 Formatteren met clang-format

Formatter één bestand:

```bash
clang-format -i engine/src/Engine.cpp
```

Formatter alle C++-bestanden (alleen vanuit de projectroot):

- PowerShell (Windows):

```powershell
Get-ChildItem -Recurse -File -Include *.cpp,*.h | ForEach-Object {
    clang-format -i $_.FullName
}
```

- Bash (macOS, Linux, Git Bash op Windows):

```bash
clang-format -i **/*.cpp **/*.h
```

### 8.2 Statische analyse met clang-tidy

clang-tidy gebruikt het door CMake gegenereerde `build/debug/compile_commands.json`:

```bash
clang-tidy -p build/debug engine/src/Engine.cpp
```

## 9 Versies controleren

Na installatie moet je minimaal deze commands kunnen uitvoeren:

```bash
cmake --version
ninja --version
git --version
clang-format --version
clang-tidy --version
```

## 10. Projectstructuur

```text
CMakeLists.txt
CMakePresets.json
vcpkg.json

engine/
    include/
    src/
game/
    src/
tests/
third_party/
```

Platform-specifieke enginecode wordt geisoleerd per platform:

```text
engine/platform/
    Windows/
    Linux/
    MacOS/
```
