/*
  ==============================================================================

    SampleManager.h
    Created: 9 Sep 2019 2:26:58pm
    Author:  artyom

  ==============================================================================
*/

#pragma once

#include "SampleInfo.h"

class SampleManager : public SampleChangeListener {

public:
    SampleManager();
    ~SampleManager() = default;

    void sampleChanged(std::shared_ptr<SampleInfo> info) override;
    void addSampleInfoListener(std::shared_ptr<SampleChangeListener> sampleInfoListener);
    void notifySampleInfoListeners();
    std::shared_ptr<SampleInfo> getActiveSample();
    std::vector<std::shared_ptr<SampleInfo>> getAllSamples();
    void addChangeListener(ChangeListener* listener);
    void removeChangeListener(ChangeListener* listener);

private:
    std::vector<std::shared_ptr<SampleInfo>> samples;
    std::shared_ptr<SampleInfo> activeSample;
    std::vector<std::shared_ptr<SampleChangeListener>> sampleInfoListeners;
    std::vector<ChangeListener*> changeListeners;

    bool contains(std::shared_ptr<SampleInfo> sample);
};
