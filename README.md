# ATtiny85 HAL Library
## Overview

This is a library for ATtiny85 microcontrollers, a cheap, popular,
and relatively simple microcontroller compatible with the Arduino ecosystem. The library is intended to be a general purpose replacement to the Arduino library.

Why not use the Arduino library if it already exists?

1. To unlock features that are simplified by the Arduino library (such as hardware timers, hardware interrupts, and PWM configuration) without having to configure hardware details manually. Keeping the intimate details abstracted is important for portable, maintainable code.

2. As a learning experience, to educate myself on how to manage levels of abstraction for an embedded systems project.

## Modules
The code is split into modules of different abstraction levels, each of which. From most concrete to most abstract:
- Hardware Abstraction Layer (HAL)
- Driver Layer
- Service Later
- App Layer

### Hardware Abstraction Layer (HAL)
The HAL allows the user to configure register settings *without* needing to know where those registers are. The HAL provides functions to write the bits of a certain setting (timer prescaler, timer wave generation mode, etc.) Sometimes the bits of a certain setting can be split among two different registers, but only the HAL has to worry about that.

The HAL does no policy enforcement or error checking. If you call a hal function directly, it will do whatever you tell it, even if it creates an invalid state. The HAL itself is also stateless, since it contains no global variables.

### Driver Layer
The driver layer enforces correct usage of the HAL. When you want to configure a certain setting, you pass an enum to the driver function, which then calls HAL functions that perform the desired action. An enum lets you use a descriptive name instead of having to remember a certain bit pattern for a given setting. This is important for portability, since different hardware may have similar capabilities but a different register layout.

For example, the driver is passed a timer mode enum, which the driver converts into wave generation mode bits WGM[2:0], which get passed to the timer0 HAL. A different timer or a different microcontroller may not configure timer modes the same way, but they generally have similar modes (Normal, CTC, PWM).

If the user attempts to configure an invalid state through the driver, the driver explicitly returns an error. If the error is passed through each layer up to main, an LED will flash a certain number of times to indicate which error has occured.

### Service Layer
The service layer combines multiple drivers and makes sure they work together smoothly. It provides more abstraction and even more error checking. A service is more for general use, while accessing drivers directly gives the user finer control when they need it.

### App Layer
The app is where you put project-specific code. The header app.h provides an interface for two functions, app_init and app_run. Like the Arduino library, the main file calls app_init once at the start and calls app_run indefinitely.

Initialize services and drivers in app_init, and add your "business logic" to app_run. Errors can be passed from services/ drivers to the main file, which blinks the error LED automatically.

The app you want to run is specified in the makefile. as APP_SRC. You can swap it out for any of the example codes provided.

## Features
I will go into details on the specific features which this library enables later.

## Instructions
Later I will also detail how to flash the firmware onto an ATtiny via in-circuit serial programming (ICSP).