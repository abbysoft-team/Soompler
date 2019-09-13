/*
  ==============================================================================

    Glide.cpp
    Created: 13 Sep 2019 1:47:08pm
    Author:  artyom

  ==============================================================================
*/

#include "Glide.h"

Glide::Glide() : currentNote(-1), prevNote(-1), level(0.0f), sampleRate(44100), samplesLeft(0)
{

}

float Glide::getGlide(int numSamples)
{
    if (samplesLeft <= 0 || prevNote == -1 || level == 0.0f) {
        return 1.0f;
    }

    auto relation = (currentNote - prevNote) / 12.0f ;

    if (relation == 0) {
        return 1.0f;
    }

    // calc glide
    auto slope = numSamples / (samplesLeft * 1.0f);

    if (slope >= 0.9) {
        slope = 1;
    }

    //
    DBG(samplesLeft);

    samplesLeft -= numSamples;

    return relation * slope;
}

void Glide::noteOn(int noteNumber)
{
    prevNote = currentNote;
    currentNote = noteNumber;

    samplesLeft = level * sampleRate;
}

void Glide::setLevel(float level)
{
    this->level = level;
}

void Glide::setSampleRate(float sampleRate)
{
    this->sampleRate = sampleRate;
}
