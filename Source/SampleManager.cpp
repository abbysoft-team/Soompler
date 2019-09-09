/*
  ==============================================================================

    SampleManager.cpp
    Created: 9 Sep 2019 2:26:58pm
    Author:  artyom

  ==============================================================================
*/

#include "SampleManager.h"


SampleManager::SampleManager()
{

}

void SampleManager::sampleInfoChanged(std::shared_ptr<SampleInfo> info)
{
    activeSample = info;
    notifySampleInfoListeners();

    if (contains(info)) {
        return;
    }
    samples.push_back(info);

    // update change listener
    if (info->getThumbnail() == nullptr) {
        return;
    }
    for (auto listener : changeListeners) {
        info->getThumbnail()->addChangeListener(listener);
    }
}

void SampleManager::addSampleInfoListener(std::shared_ptr<SampleInfoListener> sampleInfoListener)
{
    sampleInfoListeners.push_back(sampleInfoListener);
}

bool SampleManager::contains(std::shared_ptr<SampleInfo> info) {
    for (auto sample : samples) {
        if (sample == info) {
            return true;
        }
    }

    return false;
}

void SampleManager::notifySampleInfoListeners()
{
    for (auto sampleInfoListener : sampleInfoListeners) {
        sampleInfoListener->sampleInfoChanged(activeSample);
    }
}

std::shared_ptr<SampleInfo> SampleManager::getActiveSample()
{
    return activeSample;
}

std::vector<std::shared_ptr<SampleInfo> > SampleManager::getAllSamples()
{
    return samples;
}

void SampleManager::addChangeListener(ChangeListener* listener)
{
    changeListeners.push_back(listener);
    if (activeSample != nullptr && activeSample->getThumbnail() != nullptr) {
        activeSample->getThumbnail()->addChangeListener(listener);
    }
}

void SampleManager::removeChangeListener(ChangeListener *listener)
{
    for (auto sample : samples) {
        if (sample->getThumbnail() != nullptr) {
            sample->getThumbnail()->removeChangeListener(listener);
        }
    }
}

