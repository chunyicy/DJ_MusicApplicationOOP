/*
  ==============================================================================

    PlaylistComponent.h
    Created: 3 Mar 2021 12:17:19pm
    Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener

{
public:
    PlaylistComponent(juce::AudioFormatManager& _formatManager, DJAudioPlayer* player1,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics&, 
        int rowNumber, 
        int width, 
        int height, 
        bool rowIsSelected) override;

    void paintCell(juce::Graphics&, 
        int rowNumber, 
        int columnId, 
        int width, 
        int height, 
        bool rowIsSelected) override;
    
    Component* refreshComponentForCell(int rowNumber, 
        int columnId, 
        bool isRowSelected, 
        Component* existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;

    /** implement the text editor return key pressed*/
    void textEditorReturnKeyPressed(juce::TextEditor&);


private:

    juce::TableListBox tableComponent;
    juce::TextEditor textEditor;

    // creating string vectors
    std::vector<std::string> trackTitles;
    std::vector<std::string> totalLength;
    std::vector<std::string> deck;

    juce::AudioFormatManager& formatManagers;
    DJAudioPlayer* player1;
    WaveformDisplay waveformDisplay;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
