/*
  ==============================================================================

    SoomplerKnob.cpp
    Created: 18 Aug 2019 12:13:46pm
    Author:  MacPro

  ==============================================================================
*/

#include "SoomplerKnob.h"

SoomplerKnob::SoomplerKnob(const String &name) : Component(name), label(name, name) {
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

void SoomplerKnob::resized() {
    FlexBox fb;
    fb.flexDirection = FlexBox::Direction::column;
    
    fb.items.add(FlexItem(label).withFlex(1).withMargin(FlexItem::Margin(0, 0, 0, 7.0f)));
    fb.items.add(FlexItem(slider).withFlex(6).withMargin(FlexItem::Margin(-10.0f, 0, 0, 0)));
    
    fb.performLayout(getLocalBounds());
}

void SoomplerKnob::setPosition(int x, int y) {
    setBounds(x, y, 50, 60);
}

