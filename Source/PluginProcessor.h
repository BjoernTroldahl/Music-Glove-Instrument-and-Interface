/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "juce_serialport.h"
#include <iostream>
#include <sstream>
#include <string>
#include "lyd.h"
#include <Windows.h>


using namespace juce;
using namespace std;

class dsp;
class MapUI;
//==============================================================================
/**
*/
class RandomNameAudioProcessor  : public juce::AudioProcessor, public HighResolutionTimer
{
public:
    //==============================================================================
    RandomNameAudioProcessor();
    ~RandomNameAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    int pageNum = 0;
    int pageNum_OLD = 0;

    String index;
    String middle;
    String ring;
    String pinky;

    float indexNum;
    float middleNum;
    float ringNum;
    float pinkyNum;

    string playedNote="";
    String playedNoteOLD="";
    String test;

    int triggered;
    bool updateNoteA = false;
    bool updateNoteB = false;
    bool updateNoteC = false;
    bool updateNoteD = false;
    bool updateNoteE = false;
    bool updateNoteF = false;
    bool updateNoteG = false;
    clock_t start, end, startA, startB, startC, startD;
    float elapsedTime;
    float timeLimit = 200;

    //sliders

    Slider IndexUP;
    Slider IndexLOW;
    Slider MiddleUP;
    Slider MiddleLOW;
    Slider RingUP;
    Slider RingLOW;
    Slider PinkyUP;
    Slider PinkyLOW;

    //Thresholds
    float indexUpper = 280;
    float indexLower = 230;
    float middleUpper;
    float middleLower;
    float ringUpper;
    float ringLower;
    float pinkyUpper;
    float pinkyLower;

private:

    MapUI* fUI;
    dsp* fDSP;
    float** outputs;

    StringPairArray portlist;
    DebugFunction theDebugLog;
    SerialPort* pSP;
    SerialPortOutputStream* pOutputStream;
    SerialPortInputStream* pInputStream;
    bool isReady = false;

    void hiResTimerCallback();

    void initSerialPort()
    {
        //get a list of serial ports installed on the system, as a StringPairArray containing a friendly name and the port path
        portlist = SerialPort::getSerialPortPaths();

        if (portlist.size())
        {
            //open the first port on the system
            pSP = new SerialPort(portlist.getAllValues()[0], SerialPortConfig(115200, 8, SerialPortConfig::SERIALPORT_PARITY_NONE, SerialPortConfig::STOPBITS_1, SerialPortConfig::FLOWCONTROL_NONE), theDebugLog);

            if (pSP->exists())
            {
                //create streams for reading/writing
                pOutputStream = new SerialPortOutputStream(pSP);
                pInputStream = new SerialPortInputStream(pSP);
            }
        }
    }

    void readSerialPort()
    {

        // String s will contain the serial data - DBG outputs it to the Debug Window so you can see it
        if (portlist.size())
        {
            if (pSP->exists())
            {
                /*char c;
                while (!pInputStream->isExhausted())
                {
                    pInputStream->read(&c, 1);
                    DBG(&c);
                }*/

                String s;
                if (pInputStream->canReadLine())
                {   //This part of the code is what makes us able to read the values from each individual flex sensor

                    s = pInputStream->readNextLine();
                    DBG(s);

                    //INDEX FINGER individual string value
                    index = s.upToFirstOccurrenceOf(",", false, true);
                    indexNum = index.getFloatValue();

                    //Turns off all musical note buttons and resets the playedNote variable when a flex sensor is not 
                    //within its thresholds and timeLimit has not been passed.
                    playedNote = " ";
                    
                    fUI->setParamValue("A",0);
                    fUI->setParamValue("B",0);
                    fUI->setParamValue("C",0);
                    fUI->setParamValue("D",0);
                    fUI->setParamValue("E",0);
                    fUI->setParamValue("F",0);
                    fUI->setParamValue("G",0);

                    //Plays the notes from the index finger if the timer is above timeLimit (milliseconds)
                    if (updateNoteA && elapsedTime > timeLimit) {
                        fUI->setParamValue("B", 0);
                        playedNote = "A";
                        fUI->setParamValue("A",1);
                        
                    }

                    if (updateNoteB && elapsedTime > timeLimit){
                        fUI->setParamValue("A", 0);
                        playedNote = "B";
                        fUI->setParamValue("B",1);
                        
                    }

                    //MIDDLE FINGER individual string value
                    String temp1 = s.fromFirstOccurrenceOf(",", false, true);
                    middle = temp1.upToFirstOccurrenceOf(",", false, true); 
                    middleNum = middle.getFloatValue();

                    //Plays the notes from the middle finger if the timer is above timeLimit (milliseconds)
                    if (updateNoteC && elapsedTime>timeLimit){
                        fUI->setParamValue("D",0);
                        playedNote = "C";
                        fUI->setParamValue("C",1);
                    }
                    if (updateNoteD && elapsedTime> timeLimit) {
                        fUI->setParamValue("C",0);
                        playedNote = "D";
                        fUI->setParamValue("D",1);
                    }

                    //RING FINGER individual string value
                    String temp2 = temp1.fromFirstOccurrenceOf(",", false, true);
                    ring = temp2.upToFirstOccurrenceOf(",", false, true);
                    ringNum = ring.getFloatValue();

                    //Plays the notes from the ring finger if the timer is above timeLimit (milliseconds)
                    if (updateNoteE && elapsedTime> timeLimit) {
                        fUI->setParamValue("F",0);
                        playedNote = "E";
                        fUI->setParamValue("E",1);
                    }
                    if (updateNoteF && elapsedTime > timeLimit) {
                        fUI->setParamValue("E",0);
                        playedNote = "F";
                        fUI->setParamValue("F",1);
                    }

                    //PINKY FINGER individual string value
                    pinky = s.fromLastOccurrenceOf(",", false, true);
                    pinkyNum=pinky.getFloatValue();

                    //Plays the notes from the pinky finger if the timer is above timeLimit (milliseconds)
                    if (updateNoteG && elapsedTime > timeLimit) {
                        playedNote = "G";
                        fUI->setParamValue("G", 1);
                    }

                    if (pageNum == 0 || pageNum ==2 ) {
                        fUI->setParamValue("A", 0);
                        fUI->setParamValue("B", 0);
                        fUI->setParamValue("C", 0);
                        fUI->setParamValue("D", 0);
                        fUI->setParamValue("E", 0);
                        fUI->setParamValue("F", 0);
                        fUI->setParamValue("G", 0);
                    }

                    //If you are on the chord pages, you get more sustain and have more time to select your notes
                    if (pageNum == 3 || pageNum == 6 || pageNum == 19 || pageNum == 20 || pageNum == 21 || pageNum == 22 || pageNum == 23 || pageNum == 24) {
                        fUI->setParamValue("rel", 6);
                        timeLimit = 300;
                    }

                    //Otherwise the sustain is set low, so you can focus more on each individual note on the scale pages
                    else {
                        fUI->setParamValue("rel", 0.5);
                    }
                    
                    //DBG(playedNote);
                    DBG(pageNum);
                    
                    //All if and else if statements below check that the flex sensors have been bent within
                    //their threshold values before calling the if-statements above that play the notes. Note
                    //that it also starts the timer here
                    if (indexNum < indexUpper && indexNum > indexLower) {
                        updateNoteA = true;
                        start = clock();
                    }

                    else if (indexNum < indexLower) {
                        updateNoteB = true;
                        start = clock();
                    }

                    else if (middleNum <= 360 && middleNum >= 330) {
                        updateNoteC = true;
                        start = clock();
                    }

                    else if (middleNum <= 330) {
                        updateNoteD = true;
                        start = clock();
                    }

                   

                    else if (ringNum <= 400 && ringNum >= 350) {
                        updateNoteE = true;
                        start = clock();
                    }

                    else if (ringNum <= 350) {
                        updateNoteF = true;
                        start = clock();
                    }

                    else if (pinkyNum <= 410) {
                        updateNoteG = true;
                        start = clock();
                    }

                    //If none of the above are true, then all the booleans that trigger the note-playing
                    //if statements are set to false and the timer is ended.
                    else {
                        updateNoteA = false;
                        updateNoteB = false;
                        updateNoteC = false;
                        updateNoteD = false;
                        updateNoteE = false;
                        updateNoteF = false;
                        updateNoteG = false;
                        end = clock();
                        start = end;
                    }

                    //The formula that calculates the value of the milliseconds that pass whenever the timer is 
                    //active. The timer is only active when you are bending a finger within a threshold and/or
                    //playing a note.
                    elapsedTime = (start - end)/(CLOCKS_PER_SEC/1000);
                    
                }
            }
        }
    }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomNameAudioProcessor)
};
