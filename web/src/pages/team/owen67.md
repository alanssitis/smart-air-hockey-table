---
layout: ../../layouts/MDLayout.astro
title: Ben Owen Progress Report
---

# Progress Report for Ben

## Week 4

**Date:** 2023-09-15 \
**Project Hours Since Last Report:** 12 \
**Total Hours:** 39

### Description of Project Design Efforts

**Puck tracking/hall effect sensors**

The new magnets arrived and we found them to be much higher quality (and much stronger) than the ones we were using previously.  However, with our new design for battleship-style puck position tracking, we no longer need to use analog hall effect sensors, and can instead use simpler digital hall effect sensors.  Unfortunately, however, the ECE shop doesn't stock these models of sensors, so we will be ordering some this week along with our first round of PCBs.

**Individually-addressable LED driver**

The majority of my time was spent working with Will on a driver to control the individually-addressable LEDs we will be placing under the table.  This is because we are not using a library from another source, but wanted to take advantage of the peripherals on our STM32 to maximize performance.  The driver we came up with uses a timer in PWM mode with the DMA to adjust the pulse width in order to create an 800KHz signal for the LEDs.

My work on the driver included the initial setup of the timer at the correct baud rate.  After trying to adjust the PWM duration using the CPU, we found that we were unable to change the timer's CCR register fast enough for reliable data transmission.  Therefore, we pivoted to a DMA-based design, where we store the CCR register values in memory and allow the DMA to cycle through them automatically.  This DMA event is triggered by the timer interrupt, so there is virtually no CPU overhead and no processing needs to occur to display the frame data on the LEDs.  We were able to achieve refresh rates well above 60Hz, likely into the hundreds (too fast to see with the naked eye).  This will obviously lessen with more LEDs in the chain, but it is very promising for our table's design.

An example of the work I put into the driver can be seen in Figures 1 and 2.

<img src="/477grp5/team/ben/week4-led-dma-brainstorming.jpg" width="40%">

_Figure 1: LED driver with DMA brainstorming_

<img src="/477grp5/team/ben/week4-led-driver-demo.jpg" width="40%">

_Figure 2: LED driver demonstration (cycles between RGB spectrum)_

**Microcontroller research and part selection**

Our team met for around 3-4 hours on Thursday to try to finalize which microcontroller we wanted to order and implement for our first round of PCBs.  After a long discussion, we decided on the [STM32U585](https://www.st.com/en/microcontrollers-microprocessors/stm32u575-585.html) series, which falls in their "ultra-low power" family of microcontrollers.  Although power isn't the biggest concern on this project, this particular microcontroller offers a relatively fast clock speed of 160MHz, as well as a large ROM size of 2MB.

This microcontroller comes in a LQFP100 package, which will be relatively easy to solder and has just enough I/O for our application.  We determined in our meeting that we need 68 I/O pins, and this microcontroller offers 82.  We also consider other families of STM32, such as the F7 and H5 series, but we found that they were either too expensive, too large, or otherwise unnecessary for the scale of our project.  The U5 family is a good balance of the performance characteristics we need for our project.

**Microcontroller pin determination**

After deciding on a microcontroller, I spent some time creating a document where we can start to assign pins on the microcontroller to peripherals in our project.  This is going to be a time-consuming process, and this is after the hours we already spend transposing the information from the datasheet to a spreadsheet for our own use.  So far, I spent time going through each pins' alternate functions and additional functionality and marked anything of interest, such as timer channels, SPI channels, oscillator inputs/outputs, etc.  I also marked pins that we will leave open, such as the BOOT0 pins to force bootloader booting, the SWD debugging pins, and the reset button pin.  An image from this document can be seen in Figure 3.

<img src="/477grp5/team/ben/uc-pin-descriptions-week4.png" width="40%">

_Figure 3: Some of the microcontroller pin descriptions_

### Next steps

**PCB Design**

PCB design is the primary thing I will be working on this week, as we want to get these in hand as soon as possible.  Furthermore, we aren't able to test our final components since they are all SMD, so we need a board to mount them to for further testing.

- Trevor and I will continue determining pin selections for our microcontroller and laying this design out on our first round of PCBs
- Current master PCB parts to be implemented:
	- STM32U585 footprint with SWD programming header
	- Buck converter to power STM32 from a 5VDC source
	- Connections to OLED displays, hall effect sensors, and LEDs
- Current sensor PCB parts to be implemented:
	- SMD hall effect sensor (likely TI TMAG series) footprints in 2x2 grid per PCB
	- LED footprints next to hall effect sensors, also in a 2x2 grid
	- Footprints for digital logic gates to convert to row/column position data for the microcontroller
	- Connection to the master/other sensor PCBs to daisy chain the signals along the boards

---

## Week 3

**Date:** 2023-09-08 \
**Project Hours Since Last Report:** 15 \
**Total Hours:** 27

### Description of Project Design Efforts

**Puck tracking**

I spent a lot of my time brainstorming with Trevor various ways to enhance our multiplexing of the 512 estimated hall effect sensors we will be implementing.  Our first solution involved having a dedicated STM32 on each "row" of PCBs to interface with only the hall effect sensors in its row.  These microcontrollers would then report back to the master microcontroller the status of its hall sensors, where this data would be aggregated and any game-state related steps could be taken.  However, this solution involves over 16 total microcontrollers, which is complex and expensive.

We then thought of using an op-amp/comparator circuit to convert the analog values to digital on each PCB, and then simply multiplex the hall sensors using analog electronics.  This would be simpler to implement, but the amount of routing would make this infeasible.  Either each row would have to be connected differently, or all 512 signals would have to converge at a single PCB, making routing virtually impossible.  However, during the brainstorming for this idea, we came up with our likely path forward.

After drawing out a grid of hall effect sensors, we realized that by simply ORing together each row and column of sensors, we could obtain the given row and columns of active hall sensors, allowing us to pinpoint the puck's location with little overhead.  In terms of I/O, we only needed (total number of rows) + (total number of columns) GPIO pins; in our current design, this number totals to 48 pins.  Furthermore, there are no communication protocols adding delay, and the PCB routing becomes identical for every single PCB (excluding the master) with no additional microcontrollers.  I plan to flesh this idea out on a breadboard this weekend/into next week to prove feasibility.

An example of this design can be seen in Figure 1:

<img src="/477grp5/team/ben/battleship-hall-positioning.jpg" width="40%">

_Figure 1: "Battleship" hall effect position demonstration_

**Magnet research and part selection**

The other topic Trevor and I met about was determining which magnets we wanted to procure to continue testing.  We did a lot of background research and learn about different magnet grades.  It appears that the current "top grade" of consumer magnets is N52, where the N stands for neodymium and the 52 refers to a complex equation relating to its field strength.  Common magnets are typically N35 or possibly N42, so obtaining higher-grade magnets should improve our magnetic field.

The magnet supplier we ordered magnets from, [K&J Magnetics, Inc.](https://www.kjmagnetics.com/), lists many technical resources with many of their magnets.  We originally were leaning towards purchasing small cylinder magnets such as this [D12-N52](https://www.kjmagnetics.com/proddetail.asp?prod=D12-N52) model, as the surface field was listed as 7179 Gauss, higher than many larger disc magnets.  However, this comes at the expense of pull force, which at the time, we didn't believe was as important as field strength.  After looking at disc magnets, such as this [DX02-N52](https://www.kjmagnetics.com/proddetail.asp?prod=DX02-N52), we noticed that although the surface field was lower, it was more concentrated around the edges, and was in fact much stronger than the cylinder magnets.

We continued to do research, and eventually found [this page discussing how the surface fields are measured](https://www.kjmagnetics.com/blog.asp?p=measuring-magnets).  Reading this revealed that the magnets are measured at the center along the axis of the magnetic field.  Since the field is concentrated near the edges of the disc, this leads to much higher readings for cylinders, even if they are actually weaker in practice than discs.  This led us to choose the disc magnets to continue our testing in future weeks.

Figures 2 and 3 contain a comparison of the cylinder and disc magnetic fields:

|<img src="/477grp5/team/ben/cylinder-magnet-field.png" width="80%">|<img src="/477grp5/team/ben/disc-magnet-field.png" width="80%">|
|:---:|:---:|
|_Figure 2: Magnetic field drawings for a cylinder magnet provided by K&J Magnetics, Inc._|_Figure 3: Magnetic field drawings for a disc magnet provided by K&J Magnetics, Inc._|

**LDR goal detection**

My primary goal was to determine a reliable way to detect a goal from the air hockey puck.  I originally was testing IR circuits, using an IR LED and IR photodiode, but found detection complicated and unreliable.  After some thought, I decided to try a simpler LED + LDR combination, using analog circuity to convert these readings to a "digital" on/off signal.

I eventually made a comparator circuit using a reference voltage (created with a voltage divider), and a second voltage divider combining the LDR and a variable resistor (in my case, half of a potentiometer).  This combination was hooked up to an indicator LED to easily observe the output.  After some tuning, this method was shown to be very reliable and easy to adjust the threshold of on/off if needed.  These factors led to this design becoming the primary way we will pursue goal detection.  Additionally, because of the circuit's low cost, we can implement 2 on each goal to add reliability in the event that one of the LDRs doesn't trigger or something breaks over the life of the table.

A picture of the breadboard containing this circuit can be seen in Figure 3:

<img src="/477grp5/team/ben/ldr-goal-detection-breadboard.jpg" width="40%">

_Figure 4: Image of breadboard LDR goal detection circuit_

A schematic representation of the circuit can be seen in Figure 4:

<img src="/477grp5/team/ben/ldr-goal-detection-schematic.png" width="40%">

_Figure 5: "Battleship" hall effect position demonstration_

### Next steps

**Puck tracking**

- I will build a 2x2 (or larger) hall effect grid on a breadboard and connect using the digital logic mentioned earlier
- Trevor and I will test the new magnets when they arrive to try to determine the desired air gap for our table
	- Once this is completed, we will likely need to construct a miniature version of the table to continue testing the hall sensor system

**Magnet research and part selection**

- If testing goes well with the parts that are on order, nothing more needs to be done
	- Current testing includes center magnet positioning and a ring of magnets on the puck closer to the edge
- If testing does not go well, we may reconsider the magnet layout on the puck and/or the magnet type

**LDR goal detection**

- I will work with Trevor and his comparator-focused design to finalize this circuit, including redundancy and potentially testing the digital interrupt with our STM32

**LED interfacing**

- Most of my time will likely be spent here; I will be helping Will with LED communication
- He currently has implemented a basic timer serial interface, but we will need to implement DMA for optimization at higher refresh rates

**PCB design**

- I will be working with Trevor to put together the first round of PCBs to order
- PCBs will contain:
	- LED footprints and wiring
	- Hall effect sensor footprints and wiring
	- Analog/digital logic IC footprints and wiring
	- Connectors/pads to connect PCBs to each other for larger testing
	- (Potentially) an STM32 footprint to test with embedded STM32 soldering and programming without a Nucleo development board

**Project description and PSDRs**

- As I get more testing and feedback from other group members, I will be helping revise these to get them finalized as soon as possible and update the website


---

## Week 1 & 2

**Date:** 2023-09-01 \
**Total Hours:** 12

### Description of Project Design Efforts

I spent most of these two weeks doing background research and helping complete team tasks relating to the project. Most of my expertise relates to my past embedded experience, so I have been using most of this time to brainstorm potential solutions to the numerous engineering challenges and helping define constraints for our finished project.

#### Background research

Most of the background research I did over these first two weeks related to feasibility analysis for various components. Due to the scale and complexity of our table, I investigated different possible solutions for puck tracking, communication, and LED manipulation. I analyzed prices and datasheets for various sensors, checked what parts we could potentially obtain for free using the ECE shop catalog, and brainstormed a few solutions for high speed communication of dozens of microcontrollers. Heading into the next few weeks, this research will be tested for real-world feasibility.

##### STM/STM communication

My primary research was trying to find which STM32 models would support our worst-case situation: one STM32 for each daughterboard PCB. Our master STM32 would need to be high speed and have lots of communication peripherals and I/O pins. After lots of evaluation, I found the STM32F446RE, which is both in stock and cheap. It can run at high speeds of up to 180MHz, but more importantly, has lots of onboard protocols, including quad SPI and up to 114 I/O, though we will have less available to avoid the BGA packaged version.

Another microcontroller we may explore is one of the lines with OCTOSPI. This would allow us to communicate with all of the rows at the same time. However, for potential expandability of our table, this doesn't solve the overarching problem.

#### Team Meetings

Our team held a few meetings to work on the team assignments. These meetings were mostly used to outline each member's responsibility and split the work according to our strengths, but we also brainstormed ideas for our tables. Much of the time spent in these meetings is described in the other two sections for this week's progress report.

#### Team Assignments

The first two weeks had both the revised initial project proposal, final project proposal, and the function specifications due. I spent a lot of time revising our initial proposal, primarily researching and decsribing a more relevant existing product to include in our market research section of the document. For the final project proposal, I primarily helped with the initial budget estimation, using hypothetical parts and current market pricing to create a rough price estimate for our project.

#### Brainstorming PDF

Notes from my brainstorming and diagrams about my communication brainstorming can be found at this link: [Brainstorming PDF](/477grp5/team/ben/ben-brainstorming-20230901.pdf).
