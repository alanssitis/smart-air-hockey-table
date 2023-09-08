---
layout: ../layouts/MDLayout.astro
title: ECE 477 Team 5 Homepage
---

# SMART AIR HOCKEY TABLE

## Project Description

The project at hand involves the development of a smart air hockey table that
replicates the functionalities of a traditional arcade air hockey setup with a
twist. The notable innovation is the incorporation of a responsive surface
beneath a translucent playfield. This reactive surface, comprising strategically
positioned lights, sensors, and computational components, reacts in real-time to
the movements of the puck, generating visual displays that enhance the gameplay
experience.

The dynamic nature of the reactive surface introduces novel game modes that
transcend the boundaries of conventional air hockey. Despite its technologically
advanced features, the smart air hockey table maintains a compact form factor,
making it adaptable to a variety of flat surfaces, from regular tables to other
suitable platforms. This convergence of timeless gameplay with modern
technological advancements is poised to redefine the enjoyment of air hockey,
offering enthusiasts an immersive and unparalleled gaming encounter.

## Project Specific Design Requirements (PSDRs)

The design and development of the smart air hockey table will adhere to the
following focused design requirements to ensure a successful and captivating
gameplay experience:

1. PSDR #1 (Hardware): 
An ability to convert analog hall effect sensor data into digital signals with custom threshold values using comparators.
2. PSDR #2 (Hardware & Software):
An ability to implement custom digital logic to convert digital hall effect sensor data into precise puck row/column position data.
3. PSDR #3 (Hardware):
An ability to reliably detect goals scored by monitoring the passage of the puck through the goal using a photoresistor and LED pair.
4. PSDR #4 (Hardware):
An ability to display game and debug information on an OLED display connected via the I2C communication protocol.
5. PSDR #5 (Software):
An ability to control a grid of individually-addressable RGB LEDs beneath the playing surface using a serialized PWM communication protocol.
