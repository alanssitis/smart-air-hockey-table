---
layout: ../../layouts/MDLayout.astro
title: Trevor Moorman Progress Report
---

# Progress Report for Trevor

## Week 4

**Date:** 09/15/2023 \
**Project Hours Since Last Report:** 12 \
**Cumulative Semester Project Hours:** 43

### Description of Individual Project Work Efforts:

**Team Meetings**

- Coordinated when the team would work on the component analysis.

**Hardware**

- The magnets ordered last week arrived, which Ben and I promptly began to test. However, during the ad-hoc testing before the formal testing began, I suggested that we should consider switching to a digital hall effect sensor. The team's original intent with the analog hall effect sensor was that the threshold could be dynamically adjusted by the microcontrollers performing the ADC conversions. However, now that the team planned to condense the signals using digital logic, the analog hall effect sensors' signals would need to be converted to a digital signal using an op amp or comparator. Thus, my suggestion to use a digital hall effect sensor would remove this intermediate step. A digital hall effect sensor within the lab was used as a proof of concept, and a sample of digital hall effect sensors have been ordered. Unfortunately, this delays testing an additional week.

- Preceding the work done on the component analysis, the team met virtually Wednesday afternoon to finalize component selection. As part of this, the team performed a required I/O count, which was then used to ensure that the microcontroller selected by the team had enough GPIO pins of the correct type.

**Firmware**

- Before working to add features to the project's firmware, I reviewed and familiarized myself with the work that had been done so far. Most of the previous work has been the LED driver developed by Ben and Will.

- The Smart Air Hockey Table's firmware uses a super-loop architecture where the super-loop is invoked according to a timer. I have begun implementing the timer-based super-loop but will need to continue development next week.

- The game state machine determines what actions are performed during each iteration of the super-loop. A basic implementation was started; however, development was paused until the team had time during the mandatory lab to discuss the form of the state machine. The finalized flowchart can be found on Alan's week four progress report.

**A5 - Component Analysis**

- While the entire team worked on the document together, I led the initial draft of the "1.1 Analysis of Microcontroller" and "1.5 Analysis of Goal Detection Sensors". After I received feedback from the team on my draft, I then revised both sections. I also provided feedback on the rest of the document for my team members to review.

### Next Steps:

**Puck Tracking**

- As mentioned in the hardware section above, Ben and I will test the new, digital hall effect sensors once they have been received. We will gather measurements from "slide-by" experiments performed at various heights above the hall effect sensor. (https://www.ti.com/lit/ds/symlink/tmag5231.pdf)

**Goal Detection**

- As mentioned in the week three progress report, I will raise an external interrupt when the photoresistor-based circuit detects that a goal has been scored. Unfortunately, I was unable to begin work on this feature as I contracted an illness this week. The team was made aware of this setback, and we decided that this feature could be delayed until I recover.

**Game State Machine**

- I will implement the game state machine as shown in Alan's week 4 progress report. Some state transitions are not able to be implemented yet, so the firmware will include placeholders for where the transitions will later be implemented.

## Week 3

**Date:** 09/08/2023 \
**Project Hours Since Last Report:** 14 \
**Cumulative Semester Project Hours:** 31

### Description of Project Design Efforts

**Puck Tracking**

The development of puck tracking has continued in two ways.

- Firstly, the team has discussed which magnet would be best to use in the puck. While all team members have contributed to this conversation, Ben and I have discussed the topic the most and jointly took charge in making the final decision. Ben and I's discussions centered around whether the puck's magnet should try to maximize "pull force" or "surface magnetic field." After some research and informal testing, we determined that "pull force" was a better rough indicator, but we would also need to note the shape of the magnet's magnetic field. The shape of the magnet's magnetic field is almost entirely dependent on the shape of the magnet itself, as can be seen in the figure below. Ben and I looked at the field of several different shapes of magnets, and disk (with an "axial" magnetic direction) had the best shaped magnetic field for our application. However, we were unable to determine if we could create a configuration of multiple magnets which would be better. Hence, we ordered some smaller disk magnets and small cylinder magnets along with the large disk magnet we currently plan to use.

|<img src="/477grp5/team/trevor/Trevor_Week3_CylinderField.PNG" width="80%">|<img src="/477grp5/team/trevor/Trevor_Week3_DiskField.PNG" width="80%">|
|:---:|:---:|

_Figure 1: Example magnetic fields of a cylinder magnet (left) and disk magnet (right) provided by K&J Magnetics, Inc._

- Secondly, the team has had several discussions about ways to reduce the time it takes to read the hall effect sensors. A discussion between Ben and I have dramatically changed our planned approach to reading the hall effect sensors. Previously, we had planned to use "row master" microcontrollers which would be responsible for reading, processing, and reporting the hall effect sensor data to the main microcontroller. However, if we convert the hall effect sensor to a digital signal, then we can use logic gates to compress the hall effect sensors into a digital signal on if the puck is in that row or column. This method is similar to a suggestion made during our Initial Project Proposal, where it was suggested to create a lattice of IR sensors across the play field. The IR sensor method wouldn't work as anything on the play field could trip the sensors, but only the puck's magnet will trigger the hall effect sensors.

<img src="/477grp5/team/trevor/Trevor_Week3_HallGridDraft.png" width="40%">

_Figure 2: Partial diagram showing the proposed OR gate solution_

**Goal Detection**

- A quick, proof-of-concept circuit was constructed during this week's mandatory lab to showcase how a photoresistor could be used for goal detection. However, this circuit used an op-amp when a comparator would work better. Thus, the next day I went to the ECE shop, grabbed some comparators, and built a similar circuit, a picture and diagram of which can be seen below.

|<img src="/477grp5/team/trevor/Trevor_Week3_LDRCircuitSchematic.png" width="80%">|<img src="/477grp5/team/trevor/Trevor_Week3_LDRCircuitPic.jpg" width="80%">|
|:---:|:---:|

_Figure 3: Schematic of photoresistor circuit (left) and picture of built photoresistor circuit (right)_

**Project Description and PSDRs**

- I have drafted a new project description and adjusted the PSDRs based on feedback from instructional team given during the last mandatory lab session. I am waiting to receive feedback from the other team members on my changes before updating the team's website.

### Next Steps

**Puck Tracking**

- Once the new magnets arrive, Ben and I plan on testing how the hall effect sensor's output changes as the puck moves laterally above the sensor at different heights. This will give the team an idea of how far the playing field can be above the sensors.

- While preliminary testing showed that putting insulating materials between the magnet and the Hall effect sensor has no discernible effect, we will verify this with the new magnets as well.

- After this testing, further work on puck tracking will likely require using a model of a portion of the table's playing field.

**Goal Detection**

- I will connect the output of the circuit to one of the microcontroller's GPIO pins, then develop firmware for detecting when a goal is scored. This will likely be done with an external interrupt, although I will need to discuss with the team how exactly this subsystem should interact with the overall system.

- Later, this will need to be integrated with the overall system. Namely the game state.

**Project Description and PSDRs**

- As mentioned previously, I am currently waiting for feedback from all the other team members. Once I receive this feedback, I will amend the description and PSDRs further then update the team's website.

## Week 1 & 2

**Date:** 09/01/2023 \
**Project Hours Since Last Report:** 17 \
**Cumulative Semester Project Hours:** 17

### Description of Project Design Efforts

Initial Project Proposal

- Before meeting with the team, I emailed Dr. Walter for clarification on some of his comments that were concerned with the section that I had written. Using this clarification, I found alternate commercial products to use in our initial project proposal.
- During the meeting, I helped keep the team focused on the task at hand and gave suggestions on how we should prioritize our time. This was vital to our success given our limited time to meet.

Final Project Proposal

- During the meeting, I noticed that some of our conversations repeated past discussions. To help keep the team on the same page, I took notes for the meeting. Alan later helped format the notes that I took, and the team agreed to take meeting notes going forward.

A2 - Technical Specification

- I took notes during the draft meeting and reminded the other team members to add to the meeting notes as well.
- Towards the end of the meeting, Alan had to leave so I suggested that we divide the sections between team members.

Team Organization

- Scheduling the meeting turned out to be a challenging task given the team members' busy and rigid schedules. It often took team members a long time to respond to questions about their schedule and this led us to rescheduling our meeting a few times. I worried that this could be a recurring issue throughout the semester, so I first suggested that the team should schedule one or two regular meeting times outside of the mandatory lab. Unfortunately, the other team members did not like this approach as none of them felt they would be able to consistently keep a block of time free. As an alternative solution, I suggested that we should use [When2Meet](https://www.when2meet.com/) to schedule our meetings. When2Meet made scheduling the final project proposal meeting much easier, and the team has agreed to use this tool in the future.

<img src="/477grp5/team/trevor/Trevor_Week1_Figure1.PNG" width="25%">

_Figure 1: Example When2Meet from week 1_

Team Website

- I suggested that we should add a folder to our project repository on GitHub that mirrors our uploaded website. This would protect us from potentially losing data, allow for version control, and prevent several members from attempting to make changes concurrently. The team decided to adopt my suggestion.

Hardware

- EE Shop: At the beginning of the week, I visited the EE shop on behalf of the team to look for hall sensors that the team could use for feasibility testing. I was able to get two Hall effect sensors which were the exact model the team had been looking at previously. I also looked for any other components that may be useful for prototyping, but did not find anything else.
- Design Discussion: During the mandatory lab on 8/30/2023, Alan, Ben, and I discussed the design of the daughter PCBs. Our discussion primarily focused on how the Hall effect sensors and LEDs should be laid out and how data will flow between the microcontrollers. My main contribution to the discussion was suggesting one level of the microcontrollers should convert the hall effect sensor readings to a digital signal based on a threshold. The location of the magnet could then be determined by averaging the position of all Hall effect sensors that have been "triggered", rather than more complex computations based on the Hall effect sensor's magnitude readings. This would decrease the amount of data that would need to be transferred between the microcontrollers. Alan and Ben are both interested in exploring this method further, with Ben proposing that we include multiple thresholds to increase the resolution while still compressing the data. Feasibility testing with the Hall effect sensors will give us a better idea on whether this method of data compression will work.

|<img src="/477grp5/team/trevor/Trevor_Week1_Figure2a.PNG" width="40%">|<img src="/477grp5/team/trevor/Trevor_Week1_Figure2b.PNG" width="40%">|
|:---:|:---:|
|<img src="/477grp5/team/trevor/Trevor_Week1_Figure2c.PNG" width="40%">|<img src="/477grp5/team/trevor/Trevor_Week1_Figure2d.PNG" width="40%">|

_Figure 2: Puck detection examples using the threshold method_

Software

- Given that we will be using STM32 microcontrollers, the team decided that we will use [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) for primary development. I have experience with this IDE from my internship last semester, so I gave Will a quick overview of the features STM32CubeIDE provides. I especially focused on highlighting the code generation and providing a high-level explanation of the LL drivers, HAL drivers, and middle-ware available.
