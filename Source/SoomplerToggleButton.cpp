/*
  ==============================================================================

    SoomplerToggleButton.cpp
    Created: 29 Jul 2019 8:37:19pm
    Author:  artyom

  ==============================================================================
*/

#include "SoomplerToggleButton.h"

SoomplerToggleButton::SoomplerToggleButton(String buttonName) : ToggleButton(buttonName)
{

}

void SoomplerToggleButton::mouseMove(const MouseEvent &event)
{
    ToggleButton::mouseMove(event);

    setMouseCursor(MouseCursor::PointingHandCursor);
}
