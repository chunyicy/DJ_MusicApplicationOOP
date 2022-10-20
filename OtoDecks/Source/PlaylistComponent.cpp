/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 3 Mar 2021 12:17:19pm
    Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include <string>
#include <fstream>
#include <sstream>



//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager, DJAudioPlayer* _player1,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    :formatManagers(_formatManager), player1(_player1), waveformDisplay(formatManagerToUse, cacheToUse)

{

    ////string vector is created to store the track titles
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");
    trackTitles.push_back("Track 7");
    trackTitles.push_back("Track 8");
    trackTitles.push_back("Track 9");
    trackTitles.push_back("Track 10");

    // vector of totalLength is created to store the duration of the song
    totalLength.push_back("Duration 1");
    totalLength.push_back("Duration 2");
    totalLength.push_back("Duration 3");
    totalLength.push_back("Duration 4");
    totalLength.push_back("Duration 5");
    totalLength.push_back("Duration 6");
    totalLength.push_back("Duration 7");
    totalLength.push_back("Duration 8");
    totalLength.push_back("Duration 9");
    totalLength.push_back("Duration 10");


    // add column to table component
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Duration (Seconds)", 2, 200);
    tableComponent.getHeader().addColumn("Add Files", 3, 200);


    addAndMakeVisible(tableComponent);
    addAndMakeVisible(textEditor);
    tableComponent.setModel(this);
    textEditor.addListener(this);

    //Reading the name of the song from the "musicLibrary" text file
    std::string myLibrary;
    std::ifstream MyReadFile("musicLibrary.txt");
    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, myLibrary)) 
    {
        trackTitles.push_back(myLibrary); 
    }
    MyReadFile.close();

    //Reading the duration of the song from the "musicDuration" text file
    std::string mymusicDuration;
    std::ifstream MyFile("musicDuration.txt");
    while (getline(MyFile, mymusicDuration)) //read the file line by line
    {
        totalLength.push_back(mymusicDuration);
    }
    MyFile.close();// Close the file
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll (juce::Colours::black); // adding black as the background colour to the playlist component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::mediumpurple);
    g.setFont (16.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centredBottom, true);   // draw some placeholder text
    
    // adding colours to the text editor box
    getLookAndFeel().setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::TextEditor::textColourId, juce::Colours::darkorange);
    getLookAndFeel().setColour(juce::TextEditor::outlineColourId, juce::Colours::lightslategrey);
    textEditor.setText("This is a search box: ");
    // adding the lightslategrey colour to the table header
    getLookAndFeel().setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colours::lightslategrey);

}


void PlaylistComponent::resized()
{
    double rowH = getHeight() / 7;
    tableComponent.setBounds(0, 0, getWidth(), rowH *6);
    textEditor.setBounds(getWidth() / 4, 0, getWidth()/4, (rowH/3)*2);
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent ::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else {
        g.fillAll(juce::Colours::mediumpurple);
    }
}

void PlaylistComponent ::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (columnId == 1)
    {
        // draw text to the track title based on it's rowNumber
        g.drawText(trackTitles[rowNumber],
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft, true);
    }
    
    if (columnId == 2)
    {
        //draw duration text
        g.drawText(totalLength[rowNumber],
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft, true);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
   

    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            //creating a add button for Adding file
            juce::TextButton* btn = new juce::TextButton{ "Add" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id); // set component id to the button
            btn->addListener(this); // adding listener to the button
            existingComponentToUpdate = btn;

        }
    }
    return existingComponentToUpdate; 
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("PlaylistComponent::buttonClicked " << trackTitles[id] );

        //browse for file to open
        juce::FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())    
        {
            //player1 load URL into deck1 to get the song to play
            player1->loadURL(juce::URL{ chooser.getResult() });   
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
            

            //getting the filename
            juce::File filename(chooser.getResult());
            juce::String name = filename.getFileName();
            std::string file = name.toStdString();
            trackTitles[id].assign(file); // assinging the filename to trackTitles[id]

            //getting the total duration of the song in seconds
            std::unique_ptr<juce::AudioFormatReader> reader(formatManagers.createReaderFor(filename));
            auto duration = (float)reader->lengthInSamples / reader->sampleRate;
            std::string durations = std::to_string(duration);
            totalLength[id].assign(durations); //assinging duration
            

            DBG("PlaylistComponent::buttonClicked "   << filename.getFileName());
            DBG("PlaylistComponent::buttonClicked " << duration << " seconds");

            // writing to musicLibrary text file 
            std::ofstream musicLibrary;
            musicLibrary.open("musicLibrary.txt", std::ofstream::out | std::ofstream::app);
            musicLibrary << filename.getFileName() << "\n";
            musicLibrary.close();

            // writing the duration of the song to musicDuration text file 
            std::ofstream musicDuration;
            musicDuration.open("musicDuration.txt", std::ofstream::out | std::ofstream::app);
            musicDuration << duration<< "\n";
            musicDuration.close();
            

            
        }
}
    
void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& )
{
    // get text from the text editor 
    juce::String get_text = textEditor.getText();
    std::string getText = get_text.toStdString();

    //search for song name.mp3 inside the musicLibrary.txt
    std::ifstream file("musicLibrary.txt");
    std::string str;
    while (std::getline(file, str)) {
        if (getText == str)
        {
            // print out a message "found" if the song is found
            DBG("found");
            break;
        }
    }
}

