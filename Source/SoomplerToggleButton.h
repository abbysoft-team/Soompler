/*
  ==============================================================================

    SoomplerToggleButton.h
    Created: 29 Jul 2019 8:37:19pm
    Author:  artyom

  ==============================================================================
*/
#pragma once

#include <../JuceLibraryCode/JuceHeader.h>

class SoomplerToggleButton : public ToggleButton {
public:

    SoomplerToggleButton(String buttonName);
    ~SoomplerToggleButton() = default;

protected:

    void mouseMove(const MouseEvent &event) override;
};
