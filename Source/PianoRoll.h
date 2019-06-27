#pragma once

#include "../JuceLibraryCode/JuceHeader.h"



//==============================================================================
/**

*/
class PianoRoll  : public Component
{
public:
    PianoRoll ();
    ~PianoRoll();

    void paint (Graphics& g) override;
    void resized() override;



private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRoll)
};

