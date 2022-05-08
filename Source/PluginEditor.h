/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
//Sort of a constructor with all the classes declared.
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <vector>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <list>

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
    String remainingNotesA = "8";
    String stringtoTrim = "Remaining Notes ";
    TextButton Failed;
    TextButton Suceeded;
    String stringNote;
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
    float passedTime=0;
    bool wait = false;
    string chordnote;

    clock_t start, end;
    int timeThreshold;
    String failed3;
    Label practiceScaleCDescending;
    Label currentNoteCDescending;
    String stringNoteCDescending = "G";
    Label nextNoteCDescending;
    TextButton buttonNoteCDescending;
    TextButton doneScalesCDescending;
    Label playScalesCDescending;
    TextButton startScalesCDescending;
    Label remainingNotesCDescending;
    int remainingNotesIntCDescending = 7;
    TextButton FailedCDescending;
    TextButton SuceededCDescending;
    Label failedStringCDescending;
    int attemptsRemainingCDescending = 3;
    Label remainingAttemptsCDescending;
    String wrongNoteCDescending = "G";
    Label correctnoteWasCDescending;
    String correctNoteCDescending = "F";
    TextButton tryAgainCDescending;
    
    //chords
    String first_note;
    String second_note;
    String third_note;
    String oldNote;

    bool newNote = false;
    int arrSize;
    int chordNoteCounter=0;
    String Triad_Chord_notes[3] = { "","","" };
    Label practiceChords;
    Label currentChord;
    String stringChord;
    TextButton doneChords1;
    Label chordProgression;
    TextButton doneChords2;
    Label playChords;
    TextButton startChords;
    Label remainingChords;
    TextButton doneChords3;
    Label chordProgression2;
    TextButton doneChords4;
    Label playChords2;
    TextButton startChords2;
    Label remainingChords2;
    TextButton doneChords5;
    TextButton nameOfChord;


    String A_min_scalenotes[8] = { "a", "b", "c", "d", "e", "f", "g", "a"};
    String C_maj_scalenotes[8] = { "c", "d", "e", "f", "g", "a", "b", "c" };

    String nextNoteArray="a";
    int arrayCounter = 0;
    int arrayCounter2 = 0;

    


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RandomNameAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RandomNameAudioProcessorEditor)
};
