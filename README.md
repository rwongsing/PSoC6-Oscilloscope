# Oscilloscope on a PSoC6 Microcontroller
---
## Description
---
This project was written as the Final Project for CSE121/L Embedded System Design. The main folder contains the core files needed to build the project in PSoC Creator 4.4 and program the board.

## Oscilloscope Features
---
* Single or Dual Analog Channels
* Free-running or Trigger modes
* Configurable trigger channel, trigger level, and trigger slope
* Configurable X and Y scales
* Frequency measurement and display
* Potentiometer for scrolling waveforms

## Hardware Components
---
* Cypress CYBLE-416045-02 microcontroller board
* Newhaven Display 240x320 TFT LCD
* Analog Discovery 2

## Getting Started
---
This assumes that you are familiar with the software and have already set up the required hardware connections.

### Prerequisites
* PSoC Creator 4.4
* Digilent Waveforms
* Terminal with Serial Port

### Installation
1. Clone the repo
    ```sh
    git clone https://github.com/rwongsing/PSoC6-Oscilloscope.git
2. Open the project in PSoC Creator. Build the project and program the board.
3. Open a serial port using the Terminal