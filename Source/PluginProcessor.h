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

                    //INDEX FINGER
                    index = s.upToFirstOccurrenceOf(",", false, true);
                    //DBG(index);
                    indexNum = index.getFloatValue();

                    playedNote = " ";
                    //start = clock();
                    
                    fUI->setParamValue("A",0);
                    fUI->setParamValue("B",0);
                    fUI->setParamValue("C",0);
                    fUI->setParamValue("D",0);
                    fUI->setParamValue("E",0);
                    fUI->setParamValue("F",0);
                    fUI->setParamValue("G",0);

                    if (updateNoteA && elapsedTime > timeLimit) {
                        fUI->setParamValue("B", 0);
                        //DBG("A");
                        playedNote = "A";
                        fUI->setParamValue("A",1);
                        
                    }

                    if (updateNoteB && elapsedTime > timeLimit){
                        fUI->setParamValue("A", 0);
                        //DBG("B");
                        playedNote = "B";
                        fUI->setParamValue("B",1);
                        
                    }

                    //MIDDLE FINGER
                    String temp1 = s.fromFirstOccurrenceOf(",", false, true);
                    middle = temp1.upToFirstOccurrenceOf(",", false, true);
                    //DBG(temp1);
                    //DBG(middle); 
                    middleNum = middle.getFloatValue();

                    if (updateNoteC && elapsedTime>timeLimit){
                        fUI->setParamValue("D",0);
                        //DBG("C");
                        playedNote = "C";
                        fUI->setParamValue("C",1);
                    }
                    if (updateNoteD && elapsedTime> timeLimit) {
                        fUI->setParamValue("C",0);
                        ///DBG("D");
                        playedNote = "D";
                        fUI->setParamValue("D",1);
                    }

                    //RING FINGER
                    String temp2 = temp1.fromFirstOccurrenceOf(",", false, true);
                    ring = temp2.upToFirstOccurrenceOf(",", false, true);
                    //DBG(temp2);
                    //DBG(ring);
                    ringNum = ring.getFloatValue();

                    if (updateNoteE && elapsedTime> timeLimit) {
                        //DBG("E");
                        fUI->setParamValue("F",0);
                        playedNote = "E";
                        fUI->setParamValue("E",1);
                    }
                    if (updateNoteF && elapsedTime > timeLimit) {
                        //DBG("F");
                        fUI->setParamValue("E",0);
                        playedNote = "F";
                        fUI->setParamValue("F",1);
                    }

                    //PINKY FINGER
                    pinky = s.fromLastOccurrenceOf(",", false, true);
                    //DBG(pinky);
                    pinkyNum=pinky.getFloatValue();

                    if (updateNoteG && elapsedTime > timeLimit) {
                        //DBG("G");
                        playedNote = "G";
                        fUI->setParamValue("G", 1);
                    }

                    if (pageNum == 0 || pageNum == 1 || pageNum ==2 ) {
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

                    else {
                        fUI->setParamValue("rel", 0.5);
                    }
                    
                    DBG(playedNote);

                    

                    if (indexNum < 340 && indexNum > 310) {
                        updateNoteA = true;
                        //DBG("updated");
                        start = clock();
                        //updateNoteB = false;
                        
                    }

                    else if (indexNum < 310) {
                        updateNoteB = true;
                        start = clock();
                        //updateNoteA = false;
                    }

                    else if (middleNum <= 380 && middleNum >= 340) {
                        updateNoteC = true;
                        start = clock();
                        //updateNoteD = false;
                    }

                    else if (middleNum <= 340) {
                        updateNoteD = true;
                        start = clock();
                        //updateNoteC = false;
                    }

                   

                    else if (ringNum <= 420 && ringNum >= 350) {
                        updateNoteE = true;
                        start = clock();
                        //updateNoteF = false;
                    }

                    else if (ringNum <= 350) {
                        updateNoteF = true;
                        start = clock();
                        //updateNoteE = false;
                    }

                    else if (pinkyNum <= 460) {
                        updateNoteG = true;
                        start = clock();
                    }

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
                        //wait = false;
                    }

                    elapsedTime = (start - end)/(CLOCKS_PER_SEC/1000);
                    DBG(elapsedTime);
                    DBG(pageNum);
                    
                }
            }
        }
    }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomNameAudioProcessor)
};
