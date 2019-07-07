/*
  ==============================================================================

    TransportInfo.h
    Created: 5 Jul 2019 3:35:42pm
    Author:  apopov

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/**
 * Transport related information
 *
 * @brief The TransportInfo class
 */
class TransportInfo
{
public:

    TransportInfo(int sampleRate) : audioPositionSec(0), sampleRate(sampleRate)
    {

    }

    ~TransportInfo() = default;

    float getAudioPositionInSec() const
    {
        return audioPositionSec;
    }

    float getAudioPositionSample() const
    {
        return audioPositionSec * sampleRate;
    }

    void setSampleRate(int sampleRate)
    {
        this->sampleRate = sampleRate;
    }

    void setAudioPosition(float audioPositionSec)
    {
        this->audioPositionSec = audioPositionSec;
    }

private:

    float audioPositionSec;
    float sampleRate;
};

/**
 * Object that has access to TransportInfo
 *
 * @brief The TransportInfoOwner class
 */
class TransportInfoOwner
{
public:
    virtual std::shared_ptr<TransportInfo> getTransportInfo() = 0;
};
