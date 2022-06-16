/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <vector>

using namespace std;

//==============================================================================
RandomNameAudioProcessorEditor::RandomNameAudioProcessorEditor (RandomNameAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1080, 720);
    configGUI();
    startTimerHz(frameRate);
}

RandomNameAudioProcessorEditor::~RandomNameAudioProcessorEditor()
{
}
void RandomNameAudioProcessorEditor::configGUI() {
    //Parameters for the main title
    addAndMakeVisible(title);
    title.setText("P4 - Amazing digital synthesizer", dontSendNotification);
    title.setFont(40);
    //Parameters for the "Skip to lessons" button
    addAndMakeVisible(newGame);
    newGame.setButtonText("Skip to lessons");
    newGame.changeWidthToFitText();
    newGame.setColour(newGame.buttonColourId, Colours::blue);
    newGame.onClick = [this]
    {
        //When a button is clicked, pageNum switches to a new page number that then makes the corresponding page visible
        audioProcessor.pageNum = 2;
    };
    //Parameters for the "How to use and setup music glove" button
    addAndMakeVisible(howTo);
    howTo.setButtonText("Set up music glove");
    howTo.changeWidthToFitText();
    howTo.setColour(howTo.buttonColourId, Colours::green);
    howTo.onClick = [this] {
        audioProcessor.pageNum = 1;
    };
    //Parameters for the main title of music glove page
    addAndMakeVisible(title2);
    //It starts off as not visible because we only want it to be visible when pagenum==2
    title2.setVisible(false);
    title2.setText("Music glove configuration setup", dontSendNotification);
    title2.setFont(40);
    //Parameters for the "Menu" button
    addAndMakeVisible(menu);
    menu.setVisible(false);
    menu.setColour(menu.buttonColourId, Colours::red);
    menu.setButtonText("Menu");
    menu.changeWidthToFitText();
    menu.onClick = [this]
    {
        audioProcessor.pageNum = 0;
    };
    //Parameters for the learning page
    addAndMakeVisible(title3);
    title3.setVisible(false);
    title3.setText("What do you want to learn?", dontSendNotification);
    title3.setFont(40);
    //Chords button
    addAndMakeVisible(chords);
    chords.setVisible(false);
    chords.setButtonText("Chords");
    chords.changeWidthToFitText();
    chords.setColour(chords.buttonColourId, Colours::dodgerblue);
    chords.onClick = [this] {
        audioProcessor.pageNum = 3;
    };
    //Scales button
    addAndMakeVisible(scales);
    scales.setVisible(false);
    scales.setColour(scales.buttonColourId, Colours::rebeccapurple);
    scales.setButtonText("Scales ");
    scales.changeWidthToFitText();
    scales.onClick = [this] {
        audioProcessor.pageNum = 4;
    };
    addAndMakeVisible(title4);
    title4.setVisible(false);
    title4.setText("Scales Theory", dontSendNotification);
    title4.setFont(40);
    addAndMakeVisible(scalesTheory);
    scalesTheory.setVisible(false);
    scalesTheory.setText("To learn what a scale is, it is first needed to know what a note is. When you press a key on a piano a sound is played, that sound is called a note. Similarly when bending a finger with your musicglove - depending on which finger you are bending and the amount you are bending it - a specific note will be played. The notes are named after the 7 first letters of the alphabet A,B,C,D,E,F,G. A scale is a group of consecutive notes. In this lesson we are going to have to confine ourselves to the two most - frequently used scales : the major and the minor scale.", dontSendNotification);
    scalesTheory.setFont(20);
    addAndMakeVisible(scalesTheory2);
    scalesTheory2.setVisible(false);
    scalesTheory2.setText("The name of the scale determines the first note in the scale. For example the C - Major scale will be as follows: C, D, E, F, G, A, B, C The scale will continue until you return to your starting note, in this example C. Scales can also be ascending or descending which decides in which direction the scale is played. If it is ascending, the note sequence will be written as 1-2-3-4-5-6-7-1 and if it's descending the note sequence will be written as 1-7-6-5-4-3-2-1.", dontSendNotification);
    scalesTheory2.setFont(20);
    addAndMakeVisible(tryScales);
    tryScales.setVisible(false);
    tryScales.setButtonText("Try it yourself!");
    tryScales.setColour(tryScales.buttonColourId, Colours::deepskyblue);
    tryScales.changeWidthToFitText();
    tryScales.onClick = [this] {
        audioProcessor.pageNum = 5;
    };
    addAndMakeVisible(title5);
    title5.setVisible(false);
    title5.setText("Chords Theory", dontSendNotification);
    title5.setFont(40);
    addAndMakeVisible(chordsTheory);
    chordsTheory.setVisible(false);
    chordsTheory.setText("A chord is, quite simply, several notes played simultaneously. In this lesson we will focus on triads which is a chord played with three notes. The root of the chord gives the name to the chord. When you know the root of the chord the two remaining notes can be decided by the 1-3-5 principle. 1 is the root, 3 is the third and 5 is the fifth of the given chord. If the root is A, the two remaining chord notes can be found by moving up the scale and skipping a note twice - the third will therefore be C (because B is skipped) and the fifth will be E (because D is skipped). To play the chords with your musicglove you will have to play one note at a time and the application will automatically sustain the notes until all three notes of the chord are playing simultaneously.", dontSendNotification);
    chordsTheory.setFont(25);
    addAndMakeVisible(tryChords);
    tryChords.setVisible(false);
    tryChords.setButtonText("Try it yourself!");
    tryChords.setColour(tryScales.buttonColourId, Colours::deepskyblue);
    tryChords.changeWidthToFitText();
    tryChords.onClick = [this] {
        audioProcessor.pageNum = 6;
    };
    addAndMakeVisible(practiceScale);
    practiceScale.setVisible(false);
    practiceScale.setText("Practice playing the notes of an A-minor scale in ascending order", dontSendNotification);
    practiceScale.setFont(40);
    addAndMakeVisible(currentNote);
    currentNote.setVisible(false);
    currentNote.setText("Currently playing ", dontSendNotification);
    currentNote.setFont(40);

    //**

    //teksten skal ændre sig alt efter hvilken note man spiller
    addAndMakeVisible(nextNote);
    nextNote.setVisible(false);
    nextNote.setText("Next note:", dontSendNotification);
    nextNote.setFont(40);
    addAndMakeVisible(buttonNote);
    buttonNote.setVisible(false);
    buttonNote.setButtonText(nextNoteArray); //DET HER SKAL ÆNDRES TIL EN VARIABLE
    buttonNote.changeWidthToFitText();
    buttonNote.setColour(chords.buttonColourId, Colours::dodgerblue);
    /*buttonNote.onClick = [this] {
        buttonNote.setButtonText("b");
    };*/


    addAndMakeVisible(doneScales1);
    doneScales1.setVisible(false);
    doneScales1.setButtonText("Done/next");
    doneScales1.setColour(doneScales1.buttonColourId, Colours::deepskyblue);
    doneScales1.changeWidthToFitText();
    doneScales1.onClick = [this] {
        audioProcessor.pageNum = 7;
    };
    addAndMakeVisible(playScales);
    playScales.setVisible(false);
    playScales.setText("Now try playing the notes of an A-minor scale in ascending order (A-B-C-D-E-F-G-A) without assistance.", dontSendNotification);
    playScales.setFont(40);
    addAndMakeVisible(startScales);
    startScales.setVisible(false);
    startScales.setButtonText("Done/next");
    startScales.setColour(doneScales1.buttonColourId, Colours::deepskyblue);
    startScales.changeWidthToFitText();
    startScales.onClick = [this] {
        audioProcessor.pageNum = 8;
    };
    addAndMakeVisible(remainingNotes);
    remainingNotes.setVisible(false);
    remainingNotes.setText(stringtoTrim + remainingNotesA, dontSendNotification);
    remainingNotes.setFont(40);

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR FAILED SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(Failed);
    Failed.setVisible(false);
    Failed.setButtonText("Back to intro");
    Failed.setColour(Failed.buttonColourId, Colours::deepskyblue);
    Failed.changeWidthToFitText();
    Failed.onClick = [this] {

        audioProcessor.pageNum = 5;

    };

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR remainingnotes når 0 SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(Suceeded);
    Suceeded.setVisible(false);
    Suceeded.setButtonText("Suceeded");
    Suceeded.setColour(Suceeded.buttonColourId, Colours::deepskyblue);
    Suceeded.changeWidthToFitText();
    Suceeded.onClick = [this] {
        audioProcessor.pageNum = 10;
    };

    //her skal failed string ændres til den note man spillede forkert
    addAndMakeVisible(failedString);
    //auto failed = String(wrongNote);
    failedString.setVisible(false);
    failedString.setText("You played a wrong note, try again! ", dontSendNotification);
    failedString.setFont(40);

    //her skal failed 2 ændres til den note der havde været rigtig at spille eller bare fjernes hvis det er for besværligt
    addAndMakeVisible(correctnoteWas);
    //auto failed2 = String(correctNote);
    correctnoteWas.setVisible(false);
    correctnoteWas.setText("Remember that the root is A and the sequence is 1-2-3-4-5-6-7-1", dontSendNotification);
    correctnoteWas.setFont(40);

    //failed 3 skal gå én ned når man fäiler
    addAndMakeVisible(remainingAttempts);
    failed3 = to_string(attemptsRemaining);
    remainingAttempts.setVisible(false);
    remainingAttempts.setText(failed3 + " attempts remaining", dontSendNotification);
    remainingAttempts.setFont(40);

    addAndMakeVisible(tryAgain);
    tryAgain.setVisible(false);
    tryAgain.setButtonText("Try again!");
    tryAgain.setColour(tryAgain.buttonColourId, Colours::deepskyblue);
    tryAgain.changeWidthToFitText();
    tryAgain.onClick = [this] {
        audioProcessor.pageNum = 8;
    };

    addAndMakeVisible(practiceScaleDescending);
    practiceScaleDescending.setVisible(false);
    practiceScaleDescending.setText("Good job. Now practice playing the notes of an A-minor scale in DESCENDING order", dontSendNotification);
    practiceScaleDescending.setFont(40);

    addAndMakeVisible(currentNoteDescending);
    currentNoteDescending.setVisible(false);
    //stringNote skal ændre sig alt efter hvilken note der bliver spillet 
    currentNoteDescending.setText("Currently playing " + stringNoteDescending, dontSendNotification);
    currentNoteDescending.setFont(40);

    addAndMakeVisible(nextNoteDescending);
    nextNoteDescending.setVisible(false);
    nextNoteDescending.setText("Next note:", dontSendNotification);
    nextNoteDescending.setFont(40);

    addAndMakeVisible(buttonNoteDescending);
    buttonNoteDescending.setVisible(false);
    buttonNoteDescending.setButtonText("G");
    buttonNoteDescending.changeWidthToFitText();
    buttonNoteDescending.setColour(chords.buttonColourId, Colours::dodgerblue);
    buttonNoteDescending.onClick = [this] {
        buttonNoteDescending.setButtonText("F");
    };

    addAndMakeVisible(doneScalesDescending);
    doneScalesDescending.setVisible(false);
    doneScalesDescending.setButtonText("Done/next");
    doneScalesDescending.setColour(doneScalesDescending.buttonColourId, Colours::deepskyblue);
    doneScalesDescending.changeWidthToFitText();
    doneScalesDescending.onClick = [this] {
        audioProcessor.pageNum = 11;
    };

    addAndMakeVisible(playScalesDescending);
    playScalesDescending.setVisible(false);
    playScalesDescending.setText("Now try playing the notes of an A-minor scale in DESCENDING order (A-G-F-E-D-C-B-A) without assistance.", dontSendNotification);
    playScalesDescending.setFont(40);


    addAndMakeVisible(startScalesDescending);
    startScalesDescending.setVisible(false);
    startScalesDescending.setButtonText("Done/next");
    startScalesDescending.setColour(startScalesDescending.buttonColourId, Colours::deepskyblue);
    startScalesDescending.changeWidthToFitText();
    startScalesDescending.onClick = [this] {
        audioProcessor.pageNum = 12;
    };

    addAndMakeVisible(remainingNotesDescending);
    remainingNotesDescending.setVisible(false);
    remainingNotesDescending.setText(stringtoTrim + remainingNotesA, dontSendNotification);
    remainingNotesDescending.setFont(40);

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR FAILED SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(FailedDescending);
    FailedDescending.setVisible(false);
    FailedDescending.setButtonText("Back to intro");
    FailedDescending.setColour(FailedDescending.buttonColourId, Colours::deepskyblue);
    FailedDescending.changeWidthToFitText();
    FailedDescending.onClick = [this] {

        audioProcessor.pageNum = 10;

    };

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR remainingnotes når 0 SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(SuceededDescending);
    SuceededDescending.setVisible(false);
    SuceededDescending.setButtonText("Suceeded");
    SuceededDescending.setColour(Suceeded.buttonColourId, Colours::deepskyblue);
    SuceededDescending.changeWidthToFitText();
    SuceededDescending.onClick = [this] {
        audioProcessor.pageNum = 14;
    };

    //her skal failed string ændres til den note man spillede forkert
    addAndMakeVisible(failedStringDescending);
    //auto failedDescending = String(wrongNoteDescending);
    failedStringDescending.setVisible(false);
    failedStringDescending.setText("You played a wrong note, try again! ", dontSendNotification);
    failedStringDescending.setFont(40);

    //her skal failed 2 ændres til den note der havde været rigtig at spille eller bare fjernes hvis det er for besværligt
    addAndMakeVisible(correctnoteWasDescending);
    //auto failed2Descending = String(correctNoteDescending);
    correctnoteWasDescending.setVisible(false);
    correctnoteWasDescending.setText("Remember that the root is A and the sequence is 1-7-6-5-4-3-2-1", dontSendNotification);
    correctnoteWasDescending.setFont(40);

    //failed 3 skal gå én ned når man fäiler
    addAndMakeVisible(remainingAttemptsDescending);
    failed3 = to_string(attemptsRemaining);
    remainingAttemptsDescending.setVisible(false);
    remainingAttemptsDescending.setText(failed3 + " attempts remaining", dontSendNotification);
    remainingAttemptsDescending.setFont(40);

    addAndMakeVisible(tryAgainDescending);
    tryAgainDescending.setVisible(false);
    tryAgainDescending.setButtonText("Try again!");
    tryAgainDescending.setColour(tryAgain.buttonColourId, Colours::deepskyblue);
    tryAgainDescending.changeWidthToFitText();
    tryAgainDescending.onClick = [this] {
        audioProcessor.pageNum = 12;
    };

    addAndMakeVisible(practiceScaleC);
    practiceScaleC.setVisible(false);
    practiceScaleC.setText("Good job. Now practice playing the notes of a C-MAJOR scale in ASCENDING order", dontSendNotification);
    practiceScaleC.setFont(40);

    addAndMakeVisible(currentNoteC);
    currentNoteC.setVisible(false);
    //stringNote skal ændre sig alt efter hvilken note der bliver spillet 
    currentNoteC.setText("Currently playing " + stringNoteC, dontSendNotification);
    currentNoteC.setFont(40);

    addAndMakeVisible(nextNoteC);
    nextNoteC.setVisible(false);
    nextNoteC.setText("Next note:", dontSendNotification);
    nextNoteC.setFont(40);

    addAndMakeVisible(buttonNoteC);
    buttonNoteC.setVisible(false);
    buttonNoteC.setButtonText("c");
    buttonNoteC.changeWidthToFitText();
    buttonNoteC.setColour(chords.buttonColourId, Colours::dodgerblue);
    buttonNoteC.onClick = [this] {
        buttonNoteC.setButtonText("d");
    };

    addAndMakeVisible(doneScalesC);
    doneScalesC.setVisible(false);
    doneScalesC.setButtonText("Done");
    doneScalesC.setColour(doneScalesC.buttonColourId, Colours::deepskyblue);
    doneScalesC.changeWidthToFitText();
    doneScalesC.onClick = [this] {
        audioProcessor.pageNum = 15;
    };

    addAndMakeVisible(playScalesC);
    playScalesC.setVisible(false);
    playScalesC.setText("Now try playing the notes of a C-major scale in ASCENDING order (C-D-E-F-G-A-B-C) without assistance.", dontSendNotification);
    playScalesC.setFont(40);


    addAndMakeVisible(startScalesC);
    startScalesC.setVisible(false);
    startScalesC.setButtonText("Done/next");
    startScalesC.setColour(startScalesC.buttonColourId, Colours::deepskyblue);
    startScalesC.changeWidthToFitText();
    startScalesC.onClick = [this] {
        audioProcessor.pageNum = 16;
    };

    addAndMakeVisible(remainingNotesC);
    auto strC = String(remainingNotesIntC);
    remainingNotesC.setVisible(false);
    remainingNotesC.setText("Remaining notes "+remainingNotesC_str, dontSendNotification);
    remainingNotesC.setFont(40);

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR FAILED SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(FailedC);
    FailedC.setVisible(false);
    FailedC.setButtonText("Back to intro");
    FailedC.setColour(FailedC.buttonColourId, Colours::deepskyblue);
    FailedC.changeWidthToFitText();
    FailedC.onClick = [this] {

        audioProcessor.pageNum = 14;

    };

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR remainingnotes når 0 SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(SuceededC);
    SuceededC.setVisible(false);
    SuceededC.setButtonText("Suceeded");
    SuceededC.setColour(SuceededC.buttonColourId, Colours::deepskyblue);
    SuceededC.changeWidthToFitText();
    SuceededC.onClick = [this] {
        audioProcessor.pageNum = 25;
    };




    //her skal failed string ændres til den note man spillede forkert
    addAndMakeVisible(failedStringC);
    auto failedC = String(wrongNoteC);
    failedStringC.setVisible(false);
    failedStringC.setText("You played a wrong note try again!", dontSendNotification);
    failedStringC.setFont(40);

    //her skal failed 2 ændres til den note der havde været rigtig at spille eller bare fjernes hvis det er for besværligt
    addAndMakeVisible(correctnoteWasC);
    auto failed2C = String(correctNoteC);
    correctnoteWasC.setVisible(false);
    correctnoteWasC.setText("Remember that C is the root and note sequence is 1-2-3-4-5-6-7-1", dontSendNotification);
    correctnoteWasC.setFont(40);

    //failed 3 skal gå én ned når man fäiler
    addAndMakeVisible(remainingAttemptsC);
    auto failed3C = String(attemptsRemainingC);
    remainingAttemptsC.setVisible(false);
    remainingAttemptsC.setText(failed3C + " attempts remaining", dontSendNotification);
    remainingAttemptsC.setFont(40);

    addAndMakeVisible(tryAgainC);
    tryAgainC.setVisible(false);
    tryAgainC.setButtonText("Try again!");
    tryAgainC.setColour(tryAgainC.buttonColourId, Colours::deepskyblue);
    tryAgainC.changeWidthToFitText();
    tryAgainC.onClick = [this] {
        audioProcessor.pageNum = 16;
    };

    addAndMakeVisible(scalesCompleded);
    scalesCompleded.setVisible(false);
    scalesCompleded.setText("Congratulations you finished the SCALES lessons", dontSendNotification);
    scalesCompleded.setFont(40);


    addAndMakeVisible(finishScale);
    finishScale.setVisible(false);
    finishScale.setButtonText("Return to lessons");
    finishScale.setColour(startScalesC.buttonColourId, Colours::deepskyblue);
    finishScale.changeWidthToFitText();
    finishScale.onClick = [this] {
        audioProcessor.pageNum = 2;
    };

    //her begynder chords delen 
    addAndMakeVisible(practiceChords);
    practiceChords.setVisible(false);
    practiceChords.setText("Practice playing different chords. Try moving different fingers and play different notes simultaneously. Names of the valid chords you play will be shown below ", dontSendNotification);
    practiceChords.setFont(40);
    addAndMakeVisible(currentChord);
    currentChord.setVisible(false);
    //stringChord skal ændre sig alt efter hvilken Chord der bliver spillet 
    currentChord.setText("Currently playing: " + stringChord, dontSendNotification);
    currentChord.setFont(40);

    addAndMakeVisible(doneChords1);
    doneChords1.setVisible(false);
    doneChords1.setButtonText("Done/next");
    doneChords1.setColour(doneScales1.buttonColourId, Colours::deepskyblue);
    doneChords1.changeWidthToFitText();
    doneChords1.onClick = [this] {
        audioProcessor.pageNum = 19;
    };
    addAndMakeVisible(chordProgression);
    chordProgression.setVisible(false);
    chordProgression.setText("A typical chord progression is the major I-IV-V-I progression: In the key of C that will be C-major, F-major and G-major followed by C-major again.                                                                   Try playing these four chords in order on the next page:                             C-major: C + E + G                                  F-major: F + A + C                                 G-major: G + B + D                                 C-major: C + E + G", dontSendNotification);
    chordProgression.setFont(40);

    addAndMakeVisible(doneChords2);
    doneChords2.setVisible(false);
    doneChords2.setButtonText("Done/next");
    doneChords2.setColour(doneScales1.buttonColourId, Colours::deepskyblue);
    doneChords2.changeWidthToFitText();
    doneChords2.onClick = [this] {
        audioProcessor.pageNum = 21;
    };

    addAndMakeVisible(playChords);
    playChords.setVisible(false);
    playChords.setText("Try playing these chords in the correct order: C-major, F-major and a G-major ", dontSendNotification);
    playChords.setFont(40);
    addAndMakeVisible(startChords);
    startChords.setVisible(false);
    startChords.setButtonText("Done/next");
    startChords.setColour(doneScales1.buttonColourId, Colours::deepskyblue);
    startChords.changeWidthToFitText();
    startChords.onClick = [this] {
        audioProcessor.pageNum = 21;
    };

    addAndMakeVisible(remainingChords);
    remainingChords.setVisible(false);
    remainingChords.setText("Remaining Chords: ", dontSendNotification);
    remainingChords.setFont(40);

    //skal nok ikke være en knap men bare gå direkte videre når alle tre chords er blevet spillet
    addAndMakeVisible(doneChords3);
    doneChords3.setVisible(false);
    doneChords3.setButtonText("Done/next");
    doneChords3.setColour(doneChords3.buttonColourId, Colours::deepskyblue);
    doneChords3.changeWidthToFitText();
    doneChords3.onClick = [this] {
        audioProcessor.pageNum = 22;
    };

    addAndMakeVisible(chordProgression2);
    chordProgression2.setVisible(false);
    chordProgression2.setText("Another typical chord progression is the minor I-IV-V-I progression: In the key of Am that will be A-minor, D-minor and E-minor followed by A-minor again.                                                                     Try playing these four chords in order on the next page:                               A-minor: A + C + E                                  D-minor: D + F + A                                E-minor: E + G + B                               A-minor: A + C + E", dontSendNotification);
    chordProgression2.setFont(40);

    addAndMakeVisible(doneChords4);
    doneChords4.setVisible(false);
    doneChords4.setButtonText("Done/next");
    doneChords4.setColour(doneChords4.buttonColourId, Colours::deepskyblue);
    doneChords4.changeWidthToFitText();
    doneChords4.onClick = [this] {
        audioProcessor.pageNum = 24;
    };

    addAndMakeVisible(playChords2);
    playChords2.setVisible(false);
    playChords2.setText("Try playing these chords in the correct order:  A-minor, D-minor and a E-minor", dontSendNotification);
    playChords2.setFont(40);
    addAndMakeVisible(startChords2);
    startChords2.setVisible(false);
    startChords2.setButtonText("Done/next");
    startChords2.setColour(doneScales1.buttonColourId, Colours::deepskyblue);
    startChords2.changeWidthToFitText();
    startChords2.onClick = [this] {
        audioProcessor.pageNum = 24;
    };

    addAndMakeVisible(remainingChords2);
    remainingChords2.setVisible(false);
    remainingChords2.setText("Remaining Chords:                    A MINOR - D MINOR - E MINOR", dontSendNotification);
    remainingChords2.setFont(40);

    //skal nok ikke være en knap men bare gå direkte videre når alle tre chords er blevet spillet
    addAndMakeVisible(doneChords5);
    doneChords5.setVisible(false);
    doneChords5.setButtonText("Done/next");
    doneChords5.setColour(doneChords5.buttonColourId, Colours::deepskyblue);
    doneChords5.changeWidthToFitText();
    doneChords5.onClick = [this] {
        audioProcessor.pageNum = 25;
    };

    addAndMakeVisible(practiceScaleCDescending);
    practiceScaleCDescending.setVisible(false);
    practiceScaleCDescending.setText("Good job. Now practice playing the notes of a C-MAJOR scale in DESCENDING order", dontSendNotification);
    practiceScaleCDescending.setFont(40);

    addAndMakeVisible(currentNoteCDescending);
    currentNoteCDescending.setVisible(false);
    //stringNote skal ændre sig alt efter hvilken note der bliver spillet 
    currentNoteCDescending.setText("Currently playing " + stringNoteCDescending, dontSendNotification);
    currentNoteCDescending.setFont(40);

    addAndMakeVisible(nextNoteCDescending);
    nextNoteCDescending.setVisible(false);
    nextNoteCDescending.setText("Next note:", dontSendNotification);
    nextNoteCDescending.setFont(40);

    addAndMakeVisible(buttonNoteCDescending);
    buttonNoteCDescending.setVisible(false);
    buttonNoteCDescending.setButtonText("G");
    buttonNoteCDescending.changeWidthToFitText();
    buttonNoteCDescending.setColour(chords.buttonColourId, Colours::dodgerblue);
    buttonNoteCDescending.onClick = [this] {
        buttonNoteCDescending.setButtonText("F");
    };

    addAndMakeVisible(doneScalesCDescending);
    doneScalesCDescending.setVisible(false);
    doneScalesCDescending.setButtonText("Done/next");
    doneScalesCDescending.setColour(doneScalesCDescending.buttonColourId, Colours::deepskyblue);
    doneScalesCDescending.changeWidthToFitText();
    doneScalesCDescending.onClick = [this] {
        audioProcessor.pageNum = 26;
    };

    addAndMakeVisible(playScalesCDescending);
    playScalesCDescending.setVisible(false);
    playScalesCDescending.setText("Now try playing the notes of a C-MAJOR scale in DESCENDING order (C-B-A-G-F-E-D-C) without assistance.", dontSendNotification);
    playScalesCDescending.setFont(40);


    addAndMakeVisible(startScalesCDescending);
    startScalesCDescending.setVisible(false);
    startScalesCDescending.setButtonText("Done/next");
    startScalesCDescending.setColour(startScalesCDescending.buttonColourId, Colours::deepskyblue);
    startScalesCDescending.changeWidthToFitText();
    startScalesCDescending.onClick = [this] {
        audioProcessor.pageNum = 27;
    };

    addAndMakeVisible(remainingNotesCDescending);
    remainingNotesCDescending.setVisible(false);
    remainingNotesCDescending.setText("Remaining notes 8", dontSendNotification);
    remainingNotesCDescending.setFont(40);

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR FAILED SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(FailedCDescending);
    FailedCDescending.setVisible(false);
    FailedCDescending.setButtonText("Back to intro");
    FailedCDescending.setColour(FailedCDescending.buttonColourId, Colours::deepskyblue);
    FailedCDescending.changeWidthToFitText();
    FailedCDescending.onClick = [this] {

        audioProcessor.pageNum = 25;

    };

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR remainingnotes når 0 SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(SuceededCDescending);
    SuceededCDescending.setVisible(false);
    SuceededCDescending.setButtonText("Suceeded");
    SuceededCDescending.setColour(Suceeded.buttonColourId, Colours::deepskyblue);
    SuceededCDescending.changeWidthToFitText();
    SuceededCDescending.onClick = [this] {
        audioProcessor.pageNum = 18;
    };

    //her skal failed string ændres til den note man spillede forkert
    addAndMakeVisible(failedStringCDescending);
    auto failedCDescending = String(wrongNoteCDescending);
    failedStringCDescending.setVisible(false);
    failedStringCDescending.setText("You played a wrong note, try again!", dontSendNotification);
    failedStringCDescending.setFont(40);

    //her skal failed 2 ændres til den note der havde været rigtig at spille eller bare fjernes hvis det er for besværligt
    addAndMakeVisible(correctnoteWasCDescending);
    auto failed2CDescending = String(correctNoteCDescending);
    correctnoteWasCDescending.setVisible(false);
    correctnoteWasCDescending.setText("Remember that the root is C and the sequence is 1-7-6-5-4-3-2-1", dontSendNotification);
    correctnoteWasCDescending.setFont(40);

    //failed 3 skal gå én ned når man fäiler
    addAndMakeVisible(remainingAttemptsCDescending);
    auto failed3CDescending = String(attemptsRemainingCDescending);
    remainingAttemptsCDescending.setVisible(false);
    remainingAttemptsCDescending.setText(failed3CDescending + " attempts remaining", dontSendNotification);
    remainingAttemptsCDescending.setFont(40);

    addAndMakeVisible(tryAgainCDescending);
    tryAgainCDescending.setVisible(false);
    tryAgainCDescending.setButtonText("Try again!");
    tryAgainCDescending.setColour(tryAgain.buttonColourId, Colours::deepskyblue);
    tryAgainCDescending.changeWidthToFitText();
    tryAgainCDescending.onClick = [this] {
        audioProcessor.pageNum = 27;
    };

    addAndMakeVisible(nameOfChord);
    nameOfChord.setVisible(false);
    nameOfChord.setButtonText("*");
    nameOfChord.setColour(nameOfChord.buttonColourId, Colours::green);
    nameOfChord.changeWidthToFitText();

    addAndMakeVisible(trueTestcurrentChord);
    trueTestcurrentChord.setVisible(false);
    trueTestcurrentChord.setText("Currently playing: " + stringChord, dontSendNotification);
    trueTestcurrentChord.setFont(40);

    addAndMakeVisible(firstChord);
    firstChord.setVisible(false);
    firstChord.setText("C MAJOR - ", dontSendNotification);
    firstChord.setFont(40);

    addAndMakeVisible(secondChord);
    secondChord.setVisible(false);
    secondChord.setText("F MAJOR - ", dontSendNotification);
    secondChord.setFont(40);

    addAndMakeVisible(thirdChord);
    thirdChord.setVisible(false);
    thirdChord.setText("G MAJOR - ", dontSendNotification);
    thirdChord.setFont(40);

    addAndMakeVisible(fourthChord);
    fourthChord.setVisible(false);
    fourthChord.setText("C MAJOR", dontSendNotification);
    fourthChord.setFont(40);

    addAndMakeVisible(failedChord);
    failedChord.setVisible(false);
    failedChord.setText("You played a wrong chord, try again! ", dontSendNotification);
    failedChord.setFont(40);
    //failedChord.setColour(failedChord.textColourId, Colours::red);

    addAndMakeVisible(attemptsRemainingChords);
    attemptsRemainingChords.setVisible(false);
    attemptsRemainingChords.setText(failed3+" attempt(s) remaining", dontSendNotification);
    attemptsRemainingChords.setFont(40);
    //attemptsRemainingChords.setColour(attemptsRemainingChords.textColourId, Colours::red);

    addAndMakeVisible(backtoChords);
    backtoChords.setVisible(false);
    backtoChords.setButtonText("Try again!");
    backtoChords.setColour(tryAgain.buttonColourId, Colours::deepskyblue);
    backtoChords.changeWidthToFitText();
    backtoChords.onClick = [this] {
        audioProcessor.pageNum = 21;
    };

    addAndMakeVisible(CompletedChords);
    CompletedChords.setVisible(false);
    CompletedChords.setText("GOOD JOB! You may continue to the next page", dontSendNotification);
    CompletedChords.setColour(CompletedChords.textColourId, Colours::lightgreen);
    CompletedChords.setFont(40);

    addAndMakeVisible(backtoChords2);
    backtoChords2.setVisible(false);
    backtoChords2.setButtonText("Try again!");
    backtoChords2.setColour(tryAgain.buttonColourId, Colours::deepskyblue);
    backtoChords2.changeWidthToFitText();
    backtoChords2.onClick = [this] {
        audioProcessor.pageNum = 24;
    };

    addAndMakeVisible(goToWinChords);
    goToWinChords.setVisible(false);
    goToWinChords.setButtonText("Next page");
    goToWinChords.setColour(tryAgain.buttonColourId, Colours::deepskyblue);
    goToWinChords.changeWidthToFitText();
    goToWinChords.onClick = [this] {
        audioProcessor.pageNum = 31;
    };

    addAndMakeVisible(FinishedChordsLesson);
    FinishedChordsLesson.setVisible(false);
    FinishedChordsLesson.setText("Congratulations you finished the CHORDS lesson", dontSendNotification);
    FinishedChordsLesson.setColour(FinishedChordsLesson.textColourId, Colours::white);
    FinishedChordsLesson.setFont(40);
    
    addAndMakeVisible(resetChord);
    resetChord.setVisible(false);
    resetChord.setButtonText("Reset chord");
    resetChord.setColour(tryAgain.buttonColourId, Colours::red);
    resetChord.changeWidthToFitText();
    resetChord.onClick = [this] {
        chordNoteCounter = 0;
        Triad_Chord_notes[0] = "";
        Triad_Chord_notes[1] = "";
        Triad_Chord_notes[2] = "";
        trueTestcurrentChord.setText("Currently playing:  - - ", dontSendNotification);
    };

    addAndMakeVisible(backtoChordsIntro);
    backtoChordsIntro.setVisible(false);
    backtoChordsIntro.setButtonText("Back to intro");
    backtoChordsIntro.setColour(backtoChordsIntro.buttonColourId, Colours::deepskyblue);
    backtoChordsIntro.changeWidthToFitText();
    backtoChordsIntro.onClick = [this] {

        audioProcessor.pageNum = 6;

    };

    addAndMakeVisible(backtoChordsIntro2);
    backtoChordsIntro2.setVisible(false);
    backtoChordsIntro2.setButtonText("Back to intro");
    backtoChordsIntro2.setColour(backtoChordsIntro.buttonColourId, Colours::deepskyblue);
    backtoChordsIntro2.changeWidthToFitText();
    backtoChordsIntro2.onClick = [this] {

        audioProcessor.pageNum = 20;

    };

    addAndMakeVisible(to_A);
    to_A.setVisible(false);
    to_A.setButtonText("Done/next");
    to_A.setColour(backtoChordsIntro.buttonColourId, Colours::deepskyblue);
    to_A.changeWidthToFitText();
    to_A.onClick = [this] {

        audioProcessor.pageNum = 22;

    };

    addAndMakeVisible(ChordTheoryFail);
    ChordTheoryFail.setVisible(false);
    ChordTheoryFail.setText("Remember the 1-3-5 principle: a chord contains a ROOT (letter in chord name) THIRD (two notes up from root) and FIFTH (two notes up from third)", dontSendNotification);
    ChordTheoryFail.setColour(ChordTheoryFail.textColourId, Colours::white);
    ChordTheoryFail.setFont(30);

    addAndMakeVisible(audioProcessor.IndexUP);
    audioProcessor.IndexUP.setVisible(false);
    audioProcessor.IndexUP.setSliderStyle(Slider::SliderStyle::LinearBar);
    audioProcessor.IndexUP.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    audioProcessor.IndexUP.setRange(0.0f, 400.0f, 1.0f);
    audioProcessor.IndexUP.setValue(270);
    audioProcessor.IndexUP.onValueChange = [this] {
        audioProcessor.indexUpper = audioProcessor.IndexUP.getValue();
    };

    addAndMakeVisible(sliderA);
    sliderA.setVisible(false);
    sliderA.setText("Slider for A threshold", dontSendNotification);
    sliderA.setColour(ChordTheoryFail.textColourId, Colours::white);
    sliderA.setFont(20);
    sliderA.attachToComponent(&audioProcessor.IndexUP, false);

    addAndMakeVisible(audioProcessor.IndexLOW);
    audioProcessor.IndexLOW.setVisible(false);
    audioProcessor.IndexLOW.setSliderStyle(Slider::SliderStyle::LinearBar);
    audioProcessor.IndexLOW.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    audioProcessor.IndexLOW.setRange(0.0f, 400.0f, 1.0f);
    audioProcessor.IndexLOW.setValue(230);
    audioProcessor.IndexLOW.onValueChange = [this] {
        audioProcessor.indexLower = audioProcessor.IndexLOW.getValue();
    };

    addAndMakeVisible(sliderB);
    sliderB.setVisible(false);
    sliderB.setText("Slider for B threshold", dontSendNotification);
    sliderB.setColour(ChordTheoryFail.textColourId, Colours::white);
    sliderB.setFont(20);
    sliderB.attachToComponent(&audioProcessor.IndexLOW, false);

    addAndMakeVisible(audioProcessor.MiddleUP);
    audioProcessor.MiddleUP.setVisible(false);
    audioProcessor.MiddleUP.setSliderStyle(Slider::SliderStyle::LinearBar);
    audioProcessor.MiddleUP.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    audioProcessor.MiddleUP.setRange(0.0f, 500.0f, 1.0f);
    audioProcessor.MiddleUP.setValue(360);
    audioProcessor.MiddleUP.onValueChange = [this] {
        audioProcessor.middleUpper = audioProcessor.MiddleUP.getValue();
    };

    addAndMakeVisible(sliderC);
    sliderC.setVisible(false);
    sliderC.setText("Slider for C threshold", dontSendNotification);
    sliderC.setColour(ChordTheoryFail.textColourId, Colours::white);
    sliderC.setFont(20);
    sliderC.attachToComponent(&audioProcessor.MiddleUP, false);

    addAndMakeVisible(audioProcessor.MiddleLOW);
    audioProcessor.MiddleLOW.setVisible(false);
    audioProcessor.MiddleLOW.setSliderStyle(Slider::SliderStyle::LinearBar);
    audioProcessor.MiddleLOW.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    audioProcessor.MiddleLOW.setRange(0.0f, 500.0f, 1.0f);
    audioProcessor.MiddleLOW.setValue(325);
    audioProcessor.MiddleLOW.onValueChange = [this] {
        audioProcessor.middleLower = audioProcessor.MiddleLOW.getValue();
    };

    addAndMakeVisible(sliderD);
    sliderD.setVisible(false);
    sliderD.setText("Slider for D threshold", dontSendNotification);
    sliderD.setColour(ChordTheoryFail.textColourId, Colours::white);
    sliderD.setFont(20);
    sliderD.attachToComponent(&audioProcessor.MiddleLOW, false);

    addAndMakeVisible(audioProcessor.RingUP);
    audioProcessor.RingUP.setVisible(false);
    audioProcessor.RingUP.setSliderStyle(Slider::SliderStyle::LinearBar);
    audioProcessor.RingUP.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    audioProcessor.RingUP.setRange(0.0f, 600.0f, 1.0f);
    audioProcessor.RingUP.setValue(420);
    audioProcessor.RingUP.onValueChange = [this] {
        audioProcessor.ringUpper = audioProcessor.RingUP.getValue();
    };

    addAndMakeVisible(sliderE);
    sliderE.setVisible(false);
    sliderE.setText("Slider for E threshold", dontSendNotification);
    sliderE.setColour(ChordTheoryFail.textColourId, Colours::white);
    sliderE.setFont(20);
    sliderE.attachToComponent(&audioProcessor.RingUP, false);

    addAndMakeVisible(audioProcessor.RingLOW);
    audioProcessor.RingLOW.setVisible(false);
    audioProcessor.RingLOW.setSliderStyle(Slider::SliderStyle::LinearBar);
    audioProcessor.RingLOW.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    audioProcessor.RingLOW.setRange(0.0f, 600.0f, 1.0f);
    audioProcessor.RingLOW.setValue(360);
    audioProcessor.RingLOW.onValueChange = [this] {
        audioProcessor.ringLower = audioProcessor.RingLOW.getValue();
    };

    addAndMakeVisible(sliderF);
    sliderF.setVisible(false);
    sliderF.setText("Slider for F threshold", dontSendNotification);
    sliderF.setColour(ChordTheoryFail.textColourId, Colours::white);
    sliderF.setFont(20);
    sliderF.attachToComponent(&audioProcessor.RingLOW, false);

    addAndMakeVisible(audioProcessor.PinkyMID);
    audioProcessor.PinkyMID.setVisible(false);
    audioProcessor.PinkyMID.setSliderStyle(Slider::SliderStyle::LinearBar);
    audioProcessor.PinkyMID.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    audioProcessor.PinkyMID.setRange(0.0f, 600.0f, 1.0f);
    audioProcessor.PinkyMID.setValue(400);
    audioProcessor.PinkyMID.onValueChange = [this] {
        audioProcessor.pinkyMid = audioProcessor.PinkyMID.getValue();
    };

    addAndMakeVisible(sliderG);
    sliderG.setVisible(false);
    sliderG.setText("Slider for G threshold", dontSendNotification);
    sliderG.setColour(ChordTheoryFail.textColourId, Colours::white);
    sliderG.setFont(20);
    sliderG.attachToComponent(&audioProcessor.PinkyMID, false);

    addAndMakeVisible(analogValues);
    analogValues.setVisible(false);
    analogValues.setText("Index: 300 | Middle: 400 | Ring: 450 | Pinky: 400", dontSendNotification);
    analogValues.setColour(CompletedChords.textColourId, Colours::lightgreen);
    analogValues.setFont(40);

    addAndMakeVisible(configPlayedNote);
    configPlayedNote.setVisible(false);
    configPlayedNote.setText("A", dontSendNotification);
    configPlayedNote.setColour(configPlayedNote.textColourId, Colours::green);
    configPlayedNote.setFont(80);
}

void RandomNameAudioProcessorEditor::timerCallback()
//This is where all the UI callback happens
{
    failed3 = to_string(attemptsRemaining);

    //Below are all the complete fail pages. They will give you red alarming text and change the buttons
    //so that they send you to the next page instead of back to the exercise page.
    if (attemptsRemaining == 0 && audioProcessor.pageNum_OLD==8) {
        failedString.setColour(failedString.textColourId, Colours::red);
        failedString.setText("YOU HAVE FAILED THE EXERCISE", dontSendNotification);
        correctnoteWas.setColour(correctnoteWas.textColourId, Colours::red);
        correctnoteWas.setText("Continue to the next page", dontSendNotification);
        remainingAttempts.setVisible(false);
        tryAgain.setButtonText("Next page");
            tryAgain.onClick = [this] {
                audioProcessor.pageNum = 10;
            };
    }

    if (attemptsRemaining == 0 && audioProcessor.pageNum_OLD==12) {
       
        failedStringDescending.setColour(failedString.textColourId, Colours::red);
        failedStringDescending.setText("YOU HAVE FAILED THE EXERCISE", dontSendNotification);
        correctnoteWasDescending.setColour(correctnoteWas.textColourId, Colours::red);
        correctnoteWasDescending.setText("Continue to the next page", dontSendNotification);
        remainingAttemptsDescending.setVisible(false);
        tryAgainDescending.setButtonText("Next page");
        tryAgainDescending.onClick = [this] {
            audioProcessor.pageNum = 14;
        };
    }

    if (attemptsRemaining == 0 && audioProcessor.pageNum_OLD == 16) {

        failedStringC.setColour(failedString.textColourId, Colours::red);
        failedStringC.setText("YOU HAVE FAILED THE EXERCISE", dontSendNotification);
        correctnoteWasC.setColour(correctnoteWas.textColourId, Colours::red);
        correctnoteWasC.setText("Continue to the next page", dontSendNotification);
        remainingAttemptsC.setVisible(false);
        tryAgainC.setButtonText("Next page");
        tryAgainC.onClick = [this] {
            audioProcessor.pageNum = 25;
        };
    }

    if (attemptsRemaining == 0 && audioProcessor.pageNum_OLD == 27) {

        failedStringCDescending.setColour(failedString.textColourId, Colours::red);
        failedStringCDescending.setText("YOU HAVE FAILED THE EXERCISE", dontSendNotification);
        correctnoteWasCDescending.setColour(correctnoteWas.textColourId, Colours::red);
        correctnoteWasCDescending.setText("Continue to the next page", dontSendNotification);
        remainingAttemptsCDescending.setVisible(false);
        tryAgainCDescending.setButtonText("Next page");
        tryAgainCDescending.onClick = [this] {
            audioProcessor.pageNum = 18;
        };
    }

    if (attemptsRemaining == 0 && audioProcessor.pageNum_OLD == 21) {

        failedChord.setColour(failedString.textColourId, Colours::red);
        failedChord.setText("YOU HAVE FAILED THE EXERCISE", dontSendNotification);
        attemptsRemainingChords.setColour(correctnoteWas.textColourId, Colours::red);
        attemptsRemainingChords.setText("Continue to the next page", dontSendNotification);
        backtoChords.setButtonText("Next page");
        backtoChords.onClick = [this] {
            audioProcessor.pageNum = 22;
        };
    }

    if (attemptsRemaining == 0 && audioProcessor.pageNum_OLD == 24) {

        failedChord.setColour(failedString.textColourId, Colours::red);
        failedChord.setText("YOU HAVE FAILED THE EXERCISE", dontSendNotification);
        attemptsRemainingChords.setColour(correctnoteWas.textColourId, Colours::red);
        attemptsRemainingChords.setText("Continue to the next page", dontSendNotification);
        backtoChords2.setButtonText("Next page");
        backtoChords2.onClick = [this] {
            audioProcessor.pageNum = 31;
        };
    }

    if (audioProcessor.pageNum != audioProcessor.pageNum_OLD)
    {
        isPageChanged = true;
    }
    else
        isPageChanged = false;

    //Switch for each possible page
    if (isPageChanged)
    {
        switch (audioProcessor.pageNum)
        {
        //Start Screen
        case 0:
            
            title.setVisible(true);
            newGame.setVisible(true);
            howTo.setVisible(true);

            title2.setVisible(false);
            menu.setVisible(false);
            title3.setVisible(false);
            chords.setVisible(false);
            scales.setVisible(false);
            title4.setVisible(false);
            scalesTheory.setVisible(false);
            tryScales.setVisible(false);
            title5.setVisible(false);
            tryChords.setVisible(false);
            chordsTheory.setVisible(false);
            practiceScale.setVisible(false);
            currentNote.setVisible(false);
            nextNote.setVisible(false);
            buttonNote.setVisible(false);
            doneScales1.setVisible(false);
            startScales.setVisible(false);
            playScales.setVisible(false);
            remainingNotes.setVisible(false);
            Suceeded.setVisible(false);
            Failed.setVisible(false);
            failedString.setVisible(false);
            correctnoteWas.setVisible(false);
            remainingAttempts.setVisible(false);
            tryAgain.setVisible(false);

            practiceScaleDescending.setVisible(false);
            currentNoteDescending.setVisible(false);
            nextNoteDescending.setVisible(false);
            buttonNoteDescending.setVisible(false);
            doneScalesDescending.setVisible(false);
            startScalesDescending.setVisible(false);
            playScalesDescending.setVisible(false);
            remainingNotesDescending.setVisible(false);
            FailedDescending.setVisible(false);
            SuceededDescending.setVisible(false);
            failedStringDescending.setVisible(false);
            correctnoteWasDescending.setVisible(false);
            remainingAttemptsDescending.setVisible(false);
            tryAgainDescending.setVisible(false);
            practiceScaleC.setVisible(false);
            currentNoteC.setVisible(false);
            nextNoteC.setVisible(false);
            buttonNoteC.setVisible(false);
            doneScalesC.setVisible(false);
            startScalesC.setVisible(false);
            playScalesC.setVisible(false);
            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);
            failedStringC.setVisible(false);
            correctnoteWasC.setVisible(false);
            remainingAttemptsC.setVisible(false);
            tryAgainC.setVisible(false);
            practiceChords.setVisible(false);
            currentChord.setVisible(false);
            doneChords1.setVisible(false);
            chordProgression.setVisible(false);
            doneChords2.setVisible(false);
            startChords.setVisible(false);
            playChords.setVisible(false);
            remainingChords.setVisible(false);
            doneChords3.setVisible(false);
            chordProgression2.setVisible(false);
            doneChords4.setVisible(false);
            startChords2.setVisible(false);
            playChords2.setVisible(false);
            remainingChords2.setVisible(false);
            doneChords5.setVisible(false);
            nameOfChord.setVisible(false);
            stringtoTrim = "Remaining notes ";
            remainingNotesA = "8";
            trueTestcurrentChord.setVisible(false);
            firstChord.setVisible(false);
            secondChord.setVisible(false);
            thirdChord.setVisible(false);
            fourthChord.setVisible(false);
            attemptsRemainingChords.setVisible(false);
            failedChord.setVisible(false);
            backtoChords.setVisible(false);
            backtoChords2.setVisible(false);
            CompletedChords.setVisible(false);
            scalesTheory2.setVisible(false);
            backtoChords2.setVisible(false);
            goToWinChords.setVisible(false);
            FinishedChordsLesson.setVisible(false);
            backtoChordsIntro2.setVisible(false);
            backtoChordsIntro.setVisible(false);
            to_A.setVisible(false);
            audioProcessor.IndexUP.setVisible(false);
            audioProcessor.IndexLOW.setVisible(false);
            audioProcessor.MiddleLOW.setVisible(false);
            audioProcessor.MiddleUP.setVisible(false);
            audioProcessor.RingUP.setVisible(false);
            audioProcessor.RingLOW.setVisible(false);
            audioProcessor.PinkyMID.setVisible(false);
            analogValues.setVisible(false);
            configPlayedNote.setVisible(false);
            break;

        //Configuration Page
       
        case 1:
            title2.setVisible(true);
            menu.setVisible(true);
            audioProcessor.IndexUP.setVisible(true);
            audioProcessor.IndexLOW.setVisible(true);
            audioProcessor.MiddleLOW.setVisible(true);
            audioProcessor.MiddleUP.setVisible(true);
            audioProcessor.RingUP.setVisible(true);
            audioProcessor.RingLOW.setVisible(true);
            audioProcessor.PinkyMID.setVisible(true);
            analogValues.setVisible(true);
            configPlayedNote.setVisible(true);

            title.setVisible(false);
            newGame.setVisible(false);
            howTo.setVisible(false);

            break;

        //Lessons Page
        case 2:
            menu.setVisible(true);
            title3.setVisible(true);
            chords.setVisible(true);
            scales.setVisible(true);


            title.setVisible(false);
            title2.setVisible(false);
            howTo.setVisible(false);
            newGame.setVisible(false);
            finishScale.setVisible(false);
            scalesCompleded.setVisible(false);
            FinishedChordsLesson.setVisible(false);

            break;
        
        //Chords info screen
        case 3:

            title5.setVisible(true);
            tryChords.setVisible(true);
            chordsTheory.setVisible(true);

            chords.setVisible(false);
            title3.setVisible(false);
            scales.setVisible(false);

            break;
        
        //Scales info screen
        case 4:
            title4.setVisible(true);
            scalesTheory.setVisible(true);
            scalesTheory2.setVisible(true);
            tryScales.setVisible(true);

            chords.setVisible(false);
            title3.setVisible(false);
            scales.setVisible(false);


            break;

        //A minor scale ascending intro
        case 5:
            arrayCounter = 0;
            practiceScale.setVisible(true);
            currentNote.setText("Currently playing ", dontSendNotification);
            currentNote.setVisible(true);
            
            nextNote.setVisible(true);
            nextNoteArray = A_min_scalenotes[0];
            buttonNote.setButtonText("a");
            buttonNote.setColour(chords.buttonColourId, Colours::dodgerblue);
            buttonNote.setVisible(true);
            doneScales1.setVisible(true);

            title4.setVisible(false);
            scalesTheory.setVisible(false);
            scalesTheory2.setVisible(false);
            tryScales.setVisible(false);
            remainingNotes.setVisible(false);
            Failed.setVisible(false);
            Suceeded.setVisible(false);



            break;

        //Chords practical intro screen
        case 6:
        
            currentChord.setText("Currently playing: - - ", dontSendNotification);
            practiceChords.setVisible(true);
            currentChord.setVisible(true);
            doneChords1.setVisible(true);
            nameOfChord.setButtonText("*");
            nameOfChord.setVisible(true);
            firstChord.setColour(firstChord.textColourId, Colours::white);
            secondChord.setColour(firstChord.textColourId, Colours::white);
            thirdChord.setColour(firstChord.textColourId, Colours::white);
            fourthChord.setColour(firstChord.textColourId, Colours::white);
            firstChord.setText("C MAJOR -", dontSendNotification);
            secondChord.setText("F MAJOR -", dontSendNotification);
            thirdChord.setText("G MAJOR -", dontSendNotification);
            fourthChord.setText("C MAJOR", dontSendNotification);

            CompletedChords.setVisible(false);
            backtoChordsIntro.setVisible(false);
            title5.setVisible(false);
            chordsTheory.setVisible(false);
            tryChords.setVisible(false);
            remainingChords.setVisible(false);
            doneChords3.setVisible(false);
            trueTestcurrentChord.setVisible(false);
            firstChord.setVisible(false);
            secondChord.setVisible(false);
            thirdChord.setVisible(false);
            fourthChord.setVisible(false);
            resetChord.setVisible(false);
            backtoChordsIntro.setVisible(false);
            goToWinChords.setVisible(false);

            break;

        //A minor pause screen
        case 7:
            arrayCounter2 = 0;
            attemptsRemaining = 3;
            remainingNotes.setColour(remainingNotes.textColourId, Colours::white);
            remainingNotesA = "8";
            stringtoTrim = "Remaining Notes ";
            remainingNotes.setText(stringtoTrim + remainingNotesA, dontSendNotification);
            startScales.setVisible(true);
            playScales.setVisible(true);



            practiceScale.setVisible(false);
            currentNote.setVisible(false);
            nextNote.setVisible(false);
            buttonNote.setVisible(false);
            doneScales1.setVisible(false);


            break;

        //A minor scale ascending true test
        case 8:
            remainingNotes.setText(stringtoTrim + remainingNotesA, dontSendNotification);
            remainingNotes.setVisible(true);
            Failed.setVisible(true);
            Suceeded.setVisible(true);


            startScales.setVisible(false);
            playScales.setVisible(false);
            failedString.setVisible(false);
            correctnoteWas.setVisible(false);
            remainingAttempts.setVisible(false);
            tryAgain.setVisible(false);

            break;
            
        //A minor scale ascending fail page
        case 9:

            remainingAttempts.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            failedString.setVisible(true);
            correctnoteWas.setVisible(true);
            remainingAttempts.setVisible(true);
            tryAgain.setVisible(true);


            if (attemptsRemaining == 0) {
                remainingAttempts.setVisible(false);
            }
            remainingNotes.setVisible(false);
            Failed.setVisible(false);
            Suceeded.setVisible(false);
            
            break;

        //A minor scale descending intro
        case 10:
            arrayCounter = 0;
            nextNoteArray = A_min_scalenotes[7];
            buttonNoteDescending.setColour(chords.buttonColourId, Colours::dodgerblue);
            practiceScaleDescending.setVisible(true);
            currentNoteDescending.setVisible(true);
            nextNoteDescending.setVisible(true);
            buttonNoteDescending.setVisible(true);
            doneScalesDescending.setVisible(true);


            remainingNotes.setVisible(false);
            Failed.setVisible(false);
            Suceeded.setVisible(false);
            failedString.setVisible(false);
            correctnoteWas.setVisible(false);
            remainingAttempts.setVisible(false);
            tryAgain.setVisible(false);
            remainingNotesDescending.setVisible(false);
            FailedDescending.setVisible(false);
            SuceededDescending.setVisible(false);
            break;

        //A minor scale descending pause screen
        case 11:
            arrayCounter2 = 0;
            attemptsRemaining = 3;
            stringtoTrim = "Remaining notes ";
            remainingNotesA = "8";
            startScalesDescending.setVisible(true);
            playScalesDescending.setVisible(true);


            practiceScaleDescending.setVisible(false);
            currentNoteDescending.setVisible(false);
            nextNoteDescending.setVisible(false);
            buttonNoteDescending.setVisible(false);
            doneScalesDescending.setVisible(false);
            break;
        
        //A minor scale descending true test
        case 12:
            remainingNotesDescending.setVisible(true);
            remainingNotesDescending.setColour(remainingNotes.textColourId, Colours::white);
            remainingNotesDescending.setText(stringtoTrim + remainingNotesA, dontSendNotification);
            FailedDescending.setVisible(true);
            SuceededDescending.setVisible(true);

            startScalesDescending.setVisible(false);
            playScalesDescending.setVisible(false);
            failedStringDescending.setVisible(false);
            correctnoteWasDescending.setVisible(false);
            remainingAttemptsDescending.setVisible(false);
            tryAgainDescending.setVisible(false);

            break;
        
        //A minor scale descending fail page
        case 13:
            remainingAttemptsDescending.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            failedStringDescending.setVisible(true);
            correctnoteWasDescending.setVisible(true);
            remainingAttemptsDescending.setVisible(true);
            tryAgainDescending.setVisible(true);


            if (attemptsRemaining == 0) {
                remainingAttemptsDescending.setVisible(false);
            }
            remainingNotesDescending.setVisible(false);
            FailedDescending.setVisible(false);
            SuceededDescending.setVisible(false);
            break;
        
        //C major scale ascending intro
        case 14:
            arrayCounter = 0;
            practiceScaleC.setVisible(true);
            currentNoteC.setVisible(true);
            nextNoteC.setVisible(true);
            buttonNoteC.setColour(buttonNoteC.buttonColourId, Colours::deepskyblue);
            buttonNoteC.setVisible(true);
            doneScalesC.setVisible(true);

            arrayCounter = 0;
            arrayCounter2 = 0;
            attemptsRemaining = 3;
            stringtoTrim = "Remaining notes ";
            remainingNotesA = "8";
            failedStringDescending.setVisible(false);
            correctnoteWasDescending.setVisible(false);
            tryAgainDescending.setVisible(false);
            remainingNotesDescending.setVisible(false);
            FailedDescending.setVisible(false);
            SuceededDescending.setVisible(false);
            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);

            break;

        //C major scale ascending pause screen
        case 15:
            arrayCounter2 = 0;
            attemptsRemaining = 3;
            stringtoTrim = "Remaining notes ";
            remainingNotesC_str = "8";
            startScalesC.setVisible(true);
            playScalesC.setVisible(true);


            practiceScaleC.setVisible(false);
            currentNoteC.setVisible(false);
            nextNoteC.setVisible(false);
            buttonNoteC.setVisible(false);
            doneScalesC.setVisible(false);

            break;
        
        //C major scale ascending true test
        case 16:
            remainingNotesC.setText(stringtoTrim + remainingNotesC_str, dontSendNotification);
            remainingNotesC.setVisible(true);
            FailedC.setVisible(true);
            SuceededC.setVisible(true);
            nextNoteArrayC = "c";

            startScalesC.setVisible(false);
            playScalesC.setVisible(false);
            failedStringC.setVisible(false);
            correctnoteWasC.setVisible(false);
            remainingAttemptsC.setVisible(false);
            tryAgainC.setVisible(false);

            break;
        
        //C major scale ascending fail page
        case 17:
            failedStringC.setVisible(true);
            correctnoteWasC.setVisible(true);
            remainingAttemptsC.setVisible(true);
            tryAgainC.setVisible(true);

            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);
            remainingAttemptsC.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            
            if (attemptsRemaining==0) {
                remainingAttemptsC.setVisible(false);
            }
            
            break;

        //Scales finish page
        case 18:
            finishScale.setVisible(true);
            scalesCompleded.setVisible(true);
            timeThreshold = 300;


            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);
            remainingNotesCDescending.setVisible(false);
            FailedCDescending.setVisible(false);
            SuceededCDescending.setVisible(false);
            failedStringCDescending.setVisible(false);
            correctnoteWasCDescending.setVisible(false);
            remainingAttemptsCDescending.setVisible(false);
            tryAgainCDescending.setVisible(false);
            break;

        //C - F - G chord progression intro
        case 19:

            chordProgression.setVisible(true);
            doneChords2.setVisible(true);
            chordNoteCounter = 0;
            numOfCorrectChords = 0;
            Triad_Chord_notes[0] = "";
            Triad_Chord_notes[1] = "";
            Triad_Chord_notes[2] = "";
            chordLives = 3;
            attemptsRemaining = 5;

            practiceChords.setVisible(false);
            currentChord.setVisible(false);
            doneChords1.setVisible(false);
            nameOfChord.setVisible(false);

            break;

        //A - D - E chord identifier
        case 20:
            
            currentChord.setText("Currently playing: - - ", dontSendNotification);
            practiceChords.setVisible(true);
            currentChord.setVisible(true);
            to_A.setVisible(true);
            nameOfChord.setButtonText("*");
            nameOfChord.setVisible(true);
            firstChord.setColour(firstChord.textColourId, Colours::white);
            secondChord.setColour(firstChord.textColourId, Colours::white);
            thirdChord.setColour(firstChord.textColourId, Colours::white);
            fourthChord.setColour(firstChord.textColourId, Colours::white);

            CompletedChords.setVisible(false);
            chordProgression.setVisible(false);
            doneChords2.setVisible(false);
            backtoChordsIntro2.setVisible(false);
            remainingChords.setVisible(false);
            goToWinChords.setVisible(false);
            trueTestcurrentChord.setVisible(false);
            firstChord.setVisible(false);
            secondChord.setVisible(false);
            thirdChord.setVisible(false);
            fourthChord.setVisible(false);
            resetChord.setVisible(false);
            backtoChordsIntro2.setVisible(false);

            break;

        //C - F - G chord progression true test
        case 21:
            remainingChords.setVisible(true);
            doneChords3.setVisible(true);
            trueTestcurrentChord.setVisible(true);
            trueTestcurrentChord.setText("Currently playing:  - - ", dontSendNotification);
            firstChord.setVisible(true);
            secondChord.setVisible(true);
            thirdChord.setVisible(true);
            fourthChord.setVisible(true);
            attemptsRemainingChords.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            resetChord.setVisible(true);
            backtoChordsIntro.setVisible(true);

            CompletedChords.setVisible(false);
            failedChord.setVisible(false);
            attemptsRemainingChords.setVisible(false);
            playChords.setVisible(false);
            startChords.setVisible(false);
            backtoChords.setVisible(false);
            chordProgression.setVisible(false);
            doneChords2.setVisible(false);
            ChordTheoryFail.setVisible(false);
            break;


        //A - D - E chord progression intro
        case 22:
            doneChords4.setVisible(true);
            chordProgression2.setVisible(true);
            chordNoteCounter = 0;
            numOfCorrectChords = 0;
            Triad_Chord_notes[0] = "";
            Triad_Chord_notes[1] = "";
            Triad_Chord_notes[2] = "";
            chordLives = 3;
            attemptsRemaining = 5;
            firstChord.setColour(firstChord.textColourId, Colours::white);
            secondChord.setColour(firstChord.textColourId, Colours::white);
            thirdChord.setColour(firstChord.textColourId, Colours::white);
            fourthChord.setColour(firstChord.textColourId, Colours::white);
            firstChord.setText("A MINOR -", dontSendNotification);
            secondChord.setText("D MINOR -", dontSendNotification);
            thirdChord.setText("E MINOR -", dontSendNotification);
            fourthChord.setText("A MINOR", dontSendNotification);

            backtoChordsIntro.setVisible(false);
            resetChord.setVisible(false);
            trueTestcurrentChord.setVisible(false);
            remainingChords.setVisible(false);
            doneChords3.setVisible(false);
            firstChord.setVisible(false);
            secondChord.setVisible(false);
            thirdChord.setVisible(false);
            fourthChord.setVisible(false);
            failedChord.setVisible(false);
            attemptsRemainingChords.setVisible(false);
            backtoChords.setVisible(false);
            CompletedChords.setVisible(false);
            practiceChords.setVisible(false);
            currentChord.setVisible(false);
            to_A.setVisible(false);
            nameOfChord.setVisible(false);
            ChordTheoryFail.setVisible(false);
            break;

        //A - D - E chord progression pause page - IS NOT USED
        case 23:
            startChords2.setVisible(true); 
            playChords2.setVisible(true);


            doneChords4.setVisible(false);
            chordProgression2.setVisible(false);
            

            break;
        
        //A - D - E chord progression true test
        case 24:
            remainingChords.setVisible(true);
            goToWinChords.setVisible(true);
            trueTestcurrentChord.setVisible(true);
            trueTestcurrentChord.setText("Currently playing:  - - ", dontSendNotification);
            firstChord.setVisible(true);
            secondChord.setVisible(true);
            thirdChord.setVisible(true);
            fourthChord.setVisible(true);
            resetChord.setVisible(true);
            attemptsRemainingChords.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            backtoChordsIntro2.setVisible(true);

            failedChord.setVisible(false);
            attemptsRemainingChords.setVisible(false);
            startChords2.setVisible(false);
            playChords2.setVisible(false);
            backtoChords2.setVisible(false);
            doneChords4.setVisible(false);
            chordProgression2.setVisible(false);
            ChordTheoryFail.setVisible(false);

            break;

        //C major scale descending intro
        case 25:
            nextNoteArrayC = "c";
            arrayCounter = 0;
            remainingNotesC_str = "8";
            attemptsRemaining = 3;
            practiceScaleCDescending.setVisible(true);
            currentNoteCDescending.setVisible(true);
            nextNoteCDescending.setVisible(true);
            buttonNoteCDescending.setColour(chords.buttonColourId, Colours::deepskyblue);
            buttonNoteCDescending.setVisible(true);
            doneScalesCDescending.setVisible(true);

            remainingNotesCDescending.setVisible(false);
            FailedCDescending.setVisible(false);
            SuceededCDescending.setVisible(false);
            failedStringC.setVisible(false);
            correctnoteWasC.setVisible(false);
            remainingAttemptsC.setVisible(false);
            tryAgainC.setVisible(false);
            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);
            break; 

        //C major scale descending pause screen
        case 26:
            arrayCounter2 = 0;
            playScalesCDescending.setVisible(true);
            startScalesCDescending.setVisible(true);


            practiceScaleCDescending.setVisible(false);
            currentNoteCDescending.setVisible(false);
            nextNoteCDescending.setVisible(false);
            buttonNoteCDescending.setVisible(false);
            doneScalesCDescending.setVisible(false);
            stringtoTrim = "Remaining notes ";

            break;

        //C major scale descending true test
        case 27:
            remainingNotesCDescending.setText(stringtoTrim+remainingNotesC_str, dontSendNotification);
            remainingNotesCDescending.setVisible(true);
            FailedCDescending.setVisible(true);
            SuceededCDescending.setVisible(true);

            playScalesCDescending.setVisible(false);
            startScalesCDescending.setVisible(false);
            failedStringCDescending.setVisible(false);
            correctnoteWasCDescending.setVisible(false);
            remainingAttemptsCDescending.setVisible(false);
            tryAgainCDescending.setVisible(false);
            break;


        //C major scale descending fail page
        case 28:
            failedStringCDescending.setVisible(true);
            correctnoteWasCDescending.setVisible(true);
            remainingAttemptsCDescending.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            remainingAttemptsCDescending.setVisible(true);
            tryAgainCDescending.setVisible(true);
            

            if (attemptsRemaining == 0) {
                remainingAttemptsCDescending.setVisible(false);
            }
            remainingNotesCDescending.setVisible(false);
            FailedCDescending.setVisible(false);
            SuceededCDescending.setVisible(false);
            break;

        //Chords C-F-G FAIL page
        case 29:
            attemptsRemainingChords.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            failedChord.setVisible(true);
            attemptsRemainingChords.setVisible(true);
            backtoChords.setVisible(true);
            ChordTheoryFail.setVisible(true);

            if (attemptsRemaining == 0) {
                attemptsRemainingChords.setText("Continue to the next page", dontSendNotification);
                ChordTheoryFail.setVisible(false);
            }
            remainingChords.setVisible(false);
            doneChords3.setVisible(false);
            trueTestcurrentChord.setVisible(false);
            firstChord.setVisible(false);
            secondChord.setVisible(false);
            thirdChord.setVisible(false);
            fourthChord.setVisible(false);
            resetChord.setVisible(false);
            backtoChordsIntro.setVisible(false);
            
            break; 
        //Chords A-D-E FAIL page
        case 30:
            attemptsRemainingChords.setText(failed3 + " attempt(s) remaining", dontSendNotification);
            failedChord.setVisible(true);
            attemptsRemainingChords.setVisible(true);
            backtoChords2.setVisible(true);
            ChordTheoryFail.setVisible(true);

            if (attemptsRemaining == 0) {
                attemptsRemainingChords.setText("Continue to the next page", dontSendNotification);
                ChordTheoryFail.setVisible(false);
            }
            remainingChords.setVisible(false);
            doneChords3.setVisible(false);
            trueTestcurrentChord.setVisible(false);
            firstChord.setVisible(false);
            secondChord.setVisible(false);
            thirdChord.setVisible(false);
            fourthChord.setVisible(false);
            goToWinChords.setVisible(false);
            resetChord.setVisible(false);
            backtoChordsIntro.setVisible(false);
            backtoChordsIntro2.setVisible(false);
            
            break;
        
        //Chords WIN page
        case 31:
            FinishedChordsLesson.setVisible(true);
            finishScale.setVisible(true);

            resetChord.setVisible(false);
            backtoChords2.setVisible(false);
            attemptsRemainingChords.setVisible(false);
            failedChord.setVisible(false);
            goToWinChords.setVisible(false);
            remainingChords.setVisible(false);
            goToWinChords.setVisible(false);
            trueTestcurrentChord.setVisible(false);
            firstChord.setVisible(false);
            secondChord.setVisible(false);
            thirdChord.setVisible(false);
            fourthChord.setVisible(false);
            CompletedChords.setVisible(false);
            backtoChordsIntro.setVisible(false);
            backtoChordsIntro2.setVisible(false);
            ChordTheoryFail.setVisible(false);
            break;
        }
    }
    //Whenever a switch happens pageNum_OLD is overwrited to the value of pageNum.
    //Meaning that the program can then detect that a change in page number has happened.
    audioProcessor.pageNum_OLD = audioProcessor.pageNum;


    //If a note has been updated, it sets the letter to update in real time as you play on the scale page.

    /*if (audioProcessor.playedNote != audioProcessor.playedNoteOLD && updateNote) {
        newNote = true;
    }
    else {
        newNote = false;
    }*/

    //Config page
    if (audioProcessor.pageNum == 1) {
        //startTimer(100);
        configPlayedNote.setText(audioProcessor.playedNote, dontSendNotification);
        analogValues.setText("Index: " + audioProcessor.index +"| Middle: "+audioProcessor.middle+"| Ring: "+audioProcessor.ring+"| Pinky: "+audioProcessor.pinky, dontSendNotification);
    }
    //A minor scale ASCENDING intro page logic
    if (audioProcessor.pageNum == 5) {
        stringNote = audioProcessor.playedNote;
        //Currently playing shows the note that you are currently playing
        currentNote.setText("Currently playing " + stringNote, dontSendNotification);

        //Each of the if-statements check that you play the notes in the correct order and shows the next
        //up-coming note.
        if (audioProcessor.playedNote == "A" && arrayCounter == 0) {
            nextNoteArray = A_min_scalenotes[1];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter == 1) {
            nextNoteArray = A_min_scalenotes[2];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "C" && arrayCounter == 2) {
            nextNoteArray = A_min_scalenotes[3];
            arrayCounter = arrayCounter + 1;
        }
        
        if (audioProcessor.playedNote == "D" && arrayCounter == 3) {
            nextNoteArray = A_min_scalenotes[4];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "E" && arrayCounter == 4) {
            nextNoteArray = A_min_scalenotes[5];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "F" && arrayCounter == 5) {
            nextNoteArray = A_min_scalenotes[6];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter == 6) {
            nextNoteArray = A_min_scalenotes[7];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "A" && arrayCounter == 7) {
            nextNoteArray = "task completed";
            buttonNote.setColour(chords.buttonColourId, Colours::green);
            
        }

        //Button text is set according to the value of nextNoteArray
        buttonNote.setButtonText(nextNoteArray);
            
    }

    //A minor scale ASCENDING true test page logic
    if (audioProcessor.pageNum == 8 && audioProcessor.playedNote != " ") {
        
        //Used in the else if statements that check for "fails". You can only fail by playing
        //a wrong note and sustaining it for 1000 milliseconds.
        timeThreshold = 1000;

        //Very similar to the intro page above, the difference is just that you can also fail on
        //the true test page

        //Each of the if-statements updates the number of remaining notes you have to play on the screen
        //for each correct note that is played
        if (audioProcessor.playedNote == "A" && arrayCounter2 == 0) {
            remainingNotesA = "7";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        //Each of the else if-statements send you to the fail page and makes you lose 1 "life" each time
        //you make a mistake by decrementing attemptsRemaining. 
        else if (audioProcessor.playedNote != "A" && arrayCounter2 == 0 && audioProcessor.elapsedTime> timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter2 == 1) {
            remainingNotesA = "6";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "B" && audioProcessor.playedNote != oldNote && arrayCounter2 == 1 && audioProcessor.elapsedTime > timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }
        

        if (audioProcessor.playedNote == "C" && arrayCounter2 == 2) {
            remainingNotesA = "5";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "C" && audioProcessor.playedNote != oldNote && arrayCounter2 == 2 && audioProcessor.elapsedTime > timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "D" && arrayCounter2 == 3) {
            remainingNotesA = "4";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "D" && audioProcessor.playedNote != oldNote && arrayCounter2 == 3 && audioProcessor.elapsedTime > timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "E" && arrayCounter2 == 4) {
            remainingNotesA = "3";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "E" && audioProcessor.playedNote != oldNote && arrayCounter2 == 4 && audioProcessor.elapsedTime > timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "F" && arrayCounter2 == 5) {
            remainingNotesA = "2";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "F" && audioProcessor.playedNote != oldNote && arrayCounter2 == 5 && audioProcessor.elapsedTime > timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter2 == 6) {
            remainingNotesA = "1";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;

        }
        else if (audioProcessor.playedNote != "G" && audioProcessor.playedNote != oldNote && arrayCounter2 == 6 && audioProcessor.elapsedTime > timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }

        //If you play all notes correct, you win the exercise and some WIN text is displayed to you on screen
        if (audioProcessor.playedNote == "A" && arrayCounter2 == 7) {
            stringtoTrim.clear();
            remainingNotesA = "WELL DONE!";
            remainingNotes.setBounds(440, 300, 500, 40);
            remainingNotes.setColour(remainingNotes.textColourId,Colours::green);
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;

        }
        else if (audioProcessor.playedNote != "A" && audioProcessor.playedNote != oldNote && arrayCounter2 == 7 && audioProcessor.elapsedTime > timeThreshold) {
            audioProcessor.pageNum = 9;
            attemptsRemaining = attemptsRemaining - 1;
        }

        //The Label where the remaining notes are displayed
        remainingNotes.setText(stringtoTrim + remainingNotesA, dontSendNotification);

    }

    //SCALES----------------------------------------------------------------------------------------------------------
    
    //A minor scale DESCENDING intro page logic
    if (audioProcessor.pageNum == 10) {
        
        //Uses the exact same template as A minor scale ASCENDING intro page logic
        stringNoteDescending = audioProcessor.playedNote;
        currentNoteDescending.setText("Currently playing " + stringNoteDescending, dontSendNotification);

        if (audioProcessor.playedNote == "A" && arrayCounter == 0) {
            nextNoteArray = A_min_scalenotes[6];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter == 1) {
            nextNoteArray = A_min_scalenotes[5];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "F" && arrayCounter == 2) {
            nextNoteArray = A_min_scalenotes[4];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "E" && arrayCounter == 3) {
            nextNoteArray = A_min_scalenotes[3];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "D" && arrayCounter == 4) {
            nextNoteArray = A_min_scalenotes[2];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "C" && arrayCounter == 5) {
            nextNoteArray = A_min_scalenotes[1];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter == 6) {
            nextNoteArray = A_min_scalenotes[0];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "A" && arrayCounter == 7) {
            nextNoteArray = "task completed";
            buttonNoteDescending.setColour(chords.buttonColourId, Colours::green);

        }


        buttonNoteDescending.setButtonText(nextNoteArray);

    }

    //A minor scale DESCENDING true test page logic
    if (audioProcessor.pageNum == 12 && audioProcessor.playedNote != " ") {

        timeThreshold = 1000;

        //Uses the exact same template as A minor scale ASCENDING true test page logic
        if (audioProcessor.playedNote == "A" && arrayCounter2 == 0) {
            remainingNotesA = "7";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "A" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 0 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter2 == 1) {
            remainingNotesA = "6";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "G" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 1 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }


        if (audioProcessor.playedNote == "F" && arrayCounter2 == 2) {
            remainingNotesA = "5";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "F" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 2 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "E" && arrayCounter2 == 3) {
            remainingNotesA = "4";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "E" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 3 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "D" && arrayCounter2 == 4) {
            remainingNotesA = "3";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "D" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 4 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "C" && arrayCounter2 == 5) {
            remainingNotesA = "2";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "C" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 5 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter2 == 6) {
            remainingNotesA = "1";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "B" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 6 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "A" && arrayCounter2 == 7) {
            stringtoTrim.clear();
            remainingNotesA = "WELL DONE!";
            remainingNotesDescending.setBounds(440, 300, 500, 40);
            remainingNotesDescending.setColour(remainingNotes.textColourId, Colours::green);
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "A" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 7 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 13;
            attemptsRemaining = attemptsRemaining - 1;
        }

        remainingNotesDescending.setText(stringtoTrim + remainingNotesA, dontSendNotification);

    }

    //C major scale ASCENDING intro page logic
    if (audioProcessor.pageNum == 14) {

        //Uses the exact same template as A minor scale ASCENDING intro page logic
        stringNoteC = audioProcessor.playedNote;
        currentNoteC.setText("Currently playing " + stringNoteC, dontSendNotification);

        if (audioProcessor.playedNote == "C" && arrayCounter == 0) {
            nextNoteArrayC = C_maj_scalenotes[1];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "D" && arrayCounter == 1) {
            nextNoteArrayC = C_maj_scalenotes[2];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "E" && arrayCounter == 2) {
            nextNoteArrayC = C_maj_scalenotes[3];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "F" && arrayCounter == 3) {
            nextNoteArrayC = C_maj_scalenotes[4];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter == 4) {
            nextNoteArrayC = C_maj_scalenotes[5];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "A" && arrayCounter == 5) {
            nextNoteArrayC = C_maj_scalenotes[6];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter == 6) {
            nextNoteArrayC = C_maj_scalenotes[7];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "C" && arrayCounter == 7) {
            nextNoteArrayC = "task completed";
            buttonNoteC.setColour(chords.buttonColourId, Colours::green);

        }


        buttonNoteC.setButtonText(nextNoteArrayC);

    }

    //C major scale ASCENDING true test page logic
    if (audioProcessor.pageNum == 16 && audioProcessor.playedNote != " ") {

        timeThreshold = 1000;

        //Uses the exact same template as A minor scale ASCENDING true test page logic
        if (audioProcessor.playedNote == "C" && arrayCounter2 == 0) {
            remainingNotesC_str = "7";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "C" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 0 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "D" && arrayCounter2 == 1) {
            remainingNotesC_str = "6";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "D" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 1 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }


        if (audioProcessor.playedNote == "E" && arrayCounter2 == 2) {
            remainingNotesC_str = "5";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "E" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 2 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "F" && arrayCounter2 == 3) {
            remainingNotesC_str = "4";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "F" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 3 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter2 == 4) {
            remainingNotesC_str = "3";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "G" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 4 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "A" && arrayCounter2 == 5) {
            remainingNotesC_str = "2";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "A" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 5 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter2 == 6) {
            remainingNotesC_str = "1";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "B" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 6 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "C" && arrayCounter2 == 7) {
            stringtoTrim.clear();
            remainingNotesC_str = "WELL DONE!";
            remainingNotesC.setBounds(440, 300, 500, 40);
            remainingNotesC.setColour(remainingNotes.textColourId, Colours::green);
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "C" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 7 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        remainingNotesC.setText(stringtoTrim + remainingNotesC_str, dontSendNotification);

    }

    //C major scale DESCENDING intro page logic
    if (audioProcessor.pageNum == 25) {

        //Uses the exact same template as A minor scale ASCENDING intro page logic
        stringNoteC = audioProcessor.playedNote;
        currentNoteCDescending.setText("Currently playing " + stringNoteC, dontSendNotification);

        if (audioProcessor.playedNote == "C" && arrayCounter == 0) {
            nextNoteArrayC = C_maj_scalenotes[6];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter == 1) {
            nextNoteArrayC = C_maj_scalenotes[5];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "A" && arrayCounter == 2) {
            nextNoteArrayC = C_maj_scalenotes[4];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter == 3) {
            nextNoteArrayC = C_maj_scalenotes[3];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "F" && arrayCounter == 4) {
            nextNoteArrayC = C_maj_scalenotes[2];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "E" && arrayCounter == 5) {
            nextNoteArrayC = C_maj_scalenotes[1];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "D" && arrayCounter == 6) {
            nextNoteArrayC = C_maj_scalenotes[0];
            arrayCounter = arrayCounter + 1;
        }

        if (audioProcessor.playedNote == "C" && arrayCounter == 7) {
            nextNoteArrayC = "task completed";
            buttonNoteCDescending.setColour(chords.buttonColourId, Colours::green);

        }


        buttonNoteCDescending.setButtonText(nextNoteArrayC);

    }

    //C major scale DESCENDING true test page logic
    if (audioProcessor.pageNum == 27 && audioProcessor.playedNote != " ") {

        timeThreshold = 1000;

        //Uses the exact same template as A minor scale ASCENDING true test page logic
        if (audioProcessor.playedNote == "C" && arrayCounter2 == 0) {
            remainingNotesC_str = "7";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "C" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 0 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 28;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "B" && arrayCounter2 == 1) {
            remainingNotesC_str = "6";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "B" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 1 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 28;
            attemptsRemaining = attemptsRemaining - 1;
        }


        if (audioProcessor.playedNote == "A" && arrayCounter2 == 2) {
            remainingNotesC_str = "5";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "A" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 2 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 28;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "G" && arrayCounter2 == 3) {
            remainingNotesC_str = "4";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "G" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 3 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 28;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "F" && arrayCounter2 == 4) {
            remainingNotesC_str = "3";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "F" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 4 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 28;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "E" && arrayCounter2 == 5) {
            remainingNotesC_str = "2";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "E" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 5 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 28;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "D" && arrayCounter2 == 6) {
            remainingNotesC_str = "1";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "D" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 6 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 28;
            attemptsRemaining = attemptsRemaining - 1;
        }

        if (audioProcessor.playedNote == "C" && arrayCounter2 == 7) {
            stringtoTrim.clear();
            remainingNotesC_str = "WELL DONE!";
            remainingNotesCDescending.setBounds(440, 300, 500, 40);
            remainingNotesCDescending.setColour(remainingNotes.textColourId, Colours::green);
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
        else if (audioProcessor.playedNote != "C" && audioProcessor.elapsedTime > timeThreshold && arrayCounter2 == 7 && audioProcessor.playedNote != oldNote) {
            audioProcessor.pageNum = 17;
            attemptsRemaining = attemptsRemaining - 1;
        }

        remainingNotesCDescending.setText(stringtoTrim + remainingNotesC_str, dontSendNotification);
    }


    //CHORDS----------------------------------------------------------------------------------------------------------

    //Chord identifier intro page logic
    if (audioProcessor.pageNum == 6 && audioProcessor.playedNote != " " && audioProcessor.elapsedTime>timeThreshold) {

        //There is a 300 milliseconds delay for playing the notes of the chords, so that the user
        //has a little more time to select their notes before they are played as part of the chord
        timeThreshold = 300;
        stringChord = audioProcessor.playedNote;
        
        //Each if-statement checks that the notes that the user has played and adds them to a String array
        //with a fixed size of 3. Each String corresponds to one of the three notes in a triad
        if (chordNoteCounter==0 && stringChord != " ") {
            Triad_Chord_notes[0] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            first_note = stringChord;
        }

        //Only goes to the next notes in the array if the currently played note is not the same as the previously
        //played note - so that you can't get duplicates of the same note within the chord.
        if (chordNoteCounter == 1 && first_note != stringChord && stringChord != " ") {
            Triad_Chord_notes[1] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            second_note = stringChord;
        }

        if (chordNoteCounter == 2 && first_note != stringChord && second_note != stringChord && stringChord != " ") {
            Triad_Chord_notes[2] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            third_note = stringChord;
        }

        //If you have played three notes in total and they play a fourth that is not the same as the last note,
        //you will reset the whole chord. So that you can start another chord when you are done with the current chord
        if (chordNoteCounter == 3 && third_note != stringChord && stringChord != " ") {
            chordNoteCounter = 0;
            Triad_Chord_notes[0] = "";
            Triad_Chord_notes[1] = "";
            Triad_Chord_notes[2] = "";
            nameOfChord.setButtonText("*");
        }

        //Label that sets part of the text according to the notes that you are currently playing to form a chord
        currentChord.setText("Currently playing: " + Triad_Chord_notes[0] + " - " + Triad_Chord_notes[1] + " - " + Triad_Chord_notes[2], dontSendNotification);

        //FullChord collects all of your chord tones and combines them into a single string
        String FullChord = Triad_Chord_notes[0] + Triad_Chord_notes[1] + Triad_Chord_notes[2];

        //All the if-statements check for the chord tone combnations that are "valid" and sets the text of the
        //button to match the name of the chord that's been played
        if (FullChord.contains("A") && FullChord.contains("C") && FullChord.contains("E")) {
            nameOfChord.setButtonText("A-minor");
        }

        if (FullChord.contains("B") && FullChord.contains("D") && FullChord.contains("F")) {
            nameOfChord.setButtonText("B-diminished");
        }

        if (FullChord.contains("C") && FullChord.contains("E") && FullChord.contains("G")) {
            nameOfChord.setButtonText("C-major");
        }

        if (FullChord.contains("D") && FullChord.contains("F") && FullChord.contains("A")) {
            nameOfChord.setButtonText("D-minor");
        }

        if (FullChord.contains("E") && FullChord.contains("G") && FullChord.contains("B")) {
            nameOfChord.setButtonText("E-minor");
        }

        if (FullChord.contains("F") && FullChord.contains("A") && FullChord.contains("C")) {
            nameOfChord.setButtonText("F-major");
        }

        if (FullChord.contains("G") && FullChord.contains("B") && FullChord.contains("D")) {
            nameOfChord.setButtonText("G-major");
        }
    }

    //C-F-G-C chord progression true test page logic
    if (audioProcessor.pageNum == 21 && audioProcessor.playedNote != " " && audioProcessor.elapsedTime > timeThreshold) {

        stringChord = audioProcessor.playedNote;
        timeThreshold = 300;
        //Very similar to the Chord identifier intro page logic above, the difference is just that you can fail here

        if (chordNoteCounter == 0 && stringChord != " ") {
            Triad_Chord_notes[0] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            first_note = stringChord;
            failedChord.setVisible(false);
            attemptsRemainingChords.setVisible(false);
        }

        if (chordNoteCounter == 1 && first_note != stringChord && stringChord != " ") {
            Triad_Chord_notes[1] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            second_note = stringChord;
        }

        if (chordNoteCounter == 2 && first_note != stringChord && second_note != stringChord && stringChord != " ") {
            Triad_Chord_notes[2] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            third_note = stringChord;
        }

        if (chordNoteCounter == 3 && third_note != stringChord && stringChord != " ") {
            chordNoteCounter = 0;
            Triad_Chord_notes[0] = "";
            Triad_Chord_notes[1] = "";
            Triad_Chord_notes[2] = "";
            nameOfChord.setButtonText("*");
            oldNote = " ";
        }

        trueTestcurrentChord.setText("Currently playing: " + Triad_Chord_notes[0] + " - " + Triad_Chord_notes[1] + " - " + Triad_Chord_notes[2], dontSendNotification);

        String FullChord = Triad_Chord_notes[0] + Triad_Chord_notes[1] + Triad_Chord_notes[2];


        if(chordNoteCounter==3){

            if (FullChord.contains("C") && FullChord.contains("E") && FullChord.contains("G") && numOfCorrectChords==0) {
            firstChord.setColour(remainingChords.textColourId, Colours::green);
            numOfCorrectChords = 1;
            oldNote = stringChord;
            }

            //The else if statements trigger whenever you play a wrong chord. You are then taken to a fail page and you lose
            //1 "life" each time you fail, when attemptReamining is decremented
            else if (numOfCorrectChords==0 && oldNote != stringChord){
            audioProcessor.pageNum = 29;
            attemptsRemaining = attemptsRemaining - 1;
            }

            if (FullChord.contains("F") && FullChord.contains("A") && FullChord.contains("C") && numOfCorrectChords==1) {
            secondChord.setColour(remainingChords.textColourId, Colours::green);
            numOfCorrectChords = 2;
            oldNote = stringChord;
            }

            else if (numOfCorrectChords == 1 && oldNote != stringChord) {
            audioProcessor.pageNum = 29;
            attemptsRemaining = attemptsRemaining - 1;
            }

            if (FullChord.contains("G") && FullChord.contains("B") && FullChord.contains("D") && numOfCorrectChords==2) {
            thirdChord.setColour(remainingChords.textColourId, Colours::green);
            numOfCorrectChords = 3;
            oldNote = stringChord;
            }
            else if (numOfCorrectChords == 2 && oldNote != stringChord) {
            audioProcessor.pageNum = 29;
            attemptsRemaining = attemptsRemaining - 1;
            }

            //WIN text is shown when you have completed the exercise
            if (FullChord.contains("C") && FullChord.contains("E") && FullChord.contains("G") && numOfCorrectChords==3) {
            numOfCorrectChords = 4;
            fourthChord.setColour(fourthChord.textColourId, Colours::green);
            CompletedChords.setVisible(true);
           
            }
            else if (numOfCorrectChords == 3 && oldNote != stringChord) {
            audioProcessor.pageNum = 29;
            attemptsRemaining = attemptsRemaining - 1;
            }
        }
    }

    //A-D-E-A chord progression true test page logic
    if (audioProcessor.pageNum == 24 && audioProcessor.playedNote != " " && audioProcessor.elapsedTime > timeThreshold) {

        stringChord = audioProcessor.playedNote;
        timeThreshold = 300;
        //Uses the exact same template as the C-F-G-C chord progression true test page logic

        if (chordNoteCounter == 0 && stringChord != " ") {
            Triad_Chord_notes[0] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            first_note = stringChord;
            failedChord.setVisible(false);
            attemptsRemainingChords.setVisible(false);
        }

        if (chordNoteCounter == 1 && first_note != stringChord && stringChord != " ") {
            Triad_Chord_notes[1] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            second_note = stringChord;
        }

        if (chordNoteCounter == 2 && first_note != stringChord && second_note != stringChord && stringChord != " ") {
            Triad_Chord_notes[2] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            third_note = stringChord;
        }

        if (chordNoteCounter == 3 && third_note != stringChord && stringChord != " ") {
            chordNoteCounter = 0;
            Triad_Chord_notes[0] = "";
            Triad_Chord_notes[1] = "";
            Triad_Chord_notes[2] = "";
            nameOfChord.setButtonText("*");
            oldNote = " ";
        }

        trueTestcurrentChord.setText("Currently playing: " + Triad_Chord_notes[0] + " - " + Triad_Chord_notes[1] + " - " + Triad_Chord_notes[2], dontSendNotification);

        String FullChord = Triad_Chord_notes[0] + Triad_Chord_notes[1] + Triad_Chord_notes[2];


        if (chordNoteCounter == 3) {

            if (FullChord.contains("A") && FullChord.contains("C") && FullChord.contains("E") && numOfCorrectChords == 0) {
                firstChord.setColour(remainingChords.textColourId, Colours::green);
                numOfCorrectChords = 1;
                oldNote = stringChord;
            }

            else if (numOfCorrectChords == 0 && oldNote != stringChord) {
                audioProcessor.pageNum = 30;
                attemptsRemaining = attemptsRemaining - 1;
            }

            if (FullChord.contains("D") && FullChord.contains("F") && FullChord.contains("A") && numOfCorrectChords == 1) {
                secondChord.setColour(remainingChords.textColourId, Colours::green);
                numOfCorrectChords = 2;
                oldNote = stringChord;
            }

            else if (numOfCorrectChords == 1 && oldNote != stringChord) {
                audioProcessor.pageNum = 30;
                attemptsRemaining = attemptsRemaining - 1;
            }

            if (FullChord.contains("E") && FullChord.contains("G") && FullChord.contains("B") && numOfCorrectChords == 2) {
                thirdChord.setColour(remainingChords.textColourId, Colours::green);
                numOfCorrectChords = 3;
                oldNote = stringChord;
            }
            else if (numOfCorrectChords == 2 && oldNote != stringChord) {
                audioProcessor.pageNum = 30;
                attemptsRemaining = attemptsRemaining - 1;
            }

            if (FullChord.contains("A") && FullChord.contains("C") && FullChord.contains("E") && numOfCorrectChords == 3) {
                numOfCorrectChords = 4;
                fourthChord.setColour(fourthChord.textColourId, Colours::green);
                CompletedChords.setVisible(true);
            }
            else if (numOfCorrectChords == 3 && oldNote != stringChord) {
                audioProcessor.pageNum = 30;
                attemptsRemaining = attemptsRemaining - 1;
            }
        }
        
    }
}



//==============================================================================
void RandomNameAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    
}

void RandomNameAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor.. this is where you set height, width etc. of the buttons
    title.setBounds(325, 10, 500, 40);
    newGame.setBounds(450, 400, 50, 50);
    howTo.setBounds(435, 300, 100, 50);
    title2.setBounds(300, 10, 500, 40);
    menu.setBounds(1000, 10, 100, 40);
    title3.setBounds(325, 10, 500, 40);
    chords.setBounds(500, 400, 100, 50);
    scales.setBounds(500, 300, 100, 50);
    title4.setBounds(500, 10, 500, 40);
    scalesTheory.setBounds(100, 10, 500, 500);
    scalesTheory2.setBounds(100, 200, 500, 500);
    tryScales.setBounds(850, 600, 100, 100);
    title5.setBounds(500, 10, 500, 40);
    chordsTheory.setBounds(100, 10, 600, 700);
    tryChords.setBounds(850, 600, 100, 100);
    practiceScale.setBounds(200, 10, 700, 200);
    currentNote.setBounds(200, 200, 300, 100);
    Note.setBounds(500, 200, 300, 100);
    nextNote.setBounds(200, 400, 300, 100);
    buttonNote.setBounds(400, 400, 300, 100);
    doneScales1.setBounds(850, 600, 100, 100);
    startScales.setBounds(500, 400, 100, 100);
    playScales.setBounds(200, 10, 900, 200);
    remainingNotes.setBounds(350, 300, 500, 40);
    Suceeded.setBounds(850, 600, 100, 100);
    Failed.setBounds(100, 600, 100, 100);
    failedString.setBounds(350, 200, 500, 40);
    correctnoteWas.setBounds(350, 250, 600, 100);
    remainingAttempts.setBounds(350, 450, 500, 40);
    tryAgain.setBounds(450, 550, 100, 100);
    practiceScaleDescending.setBounds(200, 10, 700, 200);
    currentNoteDescending.setBounds(200, 200, 300, 100);
    nextNoteDescending.setBounds(200, 400, 300, 100);
    buttonNoteDescending.setBounds(400, 400, 300, 100);
    doneScalesDescending.setBounds(850, 600, 100, 100);
    startScalesDescending.setBounds(500, 400, 100, 100);
    playScalesDescending.setBounds(200, 10, 900, 200);
    remainingNotesDescending.setBounds(350, 300, 500, 40);
    SuceededDescending.setBounds(850, 600, 100, 100);
    FailedDescending.setBounds(100, 600, 100, 100);
    failedStringDescending.setBounds(350, 200, 500, 40);
    correctnoteWasDescending.setBounds(350, 250, 600, 100);
    remainingAttemptsDescending.setBounds(350, 450, 500, 40);
    tryAgainDescending.setBounds(450, 550, 100, 100);
    practiceScaleC.setBounds(200, 10, 700, 200);
    currentNoteC.setBounds(200, 200, 300, 100);
    nextNoteC.setBounds(200, 400, 300, 100);
    buttonNoteC.setBounds(400, 400, 300, 100);
    doneScalesC.setBounds(850, 600, 100, 100);
    startScalesC.setBounds(500, 400, 100, 100);
    playScalesC.setBounds(200, 10, 900, 200);
    remainingNotesC.setBounds(350, 300, 500, 40);
    SuceededC.setBounds(850, 600, 100, 100);
    FailedC.setBounds(100, 600, 100, 100);
    failedStringC.setBounds(350, 200, 500, 40);
    correctnoteWasC.setBounds(350, 250, 600, 100);
    remainingAttemptsC.setBounds(350, 450, 500, 40);
    tryAgainC.setBounds(450, 550, 100, 100);
    finishScale.setBounds(450, 400, 100, 100);
    scalesCompleded.setBounds(150, 10, 900, 200);
    practiceChords.setBounds(200, 10, 700, 200);
    currentChord.setBounds(200, 200, 300, 100);
    doneChords1.setBounds(850, 600, 100, 100);
    chordProgression.setBounds(300, 20, 500, 500);
    doneChords2.setBounds(850, 600, 100, 100);
    startChords.setBounds(500, 400, 100, 100);
    playChords.setBounds(200, 10, 900, 200);
    remainingChords.setBounds(100, 100, 600, 200);
    doneChords3.setBounds(850, 600, 100, 100);
    chordProgression2.setBounds(300, 20, 500, 500);
    doneChords4.setBounds(850, 600, 100, 100);
    startChords2.setBounds(500, 400, 100, 100);
    playChords2.setBounds(200, 10, 900, 200);
    remainingChords2.setBounds(300, 200, 600, 200);
    doneChords5.setBounds(850, 600, 100, 100);
    practiceScaleCDescending.setBounds(200, 10, 700, 200);
    currentNoteCDescending.setBounds(200, 200, 300, 100);
    nextNoteCDescending.setBounds(200, 400, 300, 100);
    buttonNoteCDescending.setBounds(400, 400, 300, 100);
    doneScalesCDescending.setBounds(850, 600, 100, 100);
    startScalesCDescending.setBounds(500, 400, 100, 100);
    playScalesCDescending.setBounds(200, 10, 900, 200);
    remainingNotesCDescending.setBounds(350, 300, 500, 40);
    SuceededCDescending.setBounds(850, 600, 100, 100);
    FailedCDescending.setBounds(100, 600, 100, 100);
    failedStringCDescending.setBounds(350, 200, 500, 40);
    correctnoteWasCDescending.setBounds(350, 250, 600, 100);
    remainingAttemptsCDescending.setBounds(350, 450, 500, 40);
    tryAgainCDescending.setBounds(450, 550, 100, 100);
    nameOfChord.setBounds(450, 350, 100, 100);
    trueTestcurrentChord.setBounds(100, 300, 300, 100);
    firstChord.setBounds(100, 200, 300, 100);
    secondChord.setBounds(285, 200, 300, 100);
    thirdChord.setBounds(465, 200, 300, 100);
    fourthChord.setBounds(650, 200, 300, 100);
    failedChord.setBounds(350, 200, 500, 40);
    attemptsRemainingChords.setBounds(350, 300, 500, 40);
    backtoChords.setBounds(450, 550, 100, 100);
    backtoChords2.setBounds(450, 550, 100, 100);
    CompletedChords.setBounds(100, 400, 800, 100);
    goToWinChords.setBounds(850, 600, 100, 100);
    FinishedChordsLesson.setBounds(150, 10, 900, 200);
    resetChord.setBounds(450, 550, 100, 100);
    backtoChordsIntro.setBounds(100, 600, 100, 100);
    backtoChordsIntro2.setBounds(100, 600, 100, 100);
    to_A.setBounds(850, 600, 100, 100);
    ChordTheoryFail.setBounds(350, 350, 600, 200);
    audioProcessor.IndexUP.setBounds(300, 100, 200, 30);
    audioProcessor.IndexLOW.setBounds(600, 100, 200, 30);
    audioProcessor.MiddleUP.setBounds(300, 200, 200, 30);
    audioProcessor.MiddleLOW.setBounds(600, 200, 200, 30);
    audioProcessor.RingUP.setBounds(300, 300, 200, 30);
    audioProcessor.RingLOW.setBounds(600, 300, 200, 30);
    audioProcessor.PinkyMID.setBounds(450, 400, 200, 30);
    analogValues.setBounds(250, 450, 600, 100);
    configPlayedNote.setBounds(530, 525, 600, 100);
    
}

