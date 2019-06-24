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
        private Button::Listener, private TransportStateListener, private ChangeListener, private Timer, private MouseListener,
        private Slider::Listener
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

    void changeListenerCallback(ChangeBroadcaster* source);
    void thumbnailChanged(AudioThumbnail& thumbnail);
    void drawThumbnail(Graphics& graphics);
    void drawSampleNameOrMessage(Graphics& graphics);

    String getCroppedNameIfNeeded(String fileName);

    void timerCallback();

    void mouseDrag(const MouseEvent &event);

    bool isIntersectWithStartRangeLine(Point<int>* point);
    bool isIntersectWithEndRangeLine(Point<int>* point);

    int64 calculateSampleByCoords(int coord);

    void sliderValueChanged(Slider *slider);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoomplerAudioProcessor& processor;

    TextButton openFileButton;
    TextButton playSampleButton;
    TextButton stopSampleButton;
    Slider volumeKnob;
    Label volumeLabel;

    Image backgroundImage;

    Font mainFont;

    // not active sample regions, before start line and after end lines
    int startRangeX;
    int endRangeX;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoomplerAudioProcessorEditor)
};
