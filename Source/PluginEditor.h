/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
//Sort of a constructor with all the classes declared.
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;
//==============================================================================
/**
*/
class RandomNameAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    RandomNameAudioProcessorEditor (RandomNameAudioProcessor&);
    ~RandomNameAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    Label title;
    void configGUI();
    TextButton newGame;
    TextButton howTo;
    void timerCallback();
    int frameRate = 30;
    bool isPageChanged = false;
    Label title2;
    TextButton menu;
    Label title3;
    TextButton scales;
    TextButton chords;
    Label title4;
    Label scalesTheory;
    TextButton tryScales;
    Label title5;
    Label chordsTheory;
    TextButton tryChords;
    Label practiceScale;
    Label currentNote;
    Label nextNote;
    Label Note;
    TextButton buttonNote;
    TextButton doneScales1;
    Label playScales;
    TextButton startScales;
    Label remainingNotes;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RandomNameAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomNameAudioProcessorEditor)
};
