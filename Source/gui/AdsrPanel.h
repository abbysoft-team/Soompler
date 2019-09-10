/*
  ==============================================================================

    AdsrPanel.h
    Created: 10 Sep 2019 12:14:30pm
    Author:  artyom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LinearPanel.h"
#include "SoomplerKnob.h"

class AdsrPanel : public LinearPanel, public Slider::Listener {

public:
    AdsrPanel(AudioProcessorValueTreeState& state);
    ~AdsrPanel() = default;

    void sliderValueChanged(Slider *slider) override;

private:
    std::unique_ptr<SoomplerKnob> attackKnob;
    std::unique_ptr<SoomplerKnob> decayKnob;
    std::unique_ptr<SoomplerKnob> sustainKnob;
    std::unique_ptr<SoomplerKnob> releaseKnob;

    ADSR::Parameters adsrParams;
};
