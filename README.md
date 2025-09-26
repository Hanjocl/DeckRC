# How to build on steamdeck:
### Step 0 - Get to development 
disable read-only mode on steamdeck with: `sudo steamos-readonly disable`

### Step 2 - Setup development environment 
- Install VS code
- Install CMake
- Install GCC
- Install Ninja

### Step 3 - Install all dependencies
Go through the submodule installations guides to install all dependecies (see the following READMEs).
- [ExpressUSB-Controller](https://github.com/Hanjocl/ExpressUSB-Controller) - Establishes communication with a ExpressLRS transmitter
- [OpenIPC-QML-Element](https://github.com/Hanjocl/OpenIPC-QML-Element) - Establishes a video stream with a RTL8812AU wifi Receiver
- [SDL-RC-Controller](https://github.com/Hanjocl/SDL-RC-Controller) - Maps keyboard & controller inputs to channels for use with ExpressUSB

**NOTES:**
- For windows I used VCPKG for most, expect for QML
- For Linux I just used the platfroms package manager, expect for QML
- QML was installed through their online installer

### step 4 - Build and pray for no error
- if you get any, I suggest asking GPT
- if that does not work out, Ask a friend or make a issue!
