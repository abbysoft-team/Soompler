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
#include "PianoRoll.h"

//==============================================================================
/**
*/
class SoomplerAudioProcessorEditor  : public AudioProcessorEditor,
        private Button::Listener, private TransportStateListener, private ChangeListener, private Timer,
        private Slider::Listener
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

    void mouseDrag(const MouseEvent &event);

    bool isIntersectWithStartRangeLine(Point<int>* point);
    bool isIntersectWithEndRangeLine(Point<int>* point);

    int64 calculateSampleByCoords(int coord);

    void sliderValueChanged(Slider *slider);

    void calculateEndRangeX();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoomplerAudioProcessor& processor;

    TextButton openFileButton;
    TextButton playSampleButton;
    TextButton stopSampleButton;
    Slider volumeKnob;
    Label volumeLabel;
    PianoRoll pianoRoll;

    Image backgroundImage;

    Font mainFont;

    // not active sample regions, before start line and after end lines
    int startRangeX;
    int endRangeX;

    // max range x value according to max sample length value
    int maxRangeX;

    // OpenGL context to speed up UI rendering
    OpenGLContext glContext;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoomplerAudioProcessorEditor)
};
