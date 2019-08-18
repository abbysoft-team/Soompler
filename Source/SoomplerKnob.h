/*
  ==============================================================================

    SoomplerKnob.h
    Created: 18 Aug 2019 12:13:46pm
    Author:  MacPro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SoomplerSlider.h"

class SoomplerKnob : public Component {
    
public:
    SoomplerKnob(const String &name);
    ~SoomplerKnob() = default;
    
    void resized() override;
    
    void setPosition(int x, int y);
    
private:
    SoomplerSlider slider;
    Label label;
};
