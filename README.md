# ![Morpheus Icon](https://i.imgur.com/vujpFT6.png) Morpheus
## An experimental Game Boy Advance and Nintendo DS game engine written in modern C++

![GitLab pipeline status](https://gitlab.com/insightgit/Morpheus/badges/master/pipeline.svg)
 
## ✔️ Features
- Regular/Affine Sprite (or OBJ) and Regular (or Text) Background support
- Larger "streaming" background support up to 512 tiles by 512 tiles (or 4096 px by 4096 px)
- Cross-platform "Ani" sprite animation engine
- CMake integration
- Basic libtonc/libnds Text API implemented in addition to an experimental 
  cross-platform "Expression" text API supporting ASCII or UTF8 custom 
  fonts manually created or generated from the generate_font.py buildtools
- GBA multiplayer-mode serial communication 
- MaxMod sfx and module support
- 2D hardware blending, mosaic, and windowing support
- Save manager supporting DS DLDI, GBA SRAM, and GBA 64kb/128kb FLASH saves
- External Palette support on the DS for Sprites and Backgrounds
- Several Python 3 scripts to assist with asset management and the build process

## 🏗️ Work in Progress (WIP)
- Reworking puzzler test game to support newer API, and creating the Hayai test game (in progress)s
- Doxygen and Sphinx documentation (in progress)
- Affine Background and TextBase support (in progress)
- Background animation support (in progress)
- DS wifi support (not yet started)

## ⚠️ Broken and currently postponed
- GBA EEPROM save support

## Need Help? Want to show off what you're making? Join the Discord!
https://discord.gg/rK2f63yaMK
