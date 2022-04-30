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
    bool updateNote = false;
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
    int remainingNotesInt = 7;
    TextButton Failed;
    TextButton Suceeded;
    String stringNote; //CHANGE THIS VALUE***
    Label failedString;
    int attemptsRemaining = 3;
    Label remainingAttempts;
    String wrongNote = "c";
    Label correctnoteWas;
    String correctNote = "b";
    TextButton tryAgain;
    Label practiceScaleDescending;
    Label currentNoteDescending;
    String stringNoteDescending = "G";
    Label nextNoteDescending;
    TextButton buttonNoteDescending;
    TextButton doneScalesDescending;
    Label playScalesDescending;
    TextButton startScalesDescending;
    Label remainingNotesDescending;
    int remainingNotesIntDescending = 7;
    TextButton FailedDescending;
    TextButton SuceededDescending;
    Label failedStringDescending;
    int attemptsRemainingDescending = 3;
    Label remainingAttemptsDescending;
    String wrongNoteDescending = "G";
    Label correctnoteWasDescending;
    String correctNoteDescending = "F";
    TextButton tryAgainDescending;
    Label practiceScaleC;
    Label currentNoteC;
    String stringNoteC = "C";
    Label nextNoteC;
    TextButton buttonNoteC;
    TextButton doneScalesC;
    Label playScalesC;
    TextButton startScalesC;
    Label remainingNotesC;
    int remainingNotesIntC = 7;
    TextButton FailedC;
    TextButton SuceededC;
    Label failedStringC;
    int attemptsRemainingC = 3;
    Label remainingAttemptsC;
    String wrongNoteC = "c";
    Label correctnoteWasC;
    String correctNoteC = "b";
    TextButton tryAgainC;
    Label scalesCompleded;
    TextButton finishScale;

    //chords

    Label practiceChords;
    Label currentChord;
    String stringChord = "A-minor (A-C-E)";
    TextButton doneChords1;
    Label chordProgression;
    TextButton doneChords2;
    Label playChords;
    TextButton startChords;
    Label remainingChords;

    String A_min_ascending_notes[8] = { "a", "b", "c", "d", "e", "f", "g", "a"};

   





private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RandomNameAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomNameAudioProcessorEditor)
};
