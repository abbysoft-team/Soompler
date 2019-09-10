/*
  ==============================================================================

    AdsrPanel.cpp
    Created: 10 Sep 2019 12:14:30pm
    Author:  artyom

  ==============================================================================
*/

#include "AdsrPanel.h"

AdsrPanel::AdsrPanel(AudioProcessorValueTreeState& state) : LinearPanel(Orientation::HORIZONTAL, "ADSR")
{
    // ADSR controls
    attackKnob.reset (new SoomplerKnob ("Attack"));
    attackKnob->attachTo("attack", state);

    decayKnob.reset (new SoomplerKnob ("Decay"));
    decayKnob->attachTo("decay", state);

    sustainKnob.reset (new SoomplerKnob ("Sustain"));
    sustainKnob->attachTo("sustain", state);

    releaseKnob.reset (new SoomplerKnob ("Release"));
    releaseKnob->attachTo("release", state);

    addAndMakeVisible(attackKnob.get());
    addAndMakeVisible(decayKnob.get());
    addAndMakeVisible(sustainKnob.get());
    addAndMakeVisible(releaseKnob.get());

    attackKnob->addListener(this);
    decayKnob->addListener(this);
    sustainKnob->addListener(this);
    releaseKnob->addListener(this);
}

void AdsrPanel::sliderValueChanged(Slider *slider)
{
    adsrParams.attack = attackKnob->getValue();
    adsrParams.decay = decayKnob->getValue();
    adsrParams.sustain = sustainKnob->getValue();
    adsrParams.release = releaseKnob->getValue();
}
