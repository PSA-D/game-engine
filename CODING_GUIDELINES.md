Coding Guidelines
=================

# 1. Doel

Dit document beschrijft de afspraken waarmee het ontwikkelteam van het C++ Game Engine-project samenwerkt:

- consistente en leesbare code;
- code die zo veel mogelijk onafhankelijk is van compiler en besturingssysteem;
- de [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) als basis voor onze C++-code;
- automatische controle via formatting, static analysis, tests en CI.
- hoe we de tools handig gebruiken.

Wat dit document **niet** bevat:
- de exacte gewenste formatting. Dat staat in `.clang-format`
- andere toolconfiguraties in `.clang-format`, `.clang-tidy` en de CMake-bestanden; die worden door de tools automatisch toegepast;
- installatie- en build-instructies; die staan in **DEVELOPMENT_SETUP.md**.

# 2. Technische standaard

## 2.1. C++-versie

Het project gebruikt **C++20**. We gebruiken het standaard C++-dialect en geen compiler-specifieke extensies zoals `gnu++20`. CMake dwingt dit expliciet af.

## 2.2. Compilers

Iedere developer mag de compiler gebruiken die passend is voor het eigen platform.

| Platform | Compiler      |
| -------- | ------------- |
| Linux    | GCC           |
| Windows  | MSVC of MinGW |
| macOS    | Apple Clang   |

We proberen **niet** iedereen dezelfde compiler te laten gebruiken. De code moet standaard-conform zijn en wordt in CI met meerdere compilers getest. Een developer mag lokaal een andere ondersteunde compiler gebruiken dan de andere teamleden.

## 2.3. Platformonafhankelijkheid

Code moet standaard C++ gebruiken wanneer daar een mogelijkheid voor bestaat. Platform-specifieke code is toegestaan wanneer het noodzakelijk is, maar moet worden geisoleerd:

```text
engine/
├── core/
├── graphics/
├── input/
├── audio/
└── platform/
    ├── Windows/
    ├── Linux/
    └── MacOS/
```

# 3. Bestandsstructuur

We organiseren files **per module**. Bijvoorbeeld:
```
/src
├── /core
│   ├── game_object.h
│   ├── game_object.cpp
│   ├── component.h
│   └── component.cpp
├── /components
│   ├── audio_source.h
│   ├── audio_source.cpp
│   ├── rigidbody.h
│   └── rigidbody.cpp
├── /systems
│   ├── renderer.h
│   └── renderer.cpp
└── /utils
    ├── math.h
    └── math.cpp
```

Vermijd platformchecks verspreid door de volledige codebase. Als platform-specifieke implementatie noodzakelijk is, hoort deze bij voorkeur achter een gemeenschappelijke interface te zitten.

Platform- en compiler-macro's mogen niet door elkaar worden gehaald: `#ifdef _WIN32` betekent Windows en niet MSVC.

# 4. C++ Core Guidelines

De C++ Core Guidelines van ISO C++ Foundation zijn de primaire richtlijn voor het schrijven van C++. We volgen de Guidelines waar ze van toepassing zijn en gebruiken moderne C++-constructies in plaats van legacy C++.

# 5. Ownership

Maak ownership expliciet.

- Deze code bezit de Renderer: `std::unique_ptr<Renderer> renderer;`
- Deze pointer bezit de Renderer **niet**: `Renderer* renderer;`
- Verwijzing naar een object dat elders beheerd wordt: `Renderer& renderer;`

# 6. Const

Gebruik const waar je const kunt gebruiken.

# 7. Headers

## 7.1. `#pragma once`

We gebruiken `#pragma once` als header guard. Hoewel dit formeel geen onderdeel van de ISO C++-standaard is, wordt het door alle ondersteunde compilers betrouwbaar ondersteund (GCC, Clang, Apple Clang en MSVC) en vermijdt het onnodige boilerplate.

## 7.2. Include wat je gebruikt

Een bestand mag niet afhankelijk zijn van transitive includes. Gebruikt een bestand `std::vector` rechtstreeks, dan include het bestand zelf de benodigde header; een header die toevallig iets meebrengt is daarvoor niet voldoende.

## 7.3. Include-volgorde

Includes worden gegroepeerd in deze volgorde:

1. corresponderende header;
2. projectheaders;
3. third-party headers;
4. standaardbibliotheek.

De formatter beheert de precieze ordering automatisch.

# 8. Bestandsextensies

Source-files eindigen op `.cpp`. Header-files eindigen op `.h` (en dus niet `.hpp` of `.hxx`).

# 9. Naamgevingsconventies

| Element    | Convention   | Voorbeeld          |
| ---------- | ------------ | ------------------ |
| Classes    | `PascalCase` | `RenderSystem`     |
| Structs    | `PascalCase` | `Vertex`           |
| Functions  | `camelCase`  | `createWindow()`   |
| Variables  | `camelCase`  | `windowSize`       |
| Members    | `camelCase`  | `windowHandle`     |
| Constants  | `ALL_CAPS`   | `MAX_VERTICES`     |
| Namespaces | `lowercase`  | `engine::graphics` |
| Files      | `PascalCase` | `RenderSystem.h`   |

Private members krijgen geen underscore-prefix, tenzij een specifiek onderdeel van de codebase daar een expliciete reden voor heeft.

# 10. Waarschuwingen en undefined behavior

We compileren met een strenge warning policy. Voor GCC en Clang gebruiken we minimaal `-Wall`, `-Wextra` en `-Wpedantic`; waar nuttig komen daar `-Wshadow`, `-Wconversion` en `-Wsign-conversion` bij. MSVC gebruikt de equivalente strenge instellingen. De exacte flags staan in de CMake-bestanden en worden automatisch toegepast.

We schrijven geen code die afhankelijk is van warnings die toevallig door slechts een compiler worden gegeven.

Er wordt geen code geschreven met undefined behavior. Nieuwe code wordt zonder waarschuwingen en zonder UB-instrumentatie afgeleverd.

# 11. Tools: hoe te gebruiken

## 11.1. clang-format

We gebruiken **clang-format** voor formatting; de configuratie staat in `.clang-format` en wordt toegepast op `*.cpp` en `*.h`.

Alle bestanden formatteren (alleen vanuit de projectroot):

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

## 11.2. clang-tidy

We gebruiken **clang-tidy** voor automatische controle; de configuratie staat in `.clang-tidy`.

Niet iedere check is altijd geschikt voor iedere situatie. Een check mag worden uitgeschakeld wanneer daar een concrete reden voor is. Zo'n uitzondering moet bij voorkeur worden beperkt tot de betreffende regel:

```cpp
// NOLINT(cppcoreguidelines-...)
```

en niet door de volledige check uit te schakelen.

In overleg zouden we ook hele checks uit kunnen schakelen.

clang-tidy lokaal uitvoeren (Gebruikt de door CMake gegenereerde `compile_commands.json` in de buildmap):

```bash
clang-tidy \
    -p build/debug \
    engine/src/SomeFile.cpp
```

Alle C++-bestanden analyseren (alleen vanuit de projectroot):

- PowerShell (Windows):

```powershell
Get-ChildItem -Recurse -File -Include *.cpp,*.h |
    Where-Object { $_.FullName -notlike "*\build\*" } |
    ForEach-Object {
        clang-tidy -p build/debug $_.FullName
    }
```

- Bash (macOS, Linux, Git Bash op Windows):

```bash
find . -path './build' -prune -o -type f \( -name '*.cpp' -o -name '*.h' \) -exec clang-tidy -p build/debug {} +
```

# 12. Tools automatisch uitvoeren

## 12.1. clang-format (formatteren)

**VS Code** (alle platforms):

1. Installeer de C/C++-extensie (`clangd`).
2. Voeg toe aan de workspace-setting `.vscode/settings.json`:

```json
{
    "editor.formatOnSave": true,
    "C_Cpp.clang_format_style": "file",
    "[cpp]": {
        "editor.defaultFormatter": "ms-vscode.cpptools"
    }
}
```

`C_Cpp.clang_format_style: "file"` zorgt ervoor dat de repo-eigen `.clang-format` wordt gebruikt. Bij het opslaan wordt clang-format automatisch uitgevoerd.

**CLion** (alle platforms):

Schakel in Settings → Editor → Code Style → C/C++ de ClangFormat-integratie in. CLion leest de `.clang-format` en `.clang-tidy` uit de projectroot.

**Visual Studio** (Windows):

Visual Studio 2022 heeft ingebouwde support voor het formatteren met `.clang-format` (Tools → Options → Text Editor → C/C++ → Formatting). Pas dit aan waar nodig.

## 12.2. Bij het committen (pre-commit hook)

Je kan er ook voor kiezen om clang-format en clang-tidy automatisch bij elke commit te runnen in plaats van bij het opslaan van een bestand, via een git hook. Maak het bestand `.githooks/pre-commit` aan met de volgende inhoud:

```bash
#!/usr/bin/env bash
# Format gewijzigde C++-bestanden en analyseer ze met clang-tidy voor het committen.
set -euo pipefail

files=$(git diff --cached --name-only --diff-filter=ACM -- '*.cpp' '*.h')

if [[ -z "$files" ]]; then
    exit 0
fi

clang-format -i $files

# clang-tidy heeft compile_commands.json nodig uit een eerdere configure.
if [[ -f build/debug/compile_commands.json ]]; then
    clang-tidy -p build/debug $files
else
    echo "WAARSCHUWING: compile_commands.json ontbreekt; clang-tidy overgeslagen."
fi

git add $files
echo "clang-format toegepast en clang-tidy uitgevoerd op: $files"
```

Activeer de hook-map (dezelfde opdracht op alle platforms):

```bash
git config core.hooksPath .githooks
```

Op macOS en Linux moet het script uitvoerbaar zijn:

```bash
chmod +x .githooks/pre-commit
```

Op Windows werkt dezelfde opdracht vanuit PowerShell of cmd; het script draait dankzij de Git Bash die met Git for Windows wordt meegeleverd. Maak op Windows alleen de `.githooks`-map en het scriptbestand aan via de teksteditor en stel daarna `core.hooksPath` in.

## 12.3. clang-tidy (statische analyse)

clang-tidy draait in de editor als live diagnostic: de tool laat bevindingen zien terwijl je typt. Daarvoor is een bestaande configuratie nodig (de `compile_commands.json` uit een eerdere configure, zie DEVELOPMENT_SETUP.md) en wordt de repo-eigen `.clang-tidy` gebruikt.

- **VS Code** (alle platforms): installeer de clangd-extensie (`llvm-vs-code-extensions.vscode-clangd`) en voeg aan de workspace-setting `.vscode/settings.json` toe:

```json
{
    "clangd.arguments": [
        "--clang-tidy",
        "--background-index",
        "--compile-commands-dir=${workspaceFolder}/build/debug"
    ]
}
```

Als er ook andere live diagnostic systemen zijn (zoals Microsoft IntelliSense) die draaien krijg je daar een notificatie van. Zet deze dan uit

clangd rapporteert clang-tidy-bevindingen live in de editor.

- **CLion** (alle platforms): schakel Settings → Editor → Inspections → C/C++ → Clang-Tidy in. CLion rapporteert clang-tidy-bevindingen tijdens het typen.

- **Visual Studio** (Windows): Code Analysis kan clang-tidy gebruiken wanneer de component "C++ Clang tools for Windows" is geinstalleerd. Dat is minder geautomatiseerd dan de opties hierboven.

# 13. Testing

- Nieuwe functionaliteit moet waar redelijkerwijs mogelijk door tests worden ondersteund.
- Een bugfix hoort waar mogelijk een regression test te krijgen.
- Tests worden uitgevoerd via CTest; de command daarvoor staat in DEVELOPMENT_SETUP.md.
- Tests mogen niet afhankelijk zijn van de lokale gebruikersomgeving.

# 14. Git

Commits moeten logisch en klein genoeg zijn om afzonderlijk te begrijpen. Vermijd commits zoals `fix stuff` of `changes`.

Gebruik beschrijvende commit messages, bijvoorbeeld:

```text
Add Vulkan instance initialization
Fix entity lifetime during scene removal
Add collision tests
```

Een commit moet bij voorkeur een logisch doel hebben.

# 15. Branches en Pull Requests

Werk niet rechtstreeks op de gedeelde hoofdbranch. Gebruik feature branches:

```text
main
│
├── feature/vulkan-renderer
├── feature/input-system
├── fix/entity-lifetime
└── refactor/resource-manager
```

Pull Requests worden gebruikt om wijzigingen naar `main` te brengen. Een PR moet:

- compileerbaar zijn;
- relevante tests laten slagen;
- formatting volgen;
- geen onnodige wijzigingen bevatten;
- door minimaal twee andere teamleden zijn bekeken wanneer de tijdsplanning dit toelaat.

# 16. Continuous Integration

CI draait via GitHub Actions. De CI controleert minimaal:

1. configuratie;
2. compilation;
3. tests;
4. formatting;
5. static analysis waar mogelijk.

# 17. Compiler-specifieke code

Compiler-specifieke code is alleen toegestaan wanneer dit noodzakelijk is. In zulke gevallen moet de code duidelijk worden geisoleerd en gedocumenteerd.
