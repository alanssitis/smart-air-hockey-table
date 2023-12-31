---
layout: ../layouts/MDLayout.astro
title: ECE 477 Team 5 Homepage
---

<img class="object-fill h-48 w-fit mx-auto" src="/air_hockey_table_painting.png">

# SMART AIR HOCKEY TABLE

## Project Description

The Smart Air Hockey Table (SAHT) redefines the classic arcade game, offering
an immersive experience like no other. Central to its advanced gameplay is a
vibrant grid of RGB LEDs, dynamically illustrating the puck's position and game
events. This real-time interactivity is made possible by a carefully arrayed set
of hall effect sensors, which pinpoint the puck's every move. Additionally, the
SAHT incorporates photoresistor and LED pairs at each goal slot, ensuring
precise goal detection and accurate score registration. An integrated OLED
display then showcases the game score.

The hall effect sensors are strategically embedded beneath the translucent
playing field in a grid pattern. These sensors detect changes in magnetic
fields. As the puck, equipped with a small magnet, glides across the surface,
the sensors underneath identify its precise location by sensing the magnetic
field changes caused by the puck's movement. The density and layout of this grid
ensure the puck's position is determined with high accuracy, allowing the system
to provide real-time feedback. For instance, as the puck moves, the individually
controlled LEDs beneath the surface illuminate its path, creating a mesmerizing
trail effect that enhances the overall gaming experience.

This grid-based hall effect sensor system not only offers unparalleled precision
in puck tracking but also enables the microcontroller to interpret rapid changes
in direction and speed. Such granular detail ensures that dynamic visuals
generated by the LEDs are always synchronized with the puck's position and
movement. Players, therefore, experience a highly responsive and visually rich
gameplay, where the table reacts instantly to every twist and turn of the puck.

Combined with other features like the OLED score display and photoresistor-based
goal detection, the SAHT offers an intuitive yet futuristic air hockey
experience. It seamlessly merges traditional gameplay with modern tech
enhancements, ensuring that both newcomers and seasoned players are treated to
an unmatched recreational encounter.

The SAHT taps into a standard 120 VAC wall outlet and employs an off the shelf
AC/DC regulator to produce a steady 5VDC. A buck converter provides the
microcontroller and other components on the main PCB with 3.3VDC by stepping
down from the regulator's 5VDC output. The boards hosting the RGB LEDs and Hall
effect sensors are powered directly by the 5VDC from the regulator.

## Project Specific Design Requirements (PSDRs)

The design and development of the smart air hockey table will adhere to the
following focused design requirements to ensure a successful and captivating
gameplay experience:

1. PSDR #1 (_Hardware_): An ability for the microcontroller to determine the
   puck's position on the playing surface by interpreting signals from a grid of
   hall effect switches beneath it, using digital gates to consolidate
   individual signals into row and column bit vectors.
2. PSDR #2 (_Hardware_): An ability for the microcontroller to automatically
   detect goals scored using an LED and analog photoresistor pair digitized
   using a comparator installed at each goal.
3. PSDR #3 (_Hardware_): An ability for the microcontroller to display game
   score on an OLED display using the SPI protocol.
4. PSDR #4 (_Hardware_): An ability for the microcontroller to to control a
   large grid of individually-addressable WS2812B RGB LEDs beneath the playing
   surface using the LED's custom communication protocol.
5. PSDR #5 (_Hardware_): An ability to provide power to the microcontroller at
   3.3V using a buck converter from a 5V source.

## Stretch Goal PSDRs (EPSDRs)

The following are the two stretch goals determined by the team:

1. EPSDR #1 (_Hardware_): An ability for the microcontroller to store graphical
   animations and save game score on an EEPROM.
2. EPSDR #2 (_Software_): An ability for the players to control the brightness
   of the RGB LED grid and winning game score number by interfacing with a
   rotary encoder and a menu on the OLED display.
