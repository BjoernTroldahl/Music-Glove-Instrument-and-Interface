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
    howTo.setButtonText("How to use and setup music glove");
    howTo.changeWidthToFitText();
    howTo.setColour(howTo.buttonColourId, Colours::green);
    howTo.onClick = [this] {
        audioProcessor.pageNum = 1;
    };
    //Parameters for the main title of music glove page
    addAndMakeVisible(title2);
    //It starts off as not visible because we only want it to be visible when pagenum==2
    title2.setVisible(false);
    title2.setText("Introduction to musicglove", dontSendNotification);
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
    scales.setButtonText("Scales");
    scales.changeWidthToFitText();
    scales.onClick = [this] {
        audioProcessor.pageNum = 4;
    };
    addAndMakeVisible(title4);
    title4.setVisible(false);
    title4.setText("Scales", dontSendNotification);
    title4.setFont(40);
    addAndMakeVisible(scalesTheory);
    scalesTheory.setVisible(false);
    scalesTheory.setText("Scales theory. What is a scale? Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat n", dontSendNotification);
    scalesTheory.setFont(30);
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
    title5.setText("Chords", dontSendNotification);
    title5.setFont(40);
    addAndMakeVisible(chordsTheory);
    chordsTheory.setVisible(false);
    chordsTheory.setText("Chords Theory. What is a chord? Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat n", dontSendNotification);
    chordsTheory.setFont(30);
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
    playScales.setText("Now try playing the notes of an A-minor scale in ascending order without assistance.", dontSendNotification);
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
    Failed.setButtonText("Failed");
    Failed.setColour(Failed.buttonColourId, Colours::deepskyblue);
    Failed.changeWidthToFitText();
    Failed.onClick = [this] {

        audioProcessor.pageNum = 9;

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
    correctnoteWas.setText("Remember sequence: A-B-C-D-E-F-G-A", dontSendNotification);
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
    playScalesDescending.setText("Now try playing the notes of an A-minor scale in DESCENDING order without assistance.", dontSendNotification);
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
    FailedDescending.setButtonText("Failed");
    FailedDescending.setColour(FailedDescending.buttonColourId, Colours::deepskyblue);
    FailedDescending.changeWidthToFitText();
    FailedDescending.onClick = [this] {

        audioProcessor.pageNum = 13;

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
    correctnoteWasDescending.setText("Remember sequence: A-G-F-E-D-C-B-A", dontSendNotification);
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
    practiceScaleC.setText("Good job. Now practice playing the notes of an C-MAJOR scale in ASCENDING order", dontSendNotification);
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
    buttonNoteC.setButtonText("C");
    buttonNoteC.changeWidthToFitText();
    buttonNoteC.setColour(chords.buttonColourId, Colours::dodgerblue);
    buttonNoteC.onClick = [this] {
        buttonNoteC.setButtonText("D");
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
    playScalesC.setText("Now try playing the notes of a C-major scale in ASCENDING order without assistance.", dontSendNotification);
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
    remainingNotesC.setText("Remaining Notes " + strC, dontSendNotification);
    remainingNotesC.setFont(40);

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR FAILED SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(FailedC);
    FailedC.setVisible(false);
    FailedC.setButtonText("Failed");
    FailedC.setColour(FailedC.buttonColourId, Colours::deepskyblue);
    FailedC.changeWidthToFitText();
    FailedC.onClick = [this] {

        audioProcessor.pageNum = 17;

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
    failedStringC.setText("Try again! you played " + failedC, dontSendNotification);
    failedStringC.setFont(40);

    //her skal failed 2 ændres til den note der havde været rigtig at spille eller bare fjernes hvis det er for besværligt
    addAndMakeVisible(correctnoteWasC);
    auto failed2C = String(correctNoteC);
    correctnoteWasC.setVisible(false);
    correctnoteWasC.setText("The correct note was " + failed2C, dontSendNotification);
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
    chordProgression.setText("A typical chord progression is: c major , f major, g major                                                                       Try playing these three chords in order:                             C-major: C + E + G                                  F-major: F + A + C                                 G-major: G + B + D", dontSendNotification);
    chordProgression.setFont(40);

    addAndMakeVisible(doneChords2);
    doneChords2.setVisible(false);
    doneChords2.setButtonText("Done/next");
    doneChords2.setColour(doneScales1.buttonColourId, Colours::deepskyblue);
    doneChords2.changeWidthToFitText();
    doneChords2.onClick = [this] {
        audioProcessor.pageNum = 20;
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
    remainingChords.setText("Remaining Chords:                    C MAJOR - F MAJOR - G MAJOR", dontSendNotification);
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
    chordProgression2.setText("Well done! Another typical chord progression is: A minor, D minor, E minor                                                                      Try playing these three chords in order:                               A-minor: A + C + E                                  D-minor: D + F + A                                E-minor: E + G + B", dontSendNotification);
    chordProgression2.setFont(40);

    addAndMakeVisible(doneChords4);
    doneChords4.setVisible(false);
    doneChords4.setButtonText("Done/next");
    doneChords4.setColour(doneChords4.buttonColourId, Colours::deepskyblue);
    doneChords4.changeWidthToFitText();
    doneChords4.onClick = [this] {
        audioProcessor.pageNum = 23;
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
    practiceScaleCDescending.setText("Good job. Now practice playing the notes of an C-MAJOR scale in DESCENDING order", dontSendNotification);
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
    playScalesCDescending.setText("Now try playing the notes of an C-MAJOR scale in DESCENDING order without assistance.", dontSendNotification);
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
    auto strCDescending = String(remainingNotesIntCDescending);
    remainingNotesCDescending.setVisible(false);
    remainingNotesCDescending.setText("Remaining Notes " + strCDescending, dontSendNotification);
    remainingNotesCDescending.setFont(40);

    //DET HER SKAL IKKE VÆRE EN KNAP MEN EN TRIGGER NÅR FAILED SKER SKAL DEN GØRE DET HER
    addAndMakeVisible(FailedCDescending);
    FailedCDescending.setVisible(false);
    FailedCDescending.setButtonText("Failed");
    FailedCDescending.setColour(FailedCDescending.buttonColourId, Colours::deepskyblue);
    FailedCDescending.changeWidthToFitText();
    FailedCDescending.onClick = [this] {

        audioProcessor.pageNum = 28;

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
    failedStringCDescending.setText("Try again! you played " + failedCDescending, dontSendNotification);
    failedStringCDescending.setFont(40);

    //her skal failed 2 ændres til den note der havde været rigtig at spille eller bare fjernes hvis det er for besværligt
    addAndMakeVisible(correctnoteWasCDescending);
    auto failed2CDescending = String(correctNoteCDescending);
    correctnoteWasCDescending.setVisible(false);
    correctnoteWasCDescending.setText("The correct note was " + failed2CDescending, dontSendNotification);
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



}

void RandomNameAudioProcessorEditor::timerCallback()
//The page counter/mechanism that changes the page.
//If the current page number is not the same as the old page number, then change the page.
{
    failed3 = to_string(attemptsRemaining);

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
        //remainingAttemptsDescending.setText("", dontSendNotification);
        tryAgainDescending.setButtonText("Next page");
        tryAgainDescending.onClick = [this] {
            audioProcessor.pageNum = 14;
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

            break;
        case 1:
            title2.setVisible(true);
            menu.setVisible(true);

            title.setVisible(false);
            newGame.setVisible(false);
            howTo.setVisible(false);

            break;
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

            break;

        case 3:

            title5.setVisible(true);
            tryChords.setVisible(true);
            chordsTheory.setVisible(true);

            chords.setVisible(false);
            title3.setVisible(false);
            scales.setVisible(false);

            break;

        case 4:
            title4.setVisible(true);
            scalesTheory.setVisible(true);
            tryScales.setVisible(true);

            chords.setVisible(false);
            title3.setVisible(false);
            scales.setVisible(false);


            break;
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
            tryScales.setVisible(false);



            break;


        case 6:

            currentChord.setText("Currently playing: " + Triad_Chord_notes[0] + " - " + Triad_Chord_notes[1] + " - " + Triad_Chord_notes[2], dontSendNotification);
            practiceChords.setVisible(true);
            currentChord.setVisible(true);
            doneChords1.setVisible(true);
            nameOfChord.setVisible(true);


            title5.setVisible(false);
            chordsTheory.setVisible(false);
            tryChords.setVisible(false);

            break;


        case 7:
            arrayCounter2 = 0;
            attemptsRemaining = 3;
            remainingNotes.setColour(remainingNotes.textColourId, Colours::white);
            //remainingNotesA = "8";
            //stringtoTrim = "Remaining Notes ";
            //remainingNotes.setText(stringtoTrim + remainingNotesA, dontSendNotification);
            startScales.setVisible(true);
            playScales.setVisible(true);



            practiceScale.setVisible(false);
            currentNote.setVisible(false);
            nextNote.setVisible(false);
            buttonNote.setVisible(false);
            doneScales1.setVisible(false);


            break;

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
            break;


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

        case 14:
            practiceScaleC.setVisible(true);
            currentNoteC.setVisible(true);
            nextNoteC.setVisible(true);
            buttonNoteC.setVisible(true);
            doneScalesC.setVisible(true);

            failedStringDescending.setVisible(false);
            correctnoteWasDescending.setVisible(false);
            tryAgainDescending.setVisible(false);
            remainingNotesDescending.setVisible(false);
            FailedDescending.setVisible(false);
            SuceededDescending.setVisible(false);

            break;
        case 15:
            startScalesC.setVisible(true);
            playScalesC.setVisible(true);


            practiceScaleC.setVisible(false);
            currentNoteC.setVisible(false);
            nextNoteC.setVisible(false);
            buttonNoteC.setVisible(false);
            doneScalesC.setVisible(false);

            break;

        case 16:
            remainingNotesC.setVisible(true);
            FailedC.setVisible(true);
            SuceededC.setVisible(true);


            startScalesC.setVisible(false);
            playScalesC.setVisible(false);
            failedStringC.setVisible(false);
            correctnoteWasC.setVisible(false);
            remainingAttemptsC.setVisible(false);
            tryAgainC.setVisible(false);

            break;

        case 17:
            failedStringC.setVisible(true);
            correctnoteWasC.setVisible(true);
            remainingAttemptsC.setVisible(true);
            tryAgainC.setVisible(true);

            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);

            break;


        case 18:
            finishScale.setVisible(true);
            scalesCompleded.setVisible(true);



            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);
            remainingNotesCDescending.setVisible(false);
            FailedCDescending.setVisible(false);
            SuceededCDescending.setVisible(false);
            break;


        case 19:

            chordProgression.setVisible(true);
            doneChords2.setVisible(true);

            practiceChords.setVisible(false);
            currentChord.setVisible(false);
            doneChords1.setVisible(false);


            break;


        case 20:
            playChords.setVisible(true);
            startChords.setVisible(true);

            chordProgression.setVisible(false);
            doneChords2.setVisible(false);

            break;


        case 21:

            remainingChords.setVisible(true);
            doneChords3.setVisible(true);


            playChords.setVisible(false);
            startChords.setVisible(false);
            break;



        case 22:
            doneChords4.setVisible(true);
            chordProgression2.setVisible(true);

            remainingChords.setVisible(false);
            doneChords3.setVisible(false);
            break;


        case 23:
            startChords2.setVisible(true); 
            playChords2.setVisible(true);


            doneChords4.setVisible(false);
            chordProgression2.setVisible(false);

            break;

        case 24:
            remainingChords2.setVisible(true);
            doneChords5.setVisible(true);


            startChords2.setVisible(false);
            playChords2.setVisible(false);


            break;


        case 25:
            practiceScaleCDescending.setVisible(true);
            currentNoteCDescending.setVisible(true);
            nextNoteCDescending.setVisible(true);
            buttonNoteCDescending.setVisible(true);
            doneScalesCDescending.setVisible(true);



            remainingNotesC.setVisible(false);
            FailedC.setVisible(false);
            SuceededC.setVisible(false);
            break; 


        case 26:
            playScalesCDescending.setVisible(true);
            startScalesCDescending.setVisible(true);


            practiceScaleCDescending.setVisible(false);
            currentNoteCDescending.setVisible(false);
            nextNoteCDescending.setVisible(false);
            buttonNoteCDescending.setVisible(false);
            doneScalesCDescending.setVisible(false);

            break;


        case 27:
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



        case 28:
            failedStringCDescending.setVisible(true);
            correctnoteWasCDescending.setVisible(true);
            remainingAttemptsCDescending.setVisible(true);
            tryAgainCDescending.setVisible(true);


            remainingNotesCDescending.setVisible(false);
            FailedCDescending.setVisible(false);
            SuceededCDescending.setVisible(false);
            break;


        case 29:


            break; 



        }
    }
    //Whenever a switch happens pageNum_OLD is overwrited to the value of pageNum.
    //Meaning that the program can then detect that a change in page number has happened.
    audioProcessor.pageNum_OLD = audioProcessor.pageNum;

    //If the program registers that a note has been played, it updates so that the currently played note
    //is not the same as the previously played note.

    //DBG(passedTime);
    //DBG(audioProcessor.pageNum);
    //DBG(failed3);

    //If a note has been updated, it sets the letter to update in real time as you play on the scale page.

    if (audioProcessor.playedNote != audioProcessor.playedNoteOLD && updateNote) {
        newNote = true;
        //audioProcessor.playedNoteOLD = oldNote;
        //DBG(oldNote);
    }
    else {
        newNote = false;
    }

    

    //DBG(passedTime);
    

    //audioProcessor.playedNoteOLD = audioProcessor.playedNote;
    
    if (audioProcessor.pageNum == 5) {
        stringNote = audioProcessor.playedNote;
        currentNote.setText("Currently playing " + stringNote, dontSendNotification);

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

        
        buttonNote.setButtonText(nextNoteArray);
            
    }

    if (audioProcessor.pageNum == 8 && audioProcessor.playedNote != " ") {
        
        timeThreshold = 1000;
         
        //DBG(timeThreshold);

        if (audioProcessor.playedNote == "A" && arrayCounter2 == 0) {
            remainingNotesA = "7";
            arrayCounter2 = arrayCounter2 + 1;
            oldNote = audioProcessor.playedNote;
        }
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

        remainingNotes.setText(stringtoTrim + remainingNotesA, dontSendNotification);

    }

    if (audioProcessor.pageNum == 10) {
        
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

    if (audioProcessor.pageNum == 12 && audioProcessor.playedNote != " ") {

        timeThreshold = 1000;

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
    //CHORDS-----------------------------------------------------------
    if (audioProcessor.pageNum == 6 && audioProcessor.playedNote != " ") {

        stringChord = audioProcessor.playedNote;
        

        if (chordNoteCounter==0 && stringChord != " ") {
            Triad_Chord_notes[0] = stringChord;
            chordNoteCounter = chordNoteCounter + 1;
            first_note = stringChord;
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
        }

        currentChord.setText("Currently playing: " + Triad_Chord_notes[0] + " - " + Triad_Chord_notes[1] + " - " + Triad_Chord_notes[2], dontSendNotification);

        String FullChord = Triad_Chord_notes[0] + Triad_Chord_notes[1] + Triad_Chord_notes[2];

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
        //Triad_Chord_notes.resize(3, chordnote);
        //DBG(Triad_Chord_notes[i]);
        
        //(Triad_Chord_notes.length());
        }
    
    //DBG(arrayCounter2);
    //Same logic as with the page number, this is just for playedNote
    //audioProcessor.playedNoteOLD = audioProcessor.playedNote;
}

//==============================================================================
void RandomNameAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    //g.drawFittedText ("Group 405!", getLocalBounds(), juce::Justification::centred, 1);
}

void RandomNameAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor.. this is where you set height, width etc. of the buttons
    title.setBounds(325, 10, 500, 40);
    newGame.setBounds(450, 400, 50, 50);
    howTo.setBounds(400, 300, 100, 50);
    title2.setBounds(325, 10, 500, 40);
    menu.setBounds(1000, 10, 100, 40);
    title3.setBounds(325, 10, 500, 40);
    chords.setBounds(500, 300, 100, 50);
    scales.setBounds(500, 400, 100, 50);
    title4.setBounds(500, 10, 500, 40);
    scalesTheory.setBounds(100, 10, 500, 700);
    tryScales.setBounds(850, 600, 100, 100);
    title5.setBounds(500, 10, 500, 40);
    chordsTheory.setBounds(100, 10, 500, 700);
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
    correctnoteWas.setBounds(350, 250, 500, 40);
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
    correctnoteWasDescending.setBounds(350, 250, 500, 40);
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
    correctnoteWasC.setBounds(350, 250, 500, 40);
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
    remainingChords.setBounds(300, 200, 600, 200);
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
    correctnoteWasCDescending.setBounds(350, 250, 500, 40);
    remainingAttemptsCDescending.setBounds(350, 450, 500, 40);
    tryAgainCDescending.setBounds(450, 550, 100, 100);
    nameOfChord.setBounds(450, 350, 100, 100);
}

