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
    setSize (400, 300);
    configGUI();
    startTimerHz(frameRate);
}

RandomNameAudioProcessorEditor::~RandomNameAudioProcessorEditor()
{
}
void RandomNameAudioProcessorEditor::configGUI() {
    addAndMakeVisible(title);
    title.setText("main menu",dontSendNotification);
    addAndMakeVisible(newGame);
    newGame.setButtonText("new game");
    newGame.onClick = [this]
    {
        audioProcessor.pageNum = 1;
    };
}

void RandomNameAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.pageNum != audioProcessor.pageNum_OLD)
    {
        isPageChanged = true;
    }
    else
        isPageChanged = false;

    if (isPageChanged)
    {
        switch (audioProcessor.pageNum)
        {
        case 0:

            break;
        case 1:
            title.setVisible(false);
            newGame.setVisible(false);
            break;
        case 2:
            break;
        }
    }

    audioProcessor.pageNum_OLD = audioProcessor.pageNum;
}

//==============================================================================
void RandomNameAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Group 405!", getLocalBounds(), juce::Justification::centred, 1);
}

void RandomNameAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    title.setBounds(200, 10, 100, 20);
    newGame.setBounds(200, 60, 50, 50);
}
