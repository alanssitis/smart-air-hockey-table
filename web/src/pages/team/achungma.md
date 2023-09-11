---
layout: ../../layouts/MDLayout.astro
title: Alan Chung Ma Progress Report
---

# Progress Report for Alan

## Week 3

**Date:** 2023-09-05 \
**Total Hours:** 18

### Description of Project Design Efforts

Over the past week, I was in charge of designing and building a
[new website](https://engineering.purdue.edu/477grp5/) for the team, the
[A3 - Software Overview](</477grp5/documents/A3 - Software Overview.docx>)
report for the team, and continued prototyping with neopixel lights. I also
contributed to some hardware prototyping and design discussions with the team.

#### Website ~ _11 hours_

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

#### Software Overview Project Report ~ _6 hours_

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

#### Hardware Prototyping and Design Decisions ~ _1 hour_

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
