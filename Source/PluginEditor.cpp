/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Strings.h"

//==============================================================================
SoomplerAudioProcessorEditor::SoomplerAudioProcessorEditor (SoomplerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), loadedSample(nullptr)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (WINDOW_WIDTH, WINDOW_HEIGHT);

    // init components state
    openFileButton.setButtonText(BUTTON_OPEN_FILE_TEXT);
    openFileButton.addListener(this);

    addAndMakeVisible(openFileButton);
}

SoomplerAudioProcessorEditor::~SoomplerAudioProcessorEditor()
{
}

//==============================================================================
void SoomplerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour(SAMPLE_NAME_COLOR);
    g.setFont(SAMPLE_NAME_FONT_SIZE);

    g.drawSingleLineText(getLoadedSampleNameOrPlaceholder(), SAMPLE_NAME_TEXT_X, SAMPLE_NAME_TEXT_Y, Justification::horizontallyCentred);
}

void SoomplerAudioProcessorEditor::resized()
{
    openFileButton.setBounds(BUTTON_OPEN_FILE_POSITION);
}

void SoomplerAudioProcessorEditor::buttonClicked(Button *button)
{
    if (button->getButtonText().equalsIgnoreCase(BUTTON_OPEN_FILE_TEXT)) {
        openFileButtonClicked();
    }
}

void SoomplerAudioProcessorEditor::openFileButtonClicked()
{
    FileChooser chooser(OPEN_FILE_DIALOG_TEXT,
                        File::getSpecialLocation(File::userHomeDirectory),
                        "*.mp3,*.wav,*.ogg");

    if (chooser.browseForFileToOpen())
    {
        File sampleFile (chooser.getResult());

        loadSample(sampleFile);
    }
}

void SoomplerAudioProcessorEditor::loadSample(File sampleFile)
{
    this->loadedSample = new File(sampleFile);
    repaint();
}

String SoomplerAudioProcessorEditor::getLoadedSampleNameOrPlaceholder()
{
    if (loadedSample == nullptr) {
        return NO_SAMPLE_LOADED_TEXT;
    } else {
        return loadedSample->getFileName();
    }
}
