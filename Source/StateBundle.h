/*
  ==============================================================================

    StateBundle.h
    Created: 14 Aug 2019 12:54:29pm
    Author:  artyom

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class StateBundle {

public:
    StateBundle(ValueTree& stateRoot);
    ~StateBundle() = default;

    void addProperty(var property, const String &name);
    const var& getProperty(const String &name);
    bool hasProperty(const String &name);

private:
    ValueTree& stateRoot;
};
