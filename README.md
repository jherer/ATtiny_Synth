# ATtiny85 HAL Library
## Overview

This is a library for ATtiny85 microcontrollers, a cheap, popular, and relatively simple microcontroller compatible with the Arduino ecosystem. The library is a general purpose replacement for the Arduino library that uses the principles of a [Hardware Abstraction Layer](https://en.wikipedia.org/wiki/Hardware_abstraction) (HAL).

Why not use the Arduino library if it already exists?

1. To unlock features that the Arduino library glosses over, without breaking abstraction.
2. For educational experience, to learn how to manage hardware abstractions and to learn about some ATtiny85/general microcontroller features.

## What is abstraction?
[Abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)) separates the code responsible for specific individual actions from the code responsible for general, purposeful combinations of those actions. Using abstraction makes code reusable for different hardware implementations. Abstract code is also easy to understand and reuse for different project requirements.

For example, to show an image on an LED screen, you should separate the code for *how* to display an image from the code for *what* image to display. *How* to change the pixel colors can depend on the hardware, and the user should not have to worry about that when they choose *what* image to display. If the hardware changes, then only the *how* part needs to be swapped for a different implementation. You can keep the names and parameters the same or similar in the new implementation, to the extent that the new hardware has the same features.

In the Arduino library, you often use abstract functions such as pinMode and digitalRead to control inputs and outputs, which are features any microcontroller generally has. However, immediately manipulating a register
in the next line is poor abstraction (and ugly). Literally, it is reaching inside the registers and forcing the whole module to be hardware dependent. Instead, the register manipulation needs to be in a separate module that can be swapped out when using a different microcontroller.

## Modules
The code is split into modules of different abstraction levels. From most concrete (specific) to most abstract (general):
- Hardware Abstraction Layer (HAL)
- Driver Layer
- Service Later
- App Layer

### Hardware Abstraction Layer (HAL)
The HAL allows the user to configure register settings *without* needing to know where those registers are. The HAL provides functions to write the bits of a certain setting (timer prescaler, timer wave generation mode, etc.) based on enums provided. Sometimes the bits of a certain setting can be split among two different registers, but only the HAL has to worry about that.

The HAL does no policy enforcement or error checking. If you call a HAL function directly, it will do whatever you tell it, even if it creates an invalid state. The HAL itself is also stateless, since it contains no global variables.

### Driver Layer
The driver layer enforces correct usage of the HAL. It does not worry about bits or registers, only semantic meaning provided by the HAL. When you want to configure a certain setting, you pass an enum to the driver function, which then calls HAL functions that perform the desired action. An enum lets you use a descriptive name instead of having to remember a certain bit pattern for a given setting. This is important for portability, since different hardware may have similar capabilities but a different register layout.

For example, the driver is passed a timer mode enum, which is passed the timer0 HAL. A different timer or a different microcontroller may not configure timer modes the same way, but they generally have similar features.

If the user attempts to configure an invalid state through the driver, the driver explicitly returns an error. If the error is passed through each layer up to main, an LED will flash a certain number of times to indicate which error has occured.

### Service Layer
The service layer uses drivers to perform a useful task. It provides more abstraction and even more error checking. A service is more for general use, while accessing drivers directly gives the user finer control when they need it. For example, a software timer service could take a timer interval in milliseconds and automatically configure the timer frequency and top value in through the timer driver. Additionally, it should be able to keep track of multiple events as long as the time intervals are a multiple of the clock interrupt period.

### App Layer
The app is where you put project-specific code. The header app.h provides an interface for two functions, app_init and app_run. Like the Arduino library, the main file calls app_init once at the start and calls app_run indefinitely.

Initialize services and drivers in app_init, and add your "business logic" to app_run. Variables are held in an instance of the app_state_t struct to minimize global variables. Errors can be passed from services/drivers to the main function, which will blink the error LED with the appropriate code automatically.

The app you want to run is specified in the makefile. as APP_SRC. You can swap it out for any of the example codes provided.

## Features
I will go into details on the specific features which this library enables later.

## Instructions
Later I will also detail how to flash the firmware onto an ATtiny via in-circuit serial programming (ICSP).
