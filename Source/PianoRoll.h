#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEventSupplier.h"


//==============================================================================
/**

*/
class PianoRoll  : public Component
{
public:
    PianoRoll (MidiEventSupplier& midiSupplier);
    ~PianoRoll();

    void paint (Graphics& g) override;
    void resized() override;

private:
    MidiEventSupplier& midiSupplier;

    std::vector<int> getActiveMidiNotes();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRoll)
};

