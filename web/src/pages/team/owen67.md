---
layout: ../../layouts/MDLayout.astro
title: Ben Owen Progress Report
---

# Progress Report for Ben

## Weeks 8-9

**Date:** 2023-10-20 \
**Project Hours Since Last Report:** 20 \
**Total Hours:** 114

### Description of Project Design Effors

**Midterm Design Presentation**

We spend a lot of time putting together and practicing our midterm design presentation.  We were the first group presenting.  A link to our final presentation can be seen on the Documents page of our website.

**Sensor PCB Soldering/Testing**

We wanted to be able to test a longer strip of PCBs in order to confirm our hall effect and individually-addressable LED configuration for the final run of boards.  I spent a lot of time soldering new sensor boards, and fixing some that weren't soldered correctly.  We were able to confirm that our hall effect AND gate logic works perfectly, and scales well across longer and longer PCB runs.  We also confirmed that there were no apparent issues with the LEDs, as signal could be transmitted along and back the entire 9-board configuration.

A picture of this long sensor PCB testing can be seen in Figure 1.

<img src="/477grp5/team/ben/week9-sensor-pcb-testing.jpg" width="80%">

_Figure 1: Sensor PCB testing_

**Master PCB external power test**

We finally got a chance to test our onboard buck converter on the master PCB while powering the STM32.  We prepared a simple program to flash to our STM32, and while powering with an external 5V source, we were able to program and read flash data from the microcontroller.  A screenshot from this testing can be seen in Figure 2.

<img src="/477grp5/team/ben/week9-stm32-connected.jpg" width="80%">

_Figure 2: STM32 externally powered and connected_

**PCB reviews and ordering**

For our master PCB, we already had Revision B finished and ready to order.  However, upon preparing to order the second round of sensor PCBs, we decided to price out other suppliers for our components.  After much discussion, we learned that JLCPCB's assembly service was actually cheaper than us hand-soldering these boards, and it was more likely that the soldering quality would be higher.  We then spend a long time preparing production files for JLCPCB assembly, sourcing part numbers and ensuring the pick and place files were correct.

A render from JLCPCB's pick and place confirmation tool can be seen in Figure 3

<img src="/477grp5/team/ben/week9-sensor-pcb-pickandplace.png" width="80%">

_Figure 3: JLCPCB sensor PCB pick and place render_

**Drivers and state machine firmware**

Since the PCBs are finished, I had a chance to work on firmware and prepare for implementing the entire sensor PCB array in a few weeks.  This week, I helped write a driver to turn our outlet relay on/off.  I also worked on a driver to read in our hall effect sensor row/column data and insert it into an easy-to-use variable.  This involved a lot of planning and preparation to correctly shift each input data bit to the correct location.  Code snippets for this driver can be seen in Figure 4.  Full driver code and implementation are on our Github repository.

```
void Driver_HallEffect_PollInputs()
{
	// need to shift hall data to correct bit position
	// shift amount calculated by function (IDR bit position - desired bit position)
	//
	// example: ROW1 is on PA7, so IDR is bit 7
	// desired position is bit 1 (2nd from right), so we calculate shift amount to be (7 - 1) = 6
	// final shift is >> 6
	//
	// positive values are right shift
	// negative values are left shift
	// zero value is no shift

	uint16_t result_rows = 0xFFFF;
	result_rows ^= (ROW0_GPIO->IDR & ROW0_IDR) >> 6;
	result_rows ^= (ROW1_GPIO->IDR & ROW1_IDR) >> 6;
	result_rows ^= (ROW2_GPIO->IDR & ROW2_IDR) >> 3;
	result_rows ^= (ROW3_GPIO->IDR & ROW3_IDR) >> 2;
	result_rows ^= (ROW4_GPIO->IDR & ROW4_IDR) << 3;
	result_rows ^= (ROW5_GPIO->IDR & ROW5_IDR) << 2;
	result_rows ^= (ROW6_GPIO->IDR & ROW6_IDR) >> 1;
	result_rows ^= (ROW7_GPIO->IDR & ROW7_IDR) >> 1;
	result_rows ^= (ROW8_GPIO->IDR & ROW8_IDR) >> 1;
	result_rows ^= (ROW9_GPIO->IDR & ROW9_IDR) >> 1;
	result_rows ^= (ROW10_GPIO->IDR & ROW10_IDR) >> 1;
	result_rows ^= (ROW11_GPIO->IDR & ROW11_IDR) >> 1;
	result_rows ^= (ROW12_GPIO->IDR & ROW12_IDR) >> 1;
	result_rows ^= (ROW13_GPIO->IDR & ROW13_IDR) >> 1;
	result_rows ^= (ROW14_GPIO->IDR & ROW14_IDR) >> 1;
	result_rows ^= (ROW15_GPIO->IDR & ROW15_IDR) << 5;
	halleffect_rows = result_rows;

	uint32_t result_cols = 0xFFFFFFFF;
	result_cols ^= (COL0_GPIO->IDR & COL0_IDR) >> 2;
	result_cols ^= (COL1_GPIO->IDR & COL1_IDR);
	result_cols ^= (COL2_GPIO->IDR & COL2_IDR) << 2;
	result_cols ^= (COL3_GPIO->IDR & COL3_IDR) >> 6;
	result_cols ^= (COL4_GPIO->IDR & COL4_IDR) >> 5;
	result_cols ^= (COL5_GPIO->IDR & COL5_IDR) >> 2;
	result_cols ^= (COL6_GPIO->IDR & COL6_IDR);
	result_cols ^= (COL7_GPIO->IDR & COL7_IDR) >> 2;
	result_cols ^= (COL8_GPIO->IDR & COL8_IDR) << 4;
	result_cols ^= (COL9_GPIO->IDR & COL9_IDR) << 6;
	result_cols ^= (COL10_GPIO->IDR & COL10_IDR) << 3;
	result_cols ^= (COL11_GPIO->IDR & COL11_IDR) << 5;
	result_cols ^= (COL12_GPIO->IDR & COL12_IDR) << 7;
	result_cols ^= (COL13_GPIO->IDR & COL13_IDR) << 9;
	result_cols ^= (COL14_GPIO->IDR & COL14_IDR) << 11;
	result_cols ^= (COL15_GPIO->IDR & COL15_IDR) << 13;
	result_cols ^= (COL16_GPIO->IDR & COL16_IDR) << 15;
	result_cols ^= (COL17_GPIO->IDR & COL17_IDR) << 17;
	result_cols ^= (COL18_GPIO->IDR & COL18_IDR) << 6;
	result_cols ^= (COL19_GPIO->IDR & COL19_IDR) << 8;
	result_cols ^= (COL20_GPIO->IDR & COL20_IDR) << 10;
	result_cols ^= (COL21_GPIO->IDR & COL21_IDR) << 6;
	result_cols ^= (COL22_GPIO->IDR & COL22_IDR) << 10;
	result_cols ^= (COL23_GPIO->IDR & COL23_IDR) << 12;
	result_cols ^= (COL24_GPIO->IDR & COL24_IDR) << 14;
	result_cols ^= (COL25_GPIO->IDR & COL25_IDR) << 16;
	result_cols ^= (COL26_GPIO->IDR & COL26_IDR) << 18;
	result_cols ^= (COL27_GPIO->IDR & COL27_IDR) << 18;
	result_cols ^= (COL28_GPIO->IDR & COL28_IDR) << 20;
	result_cols ^= (COL29_GPIO->IDR & COL29_IDR) << 22;
	result_cols ^= (COL30_GPIO->IDR & COL30_IDR) << 24;
	result_cols ^= (COL31_GPIO->IDR & COL31_IDR) << 16;
	halleffect_cols = result_cols;
}
```

_Figure 4: Hall effect input data driver_

Other miscellaneous testing was done in the state machine, but big changes include defining the current list of states we are handling.  This enum is defined in Figure 5.

```
typedef enum {
	GAMESTATE_IDLE,		// Idle before start of the game
	GAMESTATE_SLEEP,	// Low-power mode after inactivity
	GAMESTATE_START,	// Set-up sequence for new game
	GAMESTATE_WAIT_A,	// Playing start animation for player A
	GAMESTATE_WAIT_B,	// Playing start animation for player B
	GAMESTATE_RUN,		// Game is in progress
	GAMESTATE_SCORE_A,	// Player A scored (not the winning shot)
	GAMESTATE_SCORE_B,	// Player B scored (not the winning shot)
	GAMESTATE_WIN_A,	// Player A won the game (winning shot)
	GAMESTATE_WIN_B,	// Player B won the game (winning shot)
	GAMESTATE_ERROR		// Error state
} GameState;
```

_Figure 5: GameState enum definition_

### Next steps

**State machine firmware**

As we prepare for the large-scale sensor PCB testing, we are continuing to add state machine code to ensure we handle the game states correctly.  This testing will involve simulating the full table inputs on our Nucleo/master PCB.

**Goal detection driver**

The last major driver we need to complete is the one handling the LDR/LED goal detection inputs on the STM32.  This will require state machine testing, as we may use software filtering to ensure we avoid incorrectly detecting goals during gameplay.

**PCB assembly (if possible)**

It is unlikely to happen this week, but if the PCBs arrive this week, we will assembly the master PCB and begin testing all of the sensor PCBs to ensure correct operation.  This will be a time consuming process.

---

## Week 7

**Date:** 2023-10-06 \
**Project Hours Since Last Report:** 20 \
**Total Hours:** 94

### Description of Project Design Effors

Lots of PCB design work, soldering, and testing.

**Master PCB Design**

Although we haven't found any issues with our master PCB, we decided to make some revisions for convenience for future testing. This mainly consisted of expanding the PCB size to 100x100mm and placing components more conservatively, giving us much more room on the board. The only additional components include ESD protection ICs on some of the external signals, and an optional external oscillator.

<img src="/477grp5/team/ben/week7-master-pcb-layout.png" width="80%">

_Figure 1: Master PCB Rev. B layout_

<img src="/477grp5/team/ben/week7-master-pcb-render.png" width="80%">

_Figure 2: Master PCB Rev. B render_

**Master PCB soldering**

Our Rev. A board still needed components placed. I spent a lot of time putting everything on the board, and we were able to test connecting and flashing to the STM32 before the team left for break. A picture of the PCB can be seen in Figure 3 and an image of us connected to the STM is shown in Figure 4.

<img src="/477grp5/team/ben/week7-master-pcb-soldered.jpg" width="80%">

_Figure 3: Master PCB Rev. A_

<img src="/477grp5/team/ben/week7-stm-connected.jpg" width="80%">

_Figure 4: Connected to STM32_

**Sensor PCB soldering/testing**

We put together 8 of our sensor PCBs, and tested the interconnection feature of the boards. We were able to send LED data across multiple boards. We were also able to see hall effect sensor data, also across multiple PCBs. An image of the connected boards can be seen in Figure 5.

<img src="/477grp5/team/ben/week7-sensor-pcbs.png" width="80%">

_Figure 5: Sensor PCBs_

### Next steps

**Midterm design review presentation**

Top of the priority list is finishing and practicing our design presentation. We have multiple PCBs, revisions, and schematics, so there is a lot to cover.

**Sensor PCB soldering**

We want to test a full row of sensor boards (16 boards) before ordering the rest. This is going to involve some time to solder all components and test everything.

**Master PCB testing**

We need to verify all of the components combined on our master PCB. This involves slow testing of each component. Some of these can be ported from our Nucleo testing, but will still be time consuming.

---

## Week 6

**Date:** 2023-09-29 \
**Project Hours Since Last Report:** 10 \
**Total Hours:** 74

### Description of Project Design Effors

This was exam week, but luckily we were in a position to order components, mainly our PCBs.

**PCB Review**

_Buck converter_

Before ordering PCBs last week, we made some final changes. The biggest change involved the buck converter. We ended up using copper pours to more closely match the datasheet. A screenshot showing our newer copper pour layout can be seen in Figure 1.

<img src="/477grp5/team/ben/week6-buck-converter.png" width="80%">

_Figure 1: Buck converter PCB layout_

_5V power trace_

We also added a thicker 5V power trace to power our LEDs, LDRs, comparators, and other miscellaneous 5V components on the board. We already had a 3V3 and GND pour, but the 5V line was a thin trace spanning multiple components. To give ourselves more room for higher current draw, we expanded our board and added a thick 1mm trace to carry the 5V power line to many components. This new power trace can be seen in Figure 2.

<img src="/477grp5/team/ben/week6-power-trace.png" width="80%">

_Figure 2: 5V power trace_

Some images of our PCBs can be seen in Figures 3 and 4.

| <img src="/477grp5/team/ben/week6-master-pcb.jpg" width="80%"> | <img src="/477grp5/team/ben/week6-sensor-pcb.jpg" width="80%"> |
| :------------------------------------------------------------: | :------------------------------------------------------------: |
|                     _Figure 3: Master PCB_                     |                     _Figure 4: Sensor PCB_                     |

**Testing of new components**

Many components came in, including our air blower, OLED displays, connectors, power supply, and outlet relay. Although there wasn't much time to test due to our team's exam schedule, we were able to test our power supply and outlet relay. To do this, we cut a PC power cable and connected the hot, neutral, and ground wires to the correct screw terminals on our power supply. We then connected this power supply to the outlet relay, which can be controlled with a low-voltage DC signal. We connected this to our power supply, and tested the functionality with both 3V3 and 5V. A video showing this testing can be seen in Figure 5.

<video controls muted>
  <source src="/477grp5/team/ben/week6-outlet-relay-test.mp4" type="video/mp4">
</video>

_Figure 5: Outlet relay testing and proof of operation_

### Next steps

**PCB revisions**

Getting closer to fall break, we want to make PCB changes to improve our design. Our first revision was to be in a position where we could prototype, but we already have a list of changes we want to make:

- Better placement of decoupling capacitors (under chip)
- Expand PCB size (100 x 100 mm)
- Larger mounting holes
- Better pin header placement (along edge of board)
- ESD protection for some GPIO
- Reverse polarity protection (if possible)
- Slight GPIO remapping to allow for easier rotary encoder implementation

**OLED driver**

I am going to help test the new OLED displays and create a driver for easier usage. I am planning to define an alphabet for character displays, since this display is designed to display graphics, not just text. This means that we will have to define each letters' pixel arrangement.

---

## Week 5

**Date:** 2023-09-22 \
**Project Hours Since Last Report:** 25 \
**Total Hours:** 64

### Description of Project Design Efforts

**Schematic/PCB design**

All of my efforts this week went into creating/finalizing the schematics for our master PCB and sensor PCB designs with Trevor.

_Master PCB_

The primary schematic contains all external connectors and signals that are going off-board. We used KiCad's hierarchical schematic function to keep everything clean. You can see the main STM32 block in the center, connected to our EEPROM, OLED connectors, LEDs, LDR inputs, and row/column hall effect sensors.

<img src="/477grp5/team/ben/week5-schematic-toplevel.png" width="80%">

_Figure 1: Top level schematic_

The STM32U585 is the microcontroller we decided on, and the schematic with the STM32 on it simply defines all of the pin connections, reset button, decoupling capacitors, and BOOT0 jumper. We verified the pins' position and function using the STM32CubeIDE IOC program to make sure we were using correct pins (peripheral assignment, 5V tolerance, etc.)

<img src="/477grp5/team/ben/week5-schematic-stm32.png" width="80%">

_Figure 2: STM32 schematic_

To satisfy one of our PSDRs, we are implementing a buck converter to power our STM32. We chose the TLV62568 due to its simplicity, small size, low price, and high enough output current at 1 amp. The schematic for this regulator can be seen in Figure 3.

<img src="/477grp5/team/ben/week5-schematic-buck.png" width="80%">

_Figure 3: Buck converter schematic_

Goal detection is achieved by using an LED/LDR pair. This analog value is then compared against a reference voltage (created with a voltage divider) in a comparator circuit to convert this analog value to a digital value. This digital signal is then connected to the STM32 to determine if a goal has been scored. The schematic for our goal detection comparator circuit can be seen in Figure 4.

<img src="/477grp5/team/ben/week5-schematic-goal-detection.png" width="80%">

_Figure 4: Goal detection schematic_

One of our stretch goals is to have a menu system displayed on an OLED to allow users to change brightness, goals required to win, and other settings. To allow the user to interact with the microcontroller, we are attempting to implement a rotary encoder with an integrated button. The schematic representing this circuit can be seen in Figure 5.

<img src="/477grp5/team/ben/week5-schematic-encoder.png" width="80%">

_Figure 5: Encoder schematic_

The LEDs are interfaced from the STM32 through level shifters. The WS2812B LEDs we are using run at 5V logic, and our STM32 can only provide 3.3V. We implement the level shifter that Will had been testing with to support up to four independent channels for the LED control. The schematic for these level shifters can be seen in Figure 6.

<img src="/477grp5/team/ben/week5-schematic-level-shifter.png" width="80%">

_Figure 6: LED level shifter schematic_

One of our other stretch goals is using an EEPROM to store table configuration and animation frames. This is interfaced with our microcontroller over SPI. The schematic representing this chip and its supporting hardware can be seen in Figure 7.

<img src="/477grp5/team/ben/week5-schematic-eeprom.png" width="80%">

_Figure 7: EEPROM schematic_

After the schematic was put together, we routed the first revision of our PCB. We want to get these ordered as soon as possible to allow for debugging and ordering of a second round before week 8. Many considerations, such as power requirements, decoupling capacitors, and communication protocols were considered when laying out components. A side-by-side view of the layout and 3D renders can be seen in Figures 9 and 10.

| <img src="/477grp5/team/ben/week5-master-layout.png" width="80%"> | <img src="/477grp5/team/ben/week5-master-render.png" width="80%"> |
| :---------------------------------------------------------------: | :---------------------------------------------------------------: |
|                   _Figure 9: Master PCB layout_                   |                 _Figure 10: Master PCB 3D render_                 |

_Sensor PCB_

In addition to the master PCB, I worked a lot with Trevor doing the same process for our hall effect sensor PCB. This was a simpler design. We have 4 hall effect sensors per PCB, using AND logic gates to convert these values to rows/columns. The data can be daisy-chained between PCBs, allowing the microcontroller to only need 48 GPIO inputs to determine the position of the puck. In addition to the sensors, the schematic contains the LEDs and LED data signals, power passthrough for components on the boards, and solder jumpers to ensure non-floating inputs on our logic gates for PCBs with only two inputs (the first PCB in each row/column only has 2 inputs since there is no data being passed into it from another board). The current schematic for this design can be seen in Figure 11.

<img src="/477grp5/team/ben/week5-schematic-sensor.png" width="80%">

_Figure 11: Sensor PCB schematic_

Figures 12 and 13 show the routing and 3D render for this PCB. The main considerations were the edge connectors allowing for PCB-to-PCB connections, as well as a fixed 50mm distance between the hall effect sensors. The large hole in the center is to assist in airflow for the PCBs mounted directly over the blower on the table.

| <img src="/477grp5/team/ben/week5-sensor-layout.png" width="80%"> | <img src="/477grp5/team/ben/week5-sensor-render.png" width="80%"> |
| :---------------------------------------------------------------: | :---------------------------------------------------------------: |
|                  _Figure 12: Sensor PCB layout_                   |                 _Figure 13: Sensor PCB 3D render_                 |

### Next steps

**PCB review/ordering**

I will probably look over these boards for a bit longer to try and find any mistakes in the schematic/routing before we order the boards. Hopefully everything looks good, but I expect to spend a few hours checking again.

**Table state machine**

Once the PCBs are ordered, we will need to wait to do anything hardware related until they arrive. I will be helping the other team members, primarily on the firmware for the game state machine. The current tasks include:

- Creating the game superloop triggered off of a timer
- Defining all of the states for the game
- Defining all of the criteria to change states
- Determining where/when to use interrupts and when to use polling from the superloop

---

## Week 4

**Date:** 2023-09-15 \
**Project Hours Since Last Report:** 12 \
**Total Hours:** 39

### Description of Project Design Efforts

**Puck tracking/hall effect sensors**

The new magnets arrived and we found them to be much higher quality (and much stronger) than the ones we were using previously. However, with our new design for battleship-style puck position tracking, we no longer need to use analog hall effect sensors, and can instead use simpler digital hall effect sensors. Unfortunately, however, the ECE shop doesn't stock these models of sensors, so we will be ordering some this week along with our first round of PCBs.

**Individually-addressable LED driver**

The majority of my time was spent working with Will on a driver to control the individually-addressable LEDs we will be placing under the table. This is because we are not using a library from another source, but wanted to take advantage of the peripherals on our STM32 to maximize performance. The driver we came up with uses a timer in PWM mode with the DMA to adjust the pulse width in order to create an 800KHz signal for the LEDs.

My work on the driver included the initial setup of the timer at the correct baud rate. After trying to adjust the PWM duration using the CPU, we found that we were unable to change the timer's CCR register fast enough for reliable data transmission. Therefore, we pivoted to a DMA-based design, where we store the CCR register values in memory and allow the DMA to cycle through them automatically. This DMA event is triggered by the timer interrupt, so there is virtually no CPU overhead and no processing needs to occur to display the frame data on the LEDs. We were able to achieve refresh rates well above 60Hz, likely into the hundreds (too fast to see with the naked eye). This will obviously lessen with more LEDs in the chain, but it is very promising for our table's design.

An example of the work I put into the driver can be seen in Figures 1 and 2.

<img src="/477grp5/team/ben/week4-led-dma-brainstorming.jpg" width="40%">

_Figure 1: LED driver with DMA brainstorming_

<img src="/477grp5/team/ben/week4-led-driver-demo.jpg" width="40%">

_Figure 2: LED driver demonstration (cycles between RGB spectrum)_

**Microcontroller research and part selection**

Our team met for around 3-4 hours on Thursday to try to finalize which microcontroller we wanted to order and implement for our first round of PCBs. After a long discussion, we decided on the [STM32U585](https://www.st.com/en/microcontrollers-microprocessors/stm32u575-585.html) series, which falls in their "ultra-low power" family of microcontrollers. Although power isn't the biggest concern on this project, this particular microcontroller offers a relatively fast clock speed of 160MHz, as well as a large ROM size of 2MB.

This microcontroller comes in a LQFP100 package, which will be relatively easy to solder and has just enough I/O for our application. We determined in our meeting that we need 68 I/O pins, and this microcontroller offers 82. We also consider other families of STM32, such as the F7 and H5 series, but we found that they were either too expensive, too large, or otherwise unnecessary for the scale of our project. The U5 family is a good balance of the performance characteristics we need for our project.

**Microcontroller pin determination**

After deciding on a microcontroller, I spent some time creating a document where we can start to assign pins on the microcontroller to peripherals in our project. This is going to be a time-consuming process, and this is after the hours we already spend transposing the information from the datasheet to a spreadsheet for our own use. So far, I spent time going through each pins' alternate functions and additional functionality and marked anything of interest, such as timer channels, SPI channels, oscillator inputs/outputs, etc. I also marked pins that we will leave open, such as the BOOT0 pins to force bootloader booting, the SWD debugging pins, and the reset button pin. An image from this document can be seen in Figure 3.

<img src="/477grp5/team/ben/uc-pin-descriptions-week4.png" width="40%">

_Figure 3: Some of the microcontroller pin descriptions_

### Next steps

**PCB Design**

PCB design is the primary thing I will be working on this week, as we want to get these in hand as soon as possible. Furthermore, we aren't able to test our final components since they are all SMD, so we need a board to mount them to for further testing.

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

I spent a lot of my time brainstorming with Trevor various ways to enhance our multiplexing of the 512 estimated hall effect sensors we will be implementing. Our first solution involved having a dedicated STM32 on each "row" of PCBs to interface with only the hall effect sensors in its row. These microcontrollers would then report back to the master microcontroller the status of its hall sensors, where this data would be aggregated and any game-state related steps could be taken. However, this solution involves over 16 total microcontrollers, which is complex and expensive.

We then thought of using an op-amp/comparator circuit to convert the analog values to digital on each PCB, and then simply multiplex the hall sensors using analog electronics. This would be simpler to implement, but the amount of routing would make this infeasible. Either each row would have to be connected differently, or all 512 signals would have to converge at a single PCB, making routing virtually impossible. However, during the brainstorming for this idea, we came up with our likely path forward.

After drawing out a grid of hall effect sensors, we realized that by simply ORing together each row and column of sensors, we could obtain the given row and columns of active hall sensors, allowing us to pinpoint the puck's location with little overhead. In terms of I/O, we only needed (total number of rows) + (total number of columns) GPIO pins; in our current design, this number totals to 48 pins. Furthermore, there are no communication protocols adding delay, and the PCB routing becomes identical for every single PCB (excluding the master) with no additional microcontrollers. I plan to flesh this idea out on a breadboard this weekend/into next week to prove feasibility.

An example of this design can be seen in Figure 1:

<img src="/477grp5/team/ben/battleship-hall-positioning.jpg" width="40%">

_Figure 1: "Battleship" hall effect position demonstration_

**Magnet research and part selection**

The other topic Trevor and I met about was determining which magnets we wanted to procure to continue testing. We did a lot of background research and learn about different magnet grades. It appears that the current "top grade" of consumer magnets is N52, where the N stands for neodymium and the 52 refers to a complex equation relating to its field strength. Common magnets are typically N35 or possibly N42, so obtaining higher-grade magnets should improve our magnetic field.

The magnet supplier we ordered magnets from, [K&J Magnetics, Inc.](https://www.kjmagnetics.com/), lists many technical resources with many of their magnets. We originally were leaning towards purchasing small cylinder magnets such as this [D12-N52](https://www.kjmagnetics.com/proddetail.asp?prod=D12-N52) model, as the surface field was listed as 7179 Gauss, higher than many larger disc magnets. However, this comes at the expense of pull force, which at the time, we didn't believe was as important as field strength. After looking at disc magnets, such as this [DX02-N52](https://www.kjmagnetics.com/proddetail.asp?prod=DX02-N52), we noticed that although the surface field was lower, it was more concentrated around the edges, and was in fact much stronger than the cylinder magnets.

We continued to do research, and eventually found [this page discussing how the surface fields are measured](https://www.kjmagnetics.com/blog.asp?p=measuring-magnets). Reading this revealed that the magnets are measured at the center along the axis of the magnetic field. Since the field is concentrated near the edges of the disc, this leads to much higher readings for cylinders, even if they are actually weaker in practice than discs. This led us to choose the disc magnets to continue our testing in future weeks.

Figures 2 and 3 contain a comparison of the cylinder and disc magnetic fields:

|            <img src="/477grp5/team/ben/cylinder-magnet-field.png" width="80%">            |            <img src="/477grp5/team/ben/disc-magnet-field.png" width="80%">            |
| :---------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------: |
| _Figure 2: Magnetic field drawings for a cylinder magnet provided by K&J Magnetics, Inc._ | _Figure 3: Magnetic field drawings for a disc magnet provided by K&J Magnetics, Inc._ |

**LDR goal detection**

My primary goal was to determine a reliable way to detect a goal from the air hockey puck. I originally was testing IR circuits, using an IR LED and IR photodiode, but found detection complicated and unreliable. After some thought, I decided to try a simpler LED + LDR combination, using analog circuity to convert these readings to a "digital" on/off signal.

I eventually made a comparator circuit using a reference voltage (created with a voltage divider), and a second voltage divider combining the LDR and a variable resistor (in my case, half of a potentiometer). This combination was hooked up to an indicator LED to easily observe the output. After some tuning, this method was shown to be very reliable and easy to adjust the threshold of on/off if needed. These factors led to this design becoming the primary way we will pursue goal detection. Additionally, because of the circuit's low cost, we can implement 2 on each goal to add reliability in the event that one of the LDRs doesn't trigger or something breaks over the life of the table.

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
