/*
  ==============================================================================

    DeckGUI.cpp
    Created: 2 Mar 2021 5:23:24am
    Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse) : player(_player),
                                             waveformDisplay(formatManagerToUse, cacheToUse)
{
    //addAndMakeVisble to display the buttons, sliders and waveform onto deck
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);

    //adding text label  
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);


    //adding a listener to the buttons, sliders 
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    restartButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    //setting range for the sliders
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll(juce::Colours::black); // color the background of the DeckGUI to black

    g.setColour (juce::Colours::grey); // set the outline to grey
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setFont (18.0f); // set font size
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text 

    //add colour the slider
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::mediumpurple);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::white);
    //add colour the button
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::TextButton::textColourOffId, juce::Colours::white);

}

void DeckGUI::resized()
{
    // set the size and position of the buttons, sliders and waveform
    double rowH = getHeight() / 9;
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    volSlider.setBounds(60, rowH * 2, getWidth() - 60, rowH);
    speedSlider.setBounds(60, rowH * 3, getWidth() - 60, rowH);
    posSlider.setBounds(60, rowH * 4, getWidth() - 60, rowH);
    playButton.setBounds(0, rowH*5, getWidth(), rowH);
    stopButton.setBounds(0, rowH*6, getWidth(), rowH);
    restartButton.setBounds(0, rowH*7, getWidth(), rowH);
    loadButton.setBounds(0, rowH * 8, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        player->start();
    }
    if (button == &stopButton)
    {
        DBG("Stop button was clicked");
        player->stop();
    }

    if (button == &loadButton)
    {
        // click on the load button to load a file
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{ chooser.getResult() });
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
        }
    }
    // create a restart button to restart the song
    if (button == &restartButton)  
    {
        DBG("Restart button was clicked");

        player->setPosition(0);
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider) 
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider) 
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    // to allow dragging a file onto the deck GUI
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}
void DeckGUI::timerCallback()
{
   // DBG("DeckGUI::timerCallback()");
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}