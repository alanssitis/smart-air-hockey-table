---
layout: ../../layouts/MDLayout.astro
title: Will Dobert Progress Report
---

# Progress Report for Will

## Week 4

**Date:** 9/15/2023 \
**Total Hours:** 10

### Description of Project Design Efforts

#### LED Driver Implementation

The visual aspect of our project will heavily rely on a matrix of individually-controlled RGB LEDs placed underneath a translucent play surface. On a technical level, we chose to achieve this by making use of WS2812-compatible LED hardware. It requires an 800 kbit/s data transmission rate from the hardware that drives it, so simple bit-banging will not suffice in our case. As I covered in last week's report, I chose to drive the LEDs with our microcontroller's timer and PWM generation peripherals.

My initial approach to this problem was to store a large array containing every PWM timing configuration required to transmit the current state of the LED matrix. From there, I used DMA to continuously stream the PWM timing configuration into the control register. Although this approach functioned correctly and was able to drive the LEDs as desired, the RAM usage was very inefficent. The design for our LED matrix contains **512 LEDs** in series. For each LED, **24 bits** are required to describe its set color. For each transmitted bit, a **16 bit** control register must be populated via DMA. The combination of these factors means that the RAM usage requirement would rise to **24 kilobytes** in a full implementation. In cases where we would like to store multiple "animation frames" of LED matrix data, the memory usage is prohibitively great.

Upon further [research](https://ralimtek.com/posts/2021/ws2812/), I decided that storing a significantly smaller buffer would suffice, when given the proper design consideration. In its current iteration, the LED driver transfers PWM timing configuration from a buffer containing just two 16 bit values. In order to correctly set the values in this buffer, I make use of the *DMA half transfer* and *DMA transfer complete* interrupts to update the half of the buffer that is not actively being copied to the configuration register. Although this approach introduced new CPU overhead (in the form of the DMA interrupts), it reduced CPU overhead in a different area. In the prior implementation, calling the LED driver to update a color value required immediately translating it into the proper sequence of PWM configuration values. Now this process is taken care of by the introduction of the aforementioned DMA interrupts. An additional benefit to the new driver implementation is that the LED matrix state can easily be queried by the application, as it is now stored in raw RGB values instead of PWM timing data. The RAM savings for this approach are significant, now taking only **1.5 kilobytes (a 93.75% reduction)**. If a further reduction is needed in the future, this approach allows us to easily reduce the bit depth of the RGB color data without significant code modifications. The full driver code at the time of writing can be viewed [here](https://github.com/alanssitis/smart-air-hockey-table/blob/27d28e5e9955093ac594590cc9b1ec9c55223aa8/firmware/smart-air-hockey-table/app/driver_led.c).

<video controls muted>
  <source src="/477grp5/team/will/20230915_204133.mp4" type="video/mp4">
</video>

_Figure 1: Test pattern applied to our LED prototyping hardware by the latest driver implementation_

#### Formalizing Main Application Architecture

While developing the LED driver, I was simultaneously organzing the structure of our firmware application. Although I made important steps toward this goal last week, there was still some smoothing of rough edges to do. The entrypoint for our main application is now in *app_core.c*, and my intention is that any additional files that support our application in a central manner be named with *app_* prefix. A primary example of this is the debugging module I created (*app_debug.c*). At the moment, this module contains a function to print timestamped log messages to the integrated serial interface, but it will be expanded in the future to contain firmware debugging functions as needed. Obtaining an accurate timestamp for log messages necessitated enabling a hardware timer and writing a basic interrupt handler for it. As with *driver_led.c*, my goal is for all driver source files to be prefixed with *driver_* as they are created for new hardware (such as small panel displays).

![Debug Logging](/477grp5/team/will/Screenshot-2023-09-15-210306.png)
_Figure 2: Demonstration of debug logging output from the microcontroller_

In this week's Component Analysis assignment, I was primarily responsible for analyzing our component choice for the LED matrix and the score tracking display.

#### Next Steps

Within the next week, I plan to fully design a CAD model of our table. This will be important as we prepare to construct the table, so that no guesses or assumptions are made throughout the process. Designing the table will also require me to research common dimensions for parts that it will be comprised of, such as plywood sheets, acrylic panels, and common sizes of wooden plank. Knowing these allows me to make the best decisions for designing the table right now, even if they may be adjusted in the future as we move into construction. I have already installed *Autodesk Fusion 360* (on Ben's recommendation) for this purpose.

## Week 3

**Date:** 9/8/2023 \
**Total Hours:** 12

### Description of Project Design Efforts

#### 3.3V to 5V Level Shifter Prototyping

As the week began, I was eager to start working with our LED hardware. The most immediate obstacle to opening an IDE and writing a driver was that our microcontroller operates on 3.3V logic, whereas the LED strip we have available runs on 5V. The "fastest-to-market" approach to solving this problem was to create a level shifter circuit on a breadboard. During a visit to the ECE shop, I was recommended to utilize a 2N7000 MOSFET to accomplish the voltage conversion. After researching some possible designs, I chose two to build and test.

The first of these designs is the simplest, as it only requires a 5V supply and features very straightforward connections. [Source](https://www.penguintutor.com/electronics/mosfet-levelshift)

![Schematic 1](/477grp5/team/will/mosfet-voltageshift.png)
_Figure 1: Circuit schematic of 1st MOSFET-based level shifter_
![Scope Output](/477grp5/team/will/pwm_800mhz_pen.png)
_Figure 2: Oscilloscope output of the above schematic_

From Figure 2 above, it can be observed that the rise time of the square wave is fairly significant compared to the wave's period. This is undesirable as the timing requirements for the LED specification are fairly strict (less than a microsecond). The 800 kHz frequency of the depicted wave is important, as this is the data rate the LED specification requires, so testing at this frequency closely simulates the conditions the level shifter will be subject to.

The second design I constructed is also quite simple, but it requires a 3.3V supply as well as 5V. The design shown below was constructed once again with a 2N7000 MOSFET, as well as two 1 kΩ resistors.

![Schematic 2](/477grp5/team/will/logic-level-shifting-basics-img2.jpg)
_Figure 3: Circuit schematic of 2nd MOSFET-based level shifter_
![Scope Output](/477grp5/team/will/pwm_800mhz_1000ohm_digi.png)
_Figure 4: Oscilloscope output of the above schematic_

The level-shifter design demonstrated in the figures above features a much improved rise time as compared to the prior design. I had initially constructed this design with 10 kΩ resistors, but the response characteristic had not improved significantly. Transitioning to a lower resistance proved to provide the performance I was hoping to see in the first place. I continued to utilize this design throughout the rest of the week as I moved forward to controlling the LED strip.

#### Software Reorganization and New Implementations

Before I dove fully into writing firmware to drive the LEDs, I decided it would be in the best interest of organization to decouple our code from that generated by the STM32 IDE. Not long after beginning this process, I had already run into a couple snags. The code generated to interface with the HAL (hardware abstraction layer) did not lend itself well to being used in places besides the main.c file. Additionally, I discovered that the HAL keeps track of its own state instead of relying on the built-in hardware registers, which introduces extra overhead that we'd like to avoid in the long-term.

I switched over our project, so now the IDE was generating "low layer"-based code, as opposed to HAL-based. This route felt similar to what I was accustomed to from ECE 362, and it allowed me to refer to the microcontroller reference manual more often. In the future, we may transition out of using any code generation, and rely fully on CMSIS headers and directly accessing device registers.

From this point, I researched how to set up a basic hardware timer and programmed it to output PWM on a GPIO pin. This was the first real step towards the eventual goal of implementing a LED driver in our firmware. The timing specification for our LED lighting hardware is functionally a PWM wave with a carefully controlled pulse width.

![WS2812 Timing](/477grp5/team/will/ws2812_timing.png)
_Figure 5: WS2812 specification timing diagram for sending 0 bit, 1 bit, and reset codes_

In the following handheld video recording, I showcase my current progress on interfacing our microcontroller to the LED hardware we are prototyping with. The LEDs are toggled on and off at 2 Hz, and the associated PWM control signal can be observed on the oscilloscope display.

<video controls muted>
  <source src="/477grp5/team/will/20230907_230330_1.mp4" type="video/mp4">
</video>

#### Next Steps

In the near future, I plan to implement and seek feedback from my teammates on a full-fledged LED driver. The driver will consist of a single source and header file, and I plan to make use of DMA (direct memory access) to feed the precise timing data to the PWM timer with minimal processing overhead. This will allow us to focus on more computationally expensive tasks in the future, such as ingesting and processing sensor data.

## Week 1 & 2

**Date:** 9/1/2023 \
**Total Hours:** 8

### Description of Project Design Efforts

#### Microcontroller Software Setup

This week, I was primarily focused on setting up a software build environment for our main microcontroller, the STM32F466. I did this by installing the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) and generating a straightforward test program. As Trevor has some prior experience with this software package, I worked alongside him to learn the various features and tools it has to offer. In tandem with setting up the build environment for software, I also began working with our microcontroller development board, and succeeded in flashing a functional test program to it. I took extra steps to ensure that the standard printf function works as expected, so now device logs can be viewed in a connected [PuTTY](https://www.putty.org/) terminal.

I felt these steps were important to complete as soon as possible, so we can focus on designing our software in the coming weeks. Throughout this process, I took care to collect any relevant datasheets or reference manuals I encountered. I uploaded them to our team's shared file storage, so we can easily refer to them in the future. It is encouraging to see the hardware and software working together already at this early stage.

#### Team Assignments / Documentation

Within the first week, I collaborated with my team on our Final Project Proposal. Beyond filling in my info and delegated roles, I contributed by revising the drafted PSDRs to fit the proper phrasing ("An ability to..."), as well as reorganizing them for clarity and conciseness. During our meeting to collaborate on the Functional Specification document, I authored a Peripheral Hierarchy flowchart to visualize and clarify our intended design approach. It is my intention that this diagram be revised and expanded over time as our project has more details finalized.

![Peripheral Hierarchy](/477grp5/team/will/PeripheralHierarchy.png)
_Figure 1: Provisional draft of project's Peripheral Hierarchy_

In regards to the aforementioned Functional Specification, I am currently documenting our project's known computational and thermal/power constraints.

In the next week, I look forward to prototyping with hardware and determining the feasibility of our current design plans. I also plan to begin laying out the high-level design for our software architecture.
