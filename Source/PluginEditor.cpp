/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    title.setText("P4 - Amazing digital synthesizer",dontSendNotification);
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
    //stringNote skal �ndre sig alt efter hvilken note der bliver spillet 
    currentNote.setText("Currently playing " + stringNote, dontSendNotification);
    currentNote.setFont(40);

    //teksten skal �ndre sig alt efter hvilken note man spiller
    addAndMakeVisible(nextNote);
    nextNote.setVisible(false);
    nextNote.setText("Next note:", dontSendNotification);
    nextNote.setFont(40);
    addAndMakeVisible(buttonNote);
    buttonNote.setVisible(false);
    buttonNote.setButtonText(buttonTextNote);
    buttonNote.changeWidthToFitText();
    buttonNote.setColour(chords.buttonColourId, Colours::dodgerblue);
    buttonNote.onClick = [this] {
        if (buttonNoteInt == 0);
        {
            buttonTextNote = "b";
            buttonNote.setButtonText(buttonTextNote);
            buttonNoteInt = 1;
        } 
    };

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
    auto str = String(remainingNotesInt);
    remainingNotes.setVisible(false);
    remainingNotes.setText("Remaining Notes " + str, dontSendNotification);
    remainingNotes.setFont(40);

    //DET HER SKAL IKKE V�RE EN KNAP MEN EN TRIGGER N�R FAILED SKER SKAL DEN G�RE DET HER
    addAndMakeVisible(Failed);
    Failed.setVisible(false);
    Failed.setButtonText("Failed");
    Failed.setColour(Failed.buttonColourId, Colours::deepskyblue);
    Failed.changeWidthToFitText();
    Failed.onClick = [this] {

        audioProcessor.pageNum = 9;

    };

    //DET HER SKAL IKKE V�RE EN KNAP MEN EN TRIGGER N�R remainingnotes n�r 0 SKER SKAL DEN G�RE DET HER
    addAndMakeVisible(Suceeded);
    Suceeded.setVisible(false);
    Suceeded.setButtonText("Suceeded");
    Suceeded.setColour(Suceeded.buttonColourId, Colours::deepskyblue);
    Suceeded.changeWidthToFitText();
    Suceeded.onClick = [this] {
        audioProcessor.pageNum = 10;
    };

    //her skal failed string �ndres til den note man spillede forkert
    addAndMakeVisible(failedString);
    auto failed = String(wrongNote);
    failedString.setVisible(false);
    failedString.setText("Try again! you played " + failed, dontSendNotification);
    failedString.setFont(40);

    //her skal failed 2 �ndres til den note der havde v�ret rigtig at spille eller bare fjernes hvis det er for besv�rligt
    addAndMakeVisible(correctnoteWas);
    auto failed2 = String(correctNote);
    correctnoteWas.setVisible(false);
    correctnoteWas.setText("The correct note was " + failed2, dontSendNotification);
    correctnoteWas.setFont(40);

    //failed 3 skal g� �n ned n�r man f�iler
    addAndMakeVisible(remainingAttempts);
    auto failed3 = String(attemptsRemaining);
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
}

void RandomNameAudioProcessorEditor::timerCallback()
//The page counter/mechanism that changes the page.
//If the current page number is not the same as the old page number, then change the page.
{
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
            practiceScale.setVisible(true);
            currentNote.setVisible(true);
            nextNote.setVisible(true);
            buttonNote.setVisible(true);
            doneScales1.setVisible(true);

            title4.setVisible(false);
            scalesTheory.setVisible(false);
            tryScales.setVisible(false);



            break;


        case 6: 

            title5.setVisible(false);
            chordsTheory.setVisible(false);
            tryChords.setVisible(false);

            break;


        case 7:
            startScales.setVisible(true);
            playScales.setVisible(true);



            practiceScale.setVisible(false);
            currentNote.setVisible(false);
            nextNote.setVisible(false);
            buttonNote.setVisible(false);
            doneScales1.setVisible(false);


            break;

        case 8:

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
            failedString.setVisible(true);
            correctnoteWas.setVisible(true);
            remainingAttempts.setVisible(true);
            tryAgain.setVisible(true);
            
        
            
            remainingNotes.setVisible(false);
            Failed.setVisible(false);
            Suceeded.setVisible(false);
            break;


        case 10:

             
            break;


        case 11:


            break;

        }
    }
    //Whenever a switch happens pageNum_OLD is overwrited to the value of pageNum.
    //Meaning that the program can then detect that a change in page number has happened.
    audioProcessor.pageNum_OLD = audioProcessor.pageNum;
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
    remainingNotes.setBounds(300, 300, 500, 40);
    Suceeded.setBounds(850, 600, 100, 100);
    Failed.setBounds(100, 600, 100, 100);
    failedString.setBounds(350, 200, 500, 40);
    correctnoteWas.setBounds(350, 250, 500, 40);
    remainingAttempts.setBounds(350, 450, 500, 40);
    tryAgain.setBounds(450, 550, 100, 100);
}

