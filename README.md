# How to build on steamdeck:
### Step 0 - Get to development 
Disable read-only mode on steamdeck with: `sudo steamos-readonly disable`

### Step 2 - Setup development environment 
- Update System
```bash
sudo pacman -Syu
```
- Install VS code
```bash
pacman -S code
```
- Install base-development tools (gcc, g++, make, pkgconf, binutils, patch, fakeroot)
```bash
sudo pacman -S base-devel cmake git
```
- Install Ninja
```bash
pacman -S ninja
```
- Install QML through the [online installer](https://www.qt.io/download-qt-installer-oss)

### Step 3 - Install all dependencies (updated on: 27/09/2025)
Go through the submodule installations guides to install all dependecies (see the following READMEs). For Windows I used **VCPKG** for as package manager, for Linux I just the platforms packagemanager. QML was installed through the **online installer**. 
- [ExpressUSB-Controller](https://github.com/Hanjocl/ExpressUSB-Controller) - Establishes communication with a ExpressLRS transmitter
    - [BOOST ASIO](https://www.boost.org/)      `sudo pacman -S boost`

- [OpenIPC-QML-Element](https://github.com/Hanjocl/OpenIPC-QML-Element) - Establishes a video stream with a RTL8812AU wifi Receiver
    - [FFMPEG](https://ffmpeg.org/)             `sudo pacman -S ffmpeg ffmpeg-libs`
    - [LibSodium](https://doc.libsodium.org/)   `sudo pacman -S libsodium`
    - [LibUSB](https://libusb.info/)            `sudo pacman -S libusb`

- [SDL-RC-Controller](https://github.com/Hanjocl/SDL-RC-Controller) - Maps keyboard & controller inputs to channels for use with ExpressUSB
    - [SDL2](https://www.libsdl.org/)           `sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_net`


### step 4 - Build and pray for no error
- if you get any, I suggest asking GPT
- if that does not work out, Ask a friend or make a issue!

---
# THROUBLESHOOTING


1. Could not be found because dependency WrapOpenGL could not be found
```bash
sudo pacman -Syu mesa mesa-utils libglvnd glu
```

