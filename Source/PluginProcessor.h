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

    int triggered;

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
                    //DBG(s);

                    //INDEX FINGER
                    index = s.upToFirstOccurrenceOf(",", false, true);
                    //DBG(index);
                    indexNum = index.getFloatValue();

                    playedNote = " ";
                    fUI->setParamValue("A",0);
                    fUI->setParamValue("B",0);
                    fUI->setParamValue("C",0);
                    fUI->setParamValue("D",0);
                    fUI->setParamValue("E",0);
                    fUI->setParamValue("F",0);
                    fUI->setParamValue("G",0);

                    if (indexNum <= 350 && indexNum >= 320) {
                        //DBG("A");
                        playedNote = "A";
                        fUI->setParamValue("A",1);
                    }

                    if (indexNum <= 320) {
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

                    if (middleNum <= 370 && middleNum >= 340) {
                        //DBG("C");
                        playedNote = "C";
                        fUI->setParamValue("C",1);
                    }
                    if (middleNum <= 340) {
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

                    if (ringNum <= 420 && ringNum >=380) {
                        //DBG("E");
                        playedNote = "E";
                        fUI->setParamValue("E",1);
                    }
                    if (ringNum <= 380) {
                        //DBG("F");
                        playedNote = "F";
                        fUI->setParamValue("F", 1);
                    }

                    //PINKY FINGER
                    pinky = s.fromLastOccurrenceOf(",", false, true);
                    //DBG(pinky);
                    pinkyNum=pinky.getFloatValue();

                    if (pinkyNum <= 430) {
                        //DBG("G");
                        playedNote = "G";
                        fUI->setParamValue("G", 1);
                    }

                    
                    if (pageNum == 6) {
                        fUI->setParamValue("rel", 4);
                    }

                    else {
                        fUI->setParamValue("rel", 1);
                    }
                    //CONCATENATE
                    //upToFirstOccurrenceOf()
                    //DBG(playedNote);
                    
                }
            }
        }
    }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomNameAudioProcessor)
};
