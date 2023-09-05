---
layout: ../../layouts/MDLayout.astro
title: Will Dobert Progress Report
---

# Progress Report for Will

## Week 1 & 2

**Date:** 9/1/2023 \
**Total Hours:** 12

### Description of Project Design Efforts

#### Microcontroller Software Setup

This week, I was primarily focused on setting up a software build environment for our main microcontroller, the STM32F466. I did this by installing the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) and generating a straightforward test program. As Trevor has some prior experience with this software package, I worked alongside him to learn the various features and tools it has to offer. In tandem with setting up the build environment for software, I also began working with our microcontroller development board, and succeeded in flashing a functional test program to it. I took extra steps to ensure that the standard printf function works as expected, so now device logs can be viewed in a connected [PuTTY](https://www.putty.org/) terminal.

I felt these steps were important to complete as soon as possible, so we can focus on designing our software in the coming weeks. Throughout this process, I took care to collect any relevant datasheets or reference manuals I encountered. I uploaded them to our team's shared file storage, so we can easily refer to them in the future. It is encouraging to see the hardware and software working together already at this early stage.

#### Team Assignments / Documentation

Within the first week, I collaborated with my team on our Final Project Proposal. Beyond filling in my info and delegated roles, I contributed by revising the drafted PSDRs to fit the proper phrasing ("An ability to..."), as well as reorganizing them for clarity and conciseness. During our meeting to collaborate on the Functional Specification document, I authored a Peripheral Hierarchy flowchart to visualize and clarify our intended design approach. It is my intention that this diagram be revised and expanded over time as our project has more details finalized.

![Peripheral Hierarchy](/public/team/will/PeripheralHierarchy.png)
_Figure 1: Provisional draft of project's Peripheral Hierarchy_

In regards to the aforementioned Functional Specification, I am currently documenting our project's known computational and thermal/power constraints.

In the next week, I look forward to prototyping with hardware and determining the feasibility of our current design plans. I also plan to begin laying out the high-level design for our software architecture.
