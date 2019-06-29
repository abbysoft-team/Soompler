#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEventSupplier.h"

struct KeyInfo
{
    float x;
    float width;
    float height;
    bool isWhite;
    int number;
};

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
    static constexpr auto MAX_KEYS = 120;

    MidiEventSupplier& midiSupplier;

    std::vector<int> getActiveMidiNotes();
    void calculateKeysInfo();
    void drawActiveNotes(Graphics& g, std::vector<int> activeNotes);

    // stores some metadata for keys
    KeyInfo keysInfo[MAX_KEYS];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRoll)
};

