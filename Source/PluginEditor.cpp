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
    howTo.setColour(newGame.buttonColourId, Colours::green);
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
    chords.onClick = [this] {
        audioProcessor.pageNum = 3;
    };
    //Scales button
    addAndMakeVisible(scales);
    scales.setVisible(false);
    scales.setButtonText("Scales");
    scales.changeWidthToFitText();
    scales.onClick = [this] {
        audioProcessor.pageNum = 4;
    };
    //Harmony is commented out, because of Prithvis feedback

    /*addAndMakeVisible(harmony);
    harmony.setVisible(false);
    harmony.setButtonText("Harmony");
    harmony.changeWidthToFitText();
    harmony.onClick = [this] {
        audioProcessor.pageNum = 5;
    };*/
}

void RandomNameAudioProcessorEditor::timerCallback()
//The page counter/mechanism that changes the page.
//If the current page number as not the same as the old page number, then change the page.
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
            harmony.setVisible(false);

            


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
            harmony.setVisible(true);

            title.setVisible(false);
            title2.setVisible(false);
            howTo.setVisible(false);
            newGame.setVisible(false);

            break;

        case 3:

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
    chords.setBounds(400, 300, 100, 50);
    scales.setBounds(400, 400, 100, 50);
    harmony.setBounds(400, 500, 100, 50);
}
