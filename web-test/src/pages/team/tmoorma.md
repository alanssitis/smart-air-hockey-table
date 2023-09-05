---
layout: ../../layouts/MDLayout.astro
title: Trevor Moorman Progress Report
---

# Progress Report for Trevor

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

![W2m pic](/public/team/trevor/Trevor_Week1_Figure1.PNG)

_Figure 1: Example When2Meet from week 1_

Team Website

- I suggested that we should add a folder to our project repository on GitHub that mirrors our uploaded website. This would protect us from potentially losing data, allow for version control, and prevent several members from attempting to make changes concurrently. The team decided to adopt my suggestion.

Hardware

- EE Shop: At the beginning of the week, I visited the EE shop on behalf of the team to look for hall sensors that the team could use for feasibility testing. I was able to get two Hall effect sensors which were the exact model the team had been looking at previously. I also looked for any other components that may be useful for prototyping, but did not find anything else.
- Design Discussion: During the mandatory lab on 8/30/2023, Alan, Ben, and I discussed the design of the daughter PCBs. Our discussion primarily focused on how the Hall effect sensors and LEDs should be laid out and how data will flow between the microcontrollers. My main contribution to the discussion was suggesting one level of the microcontrollers should convert the hall effect sensor readings to a digital signal based on a threshold. The location of the magnet could then be determined by averaging the position of all Hall effect sensors that have been "triggered", rather than more complex computations based on the Hall effect sensor's magnitude readings. This would decrease the amount of data that would need to be transferred between the microcontrollers. Alan and Ben are both interested in exploring this method further, with Ben proposing that we include multiple thresholds to increase the resolution while still compressing the data. Feasibility testing with the Hall effect sensors will give us a better idea on whether this method of data compression will work.

![W2m pic](/public/team/trevor/Trevor_Week1_Figure2c.PNG)

_Figure 2: Puck detection examples using the threshold method_

Software

- Given that we will be using STM32 microcontrollers, the team decided that we will use [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) for primary development. I have experience with this IDE from my internship last semester, so I gave Will a quick overview of the features STM32CubeIDE provides. I especially focused on highlighting the code generation and providing a high-level explanation of the LL drivers, HAL drivers, and middle-ware available.
