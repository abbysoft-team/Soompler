/*
  ==============================================================================

    SoomplerSlider.cpp
    Created: 29 Jul 2019 8:23:24pm
    Author:  artyom

  ==============================================================================
*/

#include "SoomplerSlider.h"

SoomplerSlider::SoomplerSlider(String name) : Slider(name)
{
}

void SoomplerSlider::mouseMove(const MouseEvent &event)
{
    Slider::mouseMove(event);

    setMouseCursor(MouseCursor::PointingHandCursor);
}
