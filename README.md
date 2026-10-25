# Pixel Quest — Nintendo DS homebrew

A tiny 2D pixel-art collect-the-gem game for original Nintendo DS hardware.
It uses the DS framebuffer and libnds, so it does not need external graphics assets.

## Controls

- D-pad Left/Right: move
- A: jump
- Collect the glowing gold gems to increase the score

## Easiest build route (GitHub Actions)

This project includes `.github/workflows/build.yml` so GitHub can build the ROM using the devkitPro devkitARM container.

1. Create a new GitHub repository and upload the contents of this folder (including the hidden `.github` folder).
2. Open the repository's **Actions** tab and select **Build Nintendo DS ROM**.
3. Choose **Run workflow**. Wait for the run to finish successfully.
4. In the completed run, download the `pixel-quest-nds` artifact. It contains `pixel_quest.nds`.
5. Extract the artifact ZIP and copy `pixel_quest.nds` to your DS homebrew storage.

GitHub may require Actions to be enabled for the repository. This route still needs you to start the build; this chat environment cannot produce the compiled binary itself.

## Build on Windows

1. Install devkitPro from its official website: https://devkitpro.org/wiki/Getting_Started
2. In the installer, select the Nintendo DS development tools (including devkitARM and libnds).
3. Open the **MSYS2 / devkitPro terminal** installed by devkitPro.
4. Change directory to this project's folder. For example:
   `cd /c/Users/YourName/Downloads/pixel_quest_nds`
5. Run:
   `make`
6. The Makefile supplied by libnds should produce `pixel_quest.nds` in the project directory.

If the output has a different name in your installed template, check the build output and Makefile configuration.

## Build on Linux/macOS

Install devkitPro's pacman and the Nintendo DS development packages using the official devkitPro guide:
https://devkitpro.org/wiki/Getting_Started

Then open a terminal with `DEVKITPRO` configured and run `make` in this directory.

## Run on a real Nintendo DS

You need a compatible homebrew method for your DS model, commonly a flashcart and microSD card. Follow the instructions for your own hardware and use only homebrew software you trust. Copy `pixel_quest.nds` to the card and launch it from its menu.

## Project files

- `source/main.c` — game loop, framebuffer graphics, controls, physics, and score
- `Makefile` — invokes the libnds build system

## Notes

This source project was created here, but a compiled `.nds` binary is not included because the devkitPro/devkitARM Nintendo DS compiler toolchain was not installed in the build environment used to generate this project. Build it using the official toolchain steps above.
