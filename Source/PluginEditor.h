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
class SoomplerAudioProcessorEditor  : public AudioProcessorEditor,
        private Button::Listener, private TransportStateListener, private ChangeListener, private Timer
{
public:
    explicit SoomplerAudioProcessorEditor (SoomplerAudioProcessor&);
    ~SoomplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void buttonClicked(Button*) override;
    void openFileButtonClicked();
    void playSampleButtonClicked();
    void stopSampleButtonClicked();
    String getLoadedSampleNameOrPlaceholder();
    void transportStateChanged(TransportState state) override;

    void changeListenerCallback(ChangeBroadcaster* source) override ;
    void thumbnailChanged(AudioThumbnail& thumbnail);
    void drawThumbnail(Graphics& graphics);
    void drawSampleNameOrMessage(Graphics& graphics);

    void timerCallback() override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoomplerAudioProcessor& processor;

    TextButton openFileButton;
    TextButton playSampleButton;
    TextButton stopSampleButton;

    Image backgroundImage;

    Font mainFont;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoomplerAudioProcessorEditor)
};
