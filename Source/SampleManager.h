/*
  ==============================================================================

    SampleManager.h
    Created: 9 Sep 2019 2:26:58pm
    Author:  artyom

  ==============================================================================
*/

#pragma once

#include "SampleInfo.h"

class SampleManager : public SampleInfoListener {

public:
    SampleManager();
    ~SampleManager() = default;

    void sampleInfoChanged(std::shared_ptr<SampleInfo> info) override;
    void addSampleInfoListener(std::shared_ptr<SampleInfoListener> sampleInfoListener);
    void notifySampleInfoListeners();
    std::shared_ptr<SampleInfo> getActiveSample();
    std::vector<std::shared_ptr<SampleInfo>> getAllSamples();

private:
    std::vector<std::shared_ptr<SampleInfo>> samples;
    std::shared_ptr<SampleInfo> activeSample;
    std::vector<std::shared_ptr<SampleInfoListener>> sampleInfoListeners;

    bool contains(std::shared_ptr<SampleInfo> sample);
};
