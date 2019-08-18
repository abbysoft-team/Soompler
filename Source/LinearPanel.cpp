/*
  ==============================================================================

    LinearPanel.cpp
    Created: 16 Aug 2019 8:01:25pm
    Author:  MacPro

  ==============================================================================
*/

#include "LinearPanel.h"
#include "Settings.h"

LinearPanel::LinearPanel(Orientation orientation) : Component(), orientation(orientation) {
    
}

void LinearPanel::resized() {
    
    FlexBox fb;
    
    if (orientation == HORIZONTAL) {
        fb.flexDirection = FlexBox::Direction::row;
    } else {
        fb.flexDirection = FlexBox::Direction::column;
    }
    
    for (auto child : getChildren()) {
        fb.items.add(FlexItem(*child).withMinWidth(child->getWidth()).withMinHeight(child->getHeight()));;
    }
    
    fb.performLayout(getLocalBounds());
}

void LinearPanel::paint(Graphics &g) {
    g.setColour(Settings::PANEL_BACKGROUND_COLOR);
    g.fillRect(getLocalBounds());
}
