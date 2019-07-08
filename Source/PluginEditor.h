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
#include "MainPanel.h"

//==============================================================================
/**
*/
class SoomplerAudioProcessorEditor  : public AudioProcessorEditor,
        private Button::Listener, private TransportStateListener, private ChangeListener, private Timer,
        private Slider::Listener
{
public:
    explicit SoomplerAudioProcessorEditor (SoomplerAudioProcessor&);
    ~SoomplerAudioProcessorEditor() override = default;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void buttonClicked(Button*) override;
    void openFileButtonClicked();
    void playSampleButtonClicked();
    void stopSampleButtonClicked();
    void transportStateChanged(TransportState state) override;

    void changeListenerCallback(ChangeBroadcaster* source) override ;
    void thumbnailChanged(AudioThumbnail& thumbnail);
    void drawSampleNameOrMessage(Graphics& graphics);

    void timerCallback() override;

    int64 calculateSampleByCoords(int coord);

    void sliderValueChanged(Slider *slider) override;

    void calculateEndRangeX();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoomplerAudioProcessor& processor;

    MainPanel mainPanel;

    Font mainFont;

    // OpenGL context to speed up UI rendering
    OpenGLContext glContext;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoomplerAudioProcessorEditor)
};
