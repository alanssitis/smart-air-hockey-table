---
layout: ../../layouts/MDLayout.astro
title: Ben Owen Progress Report
---

# Progress Report for Ben

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
