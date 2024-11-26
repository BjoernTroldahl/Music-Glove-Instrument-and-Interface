# Flex Sensor Music Glove and GUI

A musical instrument consisting of flex sensors mounted onto the fingers of a glove. 

The glove is connected to a GUI software that has auditory feedback and plays musical notes whenever you bend the fingers of the glove within a certain threshold.

You interact with the program by bending the fingers of the glove to play the musical notes shown on screen and otherwise using the mouse to click on buttons.

The mapping of the notes are illustrated on the rightmost following image:

<img src="https://github.com/user-attachments/assets/e9d852c5-2898-43fa-8360-3d5a6b6b4bde" width="742" height="512" />
![FlexGloveMAPPING](https://github.com/user-attachments/assets/5f10c3c6-4972-41b7-a440-814b8c6c3377)

All sound processing is made with the [Faust](https://faust.grame.fr/) programming language.
Supports both monophonic (single notes) and polyphonic sounds (chords) with a game mode for each.

## Prerequisites for using the program

Hardware omponents needed:

* 1 x Teensy 4.0 microcontroller 
* 4 x flex sensors
* 4 x 10k Ohm resistors
* 1 x duct tape
* 1 x work glove
* 2 x 20 x 20 mm grove systems
* 2 x grove cables
* 1 x strip board
* 6 x pinheads
* 6 x female-female jumper wires
* 8 x teflon wires

Software components needed (should be installed on your computer prior to running the program):
* [JUCE](https://juce.com/) (for everything GUI)
* [Arduino IDE](https://www.arduino.cc/en/software) (serial communication from hardware to software)

## Circuit diagram for connecting the glove's flex sensors to the Teensy 4.0

<img src="https://github.com/user-attachments/assets/47069000-e652-43d5-b2e5-3afc92de898a" width="750" height="351" />

## Setup of the glove with self-made and soldered circuit board

![FlexGlovePCB](https://github.com/user-attachments/assets/0c9e31c0-ee40-48e9-82e4-da3759ea5f21)

## How to download the repository and install properly




