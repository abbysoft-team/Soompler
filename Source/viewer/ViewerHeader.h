/*
  ==============================================================================

    ViewerHeader.h
    Created: 9 Sep 2019 1:53:02pm
    Author:  artyom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../SampleInfo.h"

class ViewerHeader : public Component, public SampleInfoListener {

public:

    ViewerHeader();
    ~ViewerHeader() = default;

    void paint(Graphics &g) override;
    void newSampleInfoRecieved(std::shared_ptr<SampleInfo> info) override;

private:
    std::vector<std::shared_ptr<SampleInfo>> samples;
    String getCroppedNameIfNeeded();

};
