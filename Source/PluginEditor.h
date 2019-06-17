/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300
#define BUTTON_OPEN_FILE_POSITION Rectangle<int>(160, 233, 100, 40)
#define SAMPLE_NAME_COLOR Colours::black
#define SAMPLE_NAME_TEXT_X (WINDOW_WIDTH / 2)
#define SAMPLE_NAME_TEXT_Y 120
#define SAMPLE_NAME_FONT_SIZE 17.0f

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SoomplerAudioProcessorEditor  : public AudioProcessorEditor, private Button::Listener
{
public:
    SoomplerAudioProcessorEditor (SoomplerAudioProcessor&);
    ~SoomplerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void buttonClicked(Button*) override;
    void openFileButtonClicked();
    void loadSample(File);
    String getLoadedSampleNameOrPlaceholder();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoomplerAudioProcessor& processor;

    TextButton openFileButton;

    File *loadedSample;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoomplerAudioProcessorEditor)
};
