/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "TransportStateListener.h"
#include "Settings.h"

//==============================================================================
/**
*/
class SoomplerAudioProcessorEditor  : public AudioProcessorEditor, private Button::Listener, private TransportStateListener
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
    void playSampleButtonClicked();
    void stopSampleButtonClicked();
    String getLoadedSampleNameOrPlaceholder();
    void transportStateChanged(TransportState state);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoomplerAudioProcessor& processor;

    TextButton openFileButton;
    TextButton playSampleButton;
    TextButton stopSampleButton;

    Image backgroundImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoomplerAudioProcessorEditor)
};
