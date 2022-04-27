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
using namespace juce;
using namespace std;

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

private:

    StringPairArray portlist;
    DebugFunction theDebugLog;
    SerialPort* pSP;
    SerialPortOutputStream* pOutputStream;
    SerialPortInputStream* pInputStream;

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
                    float indexNum = index.getFloatValue();

                    if (indexNum <= 360) {
                        DBG("INDEX TRIGGERED");
                    }

                    //MIDDLE FINGER
                    String temp1 = s.fromFirstOccurrenceOf(",", false, true);
                    middle = temp1.upToFirstOccurrenceOf(",", false, true);
                    //DBG(temp1);
                    //DBG(middle);
                    float middleNum = middle.getFloatValue();

                    if (middleNum <= 380) {
                        DBG("MIDDLE TRIGGERED");
                    }

                    //RING FINGER
                    String temp2 = temp1.fromFirstOccurrenceOf(",", false, true);
                    ring = temp2.upToFirstOccurrenceOf(",", false, true);
                    //DBG(temp2);
                    //DBG(ring);
                    float ringNum = ring.getFloatValue();

                    if (ringNum <= 400) {
                        DBG("RING TRIGGERED");
                    }

                    //PINKY FINGER
                    pinky = s.fromLastOccurrenceOf(",", false, true);
                    //DBG(pinky);
                    float pinkyNum=pinky.getFloatValue();

                    if (pinkyNum <= 400) {
                        DBG("PINKY TRIGGERED");
                    }
                    //CONCATENATE
                    //upToFirstOccurrenceOf()
                }
            }
        }
    }
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomNameAudioProcessor)
};
