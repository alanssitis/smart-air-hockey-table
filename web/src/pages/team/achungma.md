---
layout: ../../layouts/MDLayout.astro
title: Alan Chung Ma Progress Report
---

# Progress Report for Alan

## Week 3
**Date:** 2023-09-05 \
**Total Hours:** 12

### Description of Project Design Efforts

Over the past week, I was in charge of designing and building a
[new website](https://engineering.purdue.edu/477grp5/) for the team, the
[A3 - Software Overview](</477grp5/documents/A3 - Software Overview.docx>)
report for the team, and continued prototyping with neopixel lights. I also
contributed to some hardware prototyping and design discussions with the team.

#### Website ~ 11 hours



#### Software Overview Project Report ~ 0.5

#### Neopixel Prototyping ~ ?

#### Hardware Prototyping and Design Decisions ~ 1

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
