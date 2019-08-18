/*
  ==============================================================================

    LinearPanel.h
    Created: 16 Aug 2019 8:01:25pm
    Author:  MacPro

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum Orientation {
    VERTICAL,
    HORIZONTAL
};

class LinearPanel : public Component {
public:
    LinearPanel(Orientation orientation);
    ~LinearPanel() = default;
    
    void resized() override;
    
    void paint(Graphics &g);
    
private:
    Orientation orientation;
};
