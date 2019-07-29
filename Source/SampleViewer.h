/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleInfo.h"
#include "TransportInfo.h"

/**
  Component with AudioThumbnail and range lines.

  Dont forget to set SampleInfoListener to be able to send updated sample start and end
  positions to some controller class.
*/
class SampleViewer  : public Component, public SampleInfoListener
{
public:
    SampleViewer (AudioThumbnail& thumbnail, TransportInfoOwner& transportInfoOwner, SampleInfoListener& sampleInfoListener);
    ~SampleViewer();

    void paint (Graphics& g) override;
    void resized() override;
    void newSampleInfoRecieved(std::shared_ptr<SampleInfo> info) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    AudioThumbnail& thumbnail;
    std::shared_ptr<SampleInfo> currentSample;
    TransportInfoOwner& transportInfoOwner;
    // this object can modify sample's start and end positions
    // with range lines, so some another object can use this new values
    SampleInfoListener& sampleInfoListener;

    // not active sample regions, before start line and after end lines
    int startRangeX;
    int endRangeX;

    // max range x value according to max sample length value
    int maxRangeX;

    //[/UserVariables]

    //==============================================================================
    void drawThumbnail(Graphics& g);
    void drawPositionLine(Graphics& g);
    int64 calculateSampleByCoords(int coordOnThumbnail);
    int calculateCoordBySample(int64 sample);
    void calculateEndRangeX();
    void notifySampleInfoListeners();
    String getCroppedNameIfNeeded();

    void mouseDrag(const MouseEvent &event) override;
    void mouseMove(const MouseEvent &event) override;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleViewer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

