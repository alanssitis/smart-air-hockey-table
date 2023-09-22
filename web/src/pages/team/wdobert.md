---
layout: ../../layouts/MDLayout.astro
title: Will Dobert Progress Report
---

# Progress Report for Will

## Week 5

**Date:** 9/22/2023 \
**Project Hours Since Last Report:** 10 \
**Cumulative Semester Project Hours:** 44

### Description of Project Design Efforts

#### Initial Efforts on Table CAD Model

Despite high hopes for this week, I have not yet completed a designed model of our table. As I began drawing out even the simplest parts of the design, I began to realize just how many unknowns I was encountering. How much clearance do the PCBs need, what kind of spacers will we use, how thick will the acrylic be, on and on. Even still, I managed to create a mockup of the very top portion of the table. This was possible since it required no knowledge of the wooden component dimensions we'll be using.

![CAD Model](/477grp5/team/will/Screenshot-2023-09-21-214008.png)
_Figure 1: Current state of CAD model in Fusion 360_

No matter how minimal or stripped down this design is from what it will eventually become, I still appreciated seeing it for the first time. I had the realization of just how many PCBs we'll be assembling and connecting together, and it has further fueled my excitement for this project's goals to come to fruition. Due to the number of unknowns that I was estimating rough values for, I elected to parameterize nearly every dimensioned value I possibly could, so they can be adjusted as we nail down real values. This had the added benefit of removing any *magic numbers* from the CAD model, which appeased my "programmer" tendencies.

![User Parameters](/477grp5/team/will/Screenshot-2023-09-21-214440.png)
_Figure 2: Configurable parameters that describe various dimensions of the design_

#### Initial Efforts on OLED Display Driver

As the week moved on and I concluded that the time had not yet arrived to put my entire focus on designing the CAD model, I transitioned to a different task. Writing the LED matrix driver had been a fairly straightforward and rewarding endeavor, so surely writing a driver for the OLED display would be just as fun. The first major snag I stumbled upon was reacquiring the datasheet for the display we had previously used in ECE 362. Pasting the product code into Google yieled some results, but nothing that described the SPI control protocol in much detail. I eventually decided to search for the ECE 362 lab website and located their hosted copy of the datasheet. Once I had begun poring over the document and configuring our microcontroller's peripherals accordingly, I arrived at another snag. Our prototyping device, the *STM32F446* has a less sophisticated implementation of STM's SPI peripherals, and can only be configured for transmitting 8- or 16-bit words. However, the SPI protocol for the OLED display requires us to send 9-bit words, which our microcontroller in ECE 362 was capable of. Although it meant that I was unable to test the driver at the time, the microcontroller we'll use in our final design (*STM32U585*) does posess the needed capability. All of this became a moot point when we recently switched our choice of hardware for the score tracking display. We made the decision to utilize a full-pixel OLED instead of character-based, which had been under heavy consideration beforehand. With our choice of display hardware now finalized, I began distilling some vital details from the datasheet into a formatted document on our [GitHub issues page](https://github.com/alanssitis/smart-air-hockey-table/issues/46). This was done with the intent to jumpstart driver development once the new hardware arrives.

#### Parts Procurement and Bill of Materials

Now that we've arrived at the end of week 5, the push to start purchasing hardware is stronger than ever. With Ben and Trevor making incredible progress on designing our two PCBs, we're nearly at the point where ordering components is the next logical step. Over the course of the past few days, I've collaborated with the team on putting together our official procurement documents for submission as soon as possible. At this stage, we will be making orders from three separate vendors: Digikey, Amazon, and JLCPCB. Our process for appending components to the procurement form has largely consisted of sharing URLs back and forth and giving our thoughts on each option. I also held primary responsibity for transforming each decision into a properly formatted entry in the procurement spreadsheet. An exciting result of this process was the inclusion of an *STM32U575* Nucleo evaluation board on our order form, which will allow us to prototype features for our microcontroller simultaneously while assembling our master PCB. Late in the week, I made an attempt submit our documentation for purchasing PCBs, but the recommended office (BHEE 140) is unreachable or no longer exists due to ongoing renovations. Instead, I will deliver this documentation to an alternate location in the very near future. Fortunately for us, we decided to work on the procurement document during the same week that the Bill of Materials assignment is due for submission. This overlap should make it trivial to fill in the required information for each of our components.

![Procurement Spreadsheet](/477grp5/team/will/Screenshot-2023-09-21-225322.png)
_Figure 3: Current list of components to be ordered from Digikey, with part number and pricing info_

#### Next Steps

Although this week consisted of a couple false starts on my behalf, I am hopeful that having nailed down several component choices at the end of this week will make future attempts at project design more fruitful. Early next week, I'd like to place orders for all the hardware we've decided upon, and possibly start assembling the parts that arrive before the week is out. I am especially excited to see our first batch of PCBs arrive and begin testing with them. In the near future, I will be participating in training at the [Bechtel Innovation Design Center](https://www.purdue.edu/bidc/) to obtain authorization for constructing our table at their facilities.

## Week 4

**Date:** 9/15/2023 \
**Project Hours Since Last Report:** 13 \
**Cumulative Semester Project Hours:** 34

### Description of Project Design Efforts

#### LED Driver Implementation

The visual aspect of our project will heavily rely on a matrix of individually-controlled RGB LEDs placed underneath a translucent play surface. On a technical level, we chose to achieve this by making use of WS2812-compatible LED hardware. It requires an 800 kbit/s data transmission rate from the hardware that drives it, so simple bit-banging will not suffice in our case. As I covered in last week's report, I chose to drive the LEDs with our microcontroller's timer and PWM generation peripherals.

My initial approach to this problem was to store a large array containing every PWM timing configuration required to transmit the current state of the LED matrix. From there, I used DMA to continuously stream the PWM timing configuration into the control register. Although this approach functioned correctly and was able to drive the LEDs as desired, the RAM usage was very inefficent. The design for our LED matrix contains **512 LEDs** in series. For each LED, **24 bits** are required to describe its set color. For each transmitted bit, a **16 bit** control register must be populated via DMA. The combination of these factors means that the RAM usage requirement would rise to **24 kilobytes** in a full implementation. In cases where we would like to store multiple "animation frames" of LED matrix data, the memory usage is prohibitively great.

Upon further [research](https://ralimtek.com/posts/2021/ws2812/), I decided that storing a significantly smaller buffer would suffice, when given the proper design consideration. In its current iteration, the LED driver transfers PWM timing configuration from a buffer containing just two 16 bit values. In order to correctly set the values in this buffer, I make use of the *DMA half transfer* and *DMA transfer complete* interrupts to update the half of the buffer that is not actively being copied to the configuration register. Although this approach introduced new CPU overhead (in the form of the DMA interrupts), it reduced CPU overhead in a different area. In the prior implementation, calling the LED driver to update a color value required immediately translating it into the proper sequence of PWM configuration values. Now this process is taken care of by the introduction of the aforementioned DMA interrupts. An additional benefit to the new driver implementation is that the LED matrix state can easily be queried by the application, as it is now stored in raw RGB values instead of PWM timing data. The RAM savings for this approach are significant, now taking only **1.5 kilobytes (a 93.75% reduction)**. If a further reduction is needed in the future, this approach allows us to easily reduce the bit depth of the RGB color data without significant code modifications. The full driver code can be viewed in our open source [repository](https://github.com/alanssitis/smart-air-hockey-table/).

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
**Project Hours Since Last Report:** 12 \
**Cumulative Semester Project Hours:** 21

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
**Project Hours Since Last Report:** 9 \
**Cumulative Semester Project Hours:** 9

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
