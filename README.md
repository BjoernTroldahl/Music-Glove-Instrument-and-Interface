# Flex Sensor Music Glove and GUI

A school project where I built a musical instrument with finger-mounted flex sensors on the fingers of a glove. 

The project was made in collaboration with Vincent Clarkfeldt and Christopher Reimers, they did the research and design while I did the vast majority of all hardware and programming.

Construction-wise, the glove is connected to a GUI software that has auditory feedback and plays musical notes whenever you bend the fingers of the glove within a certain threshold.

You interact with the program by bending the fingers of the glove to play the musical notes shown on screen, and otherwise using the mouse on your computer to click on buttons.

All sound processing is made with the [Faust](https://faust.grame.fr/) programming language.

Supports both monophonic (single notes) and polyphonic sounds (chords) with a game mode for each.

The mapping of the notes are illustrated on the second image below:

<img src="https://github.com/user-attachments/assets/e9d852c5-2898-43fa-8360-3d5a6b6b4bde" width="500" height="367" />

<img src="https://github.com/user-attachments/assets/5f10c3c6-4972-41b7-a440-814b8c6c3377" width="278" height="367"/>

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
* [JUCE](https://juce.com/) (for everything GUI-related)
* [Arduino IDE](https://www.arduino.cc/en/software) (serial communication from hardware to software)

## Circuit diagram for connecting the glove's flex sensors to the Teensy 4.0

<img src="https://github.com/user-attachments/assets/47069000-e652-43d5-b2e5-3afc92de898a" width="750" height="351" />

## Setup of the glove with self-made and soldered circuit board

![FlexGlovePCB](https://github.com/user-attachments/assets/0c9e31c0-ee40-48e9-82e4-da3759ea5f21)

## How to download the repository and install properly

1. Download this GitHub repository as a ZIP-file under the Code tab and unzip it into a folder.
   
2. Connect the flex sensor glove and Teensy 4.0 circuit to an available USB-port on your computer.

3. Go to the Arduino folder inside the unzipped GitHub repository folder. Then double-click the P4prototype.ino file and run it.

4. In JUCE, click Open Existing Project > select the file path of the now unzipped repository folder and then click on the P4 project.jucer file.

5. Make sure that audio output from your PC speakers is enabled (or whatever audio output device you want to use).

6. Inside the JUCE project, click on the Run icon to start the program.

7. Enjoy using the music glove and its GUI! 



