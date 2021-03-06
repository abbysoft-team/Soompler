/*
  ==============================================================================

    SampleInfo.h
    Created: 5 Jul 2019 2:41:38pm
    Author:  apopov

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"

/**
 * All required info about sample.
 * Convinient to message this info between objects.
 *
 * @brief The SampleInfo class
 */
class SampleInfo
{
public:

    SampleInfo(float lengthInSeconds, float sampleRate, String sampleName)
    {
        this->lengthInSeconds = lengthInSeconds;
        this->sampleRate = sampleRate;
        this->lengthInSamples = lengthInSeconds * sampleRate;
        this->startSample = 0;
        this->endSample = lengthInSamples;
        this->sampleName = sampleName;
        this->rootNote = Settings::DEFAULT_ROOT_NOTE;
        this->minNote = Settings::DEFAULT_MIN_NOTE;
        this->maxNote = Settings::DEFAULT_MAX_NOTE;
    }

    ~SampleInfo() = default;

    float sampleRate;
    float lengthInSeconds;
    int64 lengthInSamples;

    // this sample range marked as active, other samples would be not
    // played
    int64 startSample;
    int64 endSample;

    String sampleName;

    // root note and range for this sample
    int rootNote;
    int minNote;
    int maxNote;
};

/**
 * Interface for SampleInfo consumers
 *
 * @brief The SampleInfoListener class
 */
class SampleInfoListener
{
public:
    virtual void newSampleInfoRecieved(std::shared_ptr<SampleInfo> info) = 0;
    
    virtual ~SampleInfoListener() = default;
};
