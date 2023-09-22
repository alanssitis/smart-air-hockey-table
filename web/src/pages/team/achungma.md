---
layout: ../../layouts/MDLayout.astro
title: Alan Chung Ma Progress Report
---

# Progress Report for Alan

## Week 4

**Date:** 2023-09-15 \
**Total Hours:** 10

### Description of Project Design Efforts

I did not have as much time to work on the project since I attended IR and
other adjacent career fairs. However, I was able to work on a wide variety of
things for the team.

#### Setting up Githup Projects for Task Management

The team faced challenges in monitoring all the tasks effectively. To
streamline this, I suggested the use of a
[kanban board](https://en.wikipedia.org/wiki/Kanban_board), enabling us to view
and manage the team’s tasks in a consolidated space. I utilized GitHub's
built-in [Projects](https://docs.github.com/en/issues/planning-and-tracking-with-projects/learning-about-projects/about-projects)
feature and linked our [repository](https://github.com/alanssitis/smart-air-hockey-table)
to a new project. You can access the GitHub project [here](https://github.com/users/alanssitis/projects/2).
Additionally, I've provided a screenshot of the board below for a quick overview.

![Screenshot of the kanban board](/477grp5/team/alan/week04/figure1.png)
_Caption: Screenshot of the GitHub project page_

#### Finalizing the System Flowchart

I worked with the rest of the team to make the project's flowchart. Below is a
cleaned up diagram of it. It was adapted from the diagram drawn from the
[Software Overview](</477grp5/documents/A3 - Software Overview.docx>). One of
the major changes that whoever got scored on will have the puck, instead of
needing to place the puck in the middle of the playing field.

![Flow chart of the system](/477grp5/team/alan/week04/figure2.png)
_Caption: Flowchart of the system_

#### Work on A5 - Component Analysis

I worked on the overview of the Component Analysis and the analysis of Puck
Tracking Sensors. The document can be found [here](</477grp5/documents/A5 - Component Analysis.docx>).
Below is a table I made for my section.

| Method                      | Advantages                                                                                                           | Disadvantages                                                                                                                |
| --------------------------- | -------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| IMU embedded inside puck    | Direct tracking from the puck                                                                                        | Complexity, IMU errors, requires embedding a computer in the puck that can send position information to an external computer |
| Using a camera              | Can potentially track multiple objects, common method for object tracking, simple to install                         | Requires more powerful external computing in order for it to be performant, weird edge cases                                 |
| Grid of Hall effect sensors | Could integrate with the daughter boards that will hold the RGB LEDs, could use a basic algorithm that is performant | Requires meticulous placement of all sensors, potential interference from other components                                   |

#### Update Project Description & PSDR

I also worked on finalizing the team's PSDRs and updating the Project
Description in the home of the team webpage.
[Here](https://github.com/alanssitis/smart-air-hockey-table/pull/38) is a link
to the pull request for the change. I made sure to make changes based on course
staff feedback.

#### STM32 Tooling

Ben mentioned once in a meeting that VSCode had an STM32 extension. Me and Ben
both run our machines on Linux, and in order to make sure that we are able to
work efficiently, I did some exploratory work with the plugin. The extension
info can be found below

```
Name: STM32 VS Code Extension
Id: STMicroelectronics.stm32-vscode-extension
Description: STM32 embedded development support added to Visual Studio Code.
Version: 1.0.0
Publisher: STMicroelectronics
VS Marketplace Link: https://marketplace.visualstudio.com/items?itemName=stmicroelectronics.stm32-vscode-extension
```

I was able to install and run all of the required tooling and the extension
itself. Below are screenshots of STM32CubeMX running and our current project under
firmware opened on the side.

![STM32CubeMX Running on PopOS!](/477grp5/team/alan/week04/figure3.png) 
_Caption: STM32CubeMX Running on PopOS!_

![STM32 VS Code Extension running Team 5's Project](/477grp5/team/alan/week04/figure4.png)
_Caption: `/firmware` opened as an STM32 project in VSCode_

However, there were some issues with building projects and the such.

#### Next Steps

The next two weeks will be focused on designing and building the table. Me and
Will will be working together to get a proper design ready and start working
with the [Bechtel Innovation Design Center](https://www.purdue.edu/bidc/), where
we will build the table. Will has begun building it on Fusion 360. I will be
looking for ways of making table development easier since my laptop is not
running Windows.

I will also be working on figuring out the tooling for VSCode and getting it
working with our project and my teammates' machines. This way, everyone will be
able to take advantage of the newer programmer tooling available out there.

## Week 3

**Date:** 2023-09-08 \
**Total Hours:** 18

### Description of Project Design Efforts

Over the past week, I was in charge of designing and building a
[new website](https://engineering.purdue.edu/477grp5/) for the team, the
[A3 - Software Overview](</477grp5/documents/A3 - Software Overview.docx>)
report for the team, and continued prototyping with neopixel lights. I also
contributed to some hardware prototyping and design discussions with the team.

#### Website

Most of my work was done creating the new website for the team. Early on, me and
Ben realized that it would be very tedious and inefficient for each member to
write in `HTML` for the progress reports. Therefore, I spent about 11 hours
(probably a bit more) on building a website from scratch that has all of the
same functionality as the previous website but with better user/write experience.

It was written from scratch using [Astro](https://astro.build/) for the web
framework and [tailwindcss](https://tailwindcss.com/) for the styling. This was
a combination I have used previously for my personal website
[www.alanchungma.com](https://www.alanchungma.com/).

In order to maintain most of the features, I knew that I needed to provide the
following sections in addition to a navigation bar I got
[here](https://v1.tailwindcss.com/components/navigation) with various changes:

```bash
/ -> A Home page
/team/ -> Team page with profiles of all the members
/team/{MEMBERS} -> A subpage for each member which will house the progress report
/documents/ -> A page with all of the reports and other documents and files for the project
/references/ -> A page that holds all datasheets and references for the team
/media/ -> A page that will show pictures and videos for the team
/contact/ -> A page to reach out to the members
```

    I will omit taking screenshots of the website, as you can view and interact
    with the website here! A picture cannot show the neat hover effects.

The source code of the website itself is available in the [project repo](https://github.com/alanssitis/smart-air-hockey-table/tree/main/web).

#### Software Overview Project Report

I was tasked with writing the software overview for the team. This document can
be accessed in the [Documents](/477grp5/documents/) page. Since the team has
mostly been working on chosing and deciding on the hardware we will be working
on, a lot of work was done in designing solutions and create a good software
overview that covered as much ground as possible. Only a few things in the
report have been discussed in passing, therefore, it took a good amount
of time to get the whole document working. The main concern was the state
diagram for the project, which can be viewed below.

![overall system state diagram](/477grp5/team/alan/week03/figure1.png)
_Caption: State diagram of the main microcontroller_

I also created a flowchart that represents the game flow.

![game flow chart](/477grp5/team/alan/week03/figure2.png)
_Caption: Flow chart for a typical game_

#### Hardware Prototyping and Design Decisions

All that I did for Hardware Prototyping was getting the neopixel lights to work
with a basic arduino script I made:

```cpp
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        6
#define NUMPIXELS 30

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
}

void loop() {
  pixels.clear();

  for(int i=0; i<NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
    delay(DELAYVAL);
  }
}
```

Will picked up most of the prototyping work for the neopixels since I was
working on the software overview.

Throughout the week, numerous discussions were had about the hardware. We ended
up deciding to removing row master microcontrollers, and changing the number of
LEDs on a daughter board to 4 instead of 9. The later decision was made because
having the LEDs staggered in a 3x3 was very costly and would not not have netted
the team better graphics since that relies on a less dense array of 2x2 hall
effect sensors. Below is some ASCII art that displays this gap.

```
________________________
|                      |
|  L       L        L  |
|     H          H     |
|                      |
|                      |
|  L       L        L  |
|                      |
|     H          H     |
|  L       L        L  |
|                      |
________________________
```

As can be viewed from above, if you were to place two PCBs next to each other,
there will be varying densities of lights between each hall effect sensor, where
some gaps have only one light between each sensor while other gaps have two
sensors between each gap.

We were also discussing the usage of a high-speed mux to multiplex all of the
hall effect sensors from a single master microcontroller. However, Ben and Trevor
came up with a much smarter solution of `OR`ing each column and row and look at
the problem as a [break-beam problem](https://www.digikey.com/en/maker/blogs/2022/add-an-ir-break-beam-sensor-to-an-arduino-project-for-object-detection).
This idea was originally scratched since pushers would have blocked the beams,
but since this is done by the hall effect sensors and the puck should be the only
magnetic object on the table, it now works.

Below is an image Ben drew up showing this method of detecting the puck.

![Ben's brainstorm of solution](/477grp5/team/alan/week03/figure3.jpg)
_Caption: "Battleship" puck detection system_

## Week 1 & 2

**Date:** 2023-09-01 \
**Total Hours:** 16

### Description of Project Design Efforts

I have spent most of the past two weeks working on the design and technical
specifications of the project. Most of this process was on a shared whiteboard
in the lab. I also set up some of the infrastructure the team will be using for
the remainder of the semester. I will be writing a lot of what has been
discussed to the team's documentation in PR over the weekend; diagrams will be
added to the documentation as well.

First, I set up a repository which would hold all of the team's source code and
documentation; it can be found in the "SOURCE" button of the navigation bar
above, or here. This will include technical documentation for our reference,
microcontroller source code, and also a centralized version control for the
website. The reason for the addition of the website to the repo was because the
team encountered permission issues when attempting to access the destination
over SSH. This is still the case as of 09/01/2023 10:18 am.

Second, I contributed extensively to the Final Project Proposal, which can be
found under "Documents". I wrote the project description, my corresponding team
member section and wrote the first draft of our PSDRs. These PSDRs were then
revised to align more closely to the course requirements, but each of their
functionality remained the same.

Third, me and Ben discussed the feasibility and specifications of the table we
will be constructing. The consensus was that building our own table will give us
the most margin of error and the flexibility of changing the size and
capabilities of the project at will. Another notable limitation of using a
store-bought table was that taking it apart, modeling, and retrofiting the whole
top would have taken more effort and felt out of scope of the project; using an
off-the-shelf table would also pigeonhole the team to physical constraints that
are unknown to us. We also decided that the interim playing surface dimensions
will be of 1600mm x 800mm.

Fourth, me and Ben discussed the electronics layout of the components that will
be under the playing surface. The array which will be on each daughter board
that will sit under the surface will contain 4 Hall Effect sensors and 9
programmable LEDs; this is pending a feasibility test. These will be laid down
in a 16x8 matrix under the playing surface, with a "row master" placed at the
head of one row/column. Each row master will be connected to the master
microcontroller, which will reside in its own PCB. Goal slot sensors will be
connected directly to the main PCB, and an additional display will be attached
to display supplemental information such as game score. The precense of a
separate microcontroller in each daughter board is yet to be determined.

Fifth, me, Ben and Trevor discussed a possible algorithm to track the puck. A
matrix of low precision (1-bit or 2-bit values) will be used to track the
"weights" of each sensor. This will then be used to compute an estimated
position by finding the average of all weights (a possible path of optimization
is using SIMD instructions).

Sixth, I worked on the Functional Description and Theory of Operation of the
Functional Specification, which can also be found in "Documents".

Lastly, I had the chance to play with some LED lights I have. I was not able to
get much progress, but I am planning to look into how the communication protocol
is implemented and start building a library which the team will then use to
control the lights.
