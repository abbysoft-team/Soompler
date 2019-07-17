#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEventSupplier.h"
#include "MidiEventConsumer.h"
#include <array>

class KeyInfo
{
public:
    float x;
    float width;
    float height;
    bool isWhite;
    int number;

    bool contains(Point<int> point)
    {
        Rectangle<int> rect(x, 0, width, height);
        return rect.contains(point);
    }
};

//==============================================================================
/**

*/
class PianoRoll  : public Component
{
public:
    PianoRoll (MidiEventSupplier& midiSupplier, MidiEventConsumer& midiConsumer);
    ~PianoRoll();

    void paint (Graphics& g) override;
    void resized() override;

private:
    static constexpr auto MAX_KEYS = 120;
    static constexpr auto FIRST_VISIBLE_KEY = 48;

    MidiEventSupplier& midiSupplier;
    MidiEventConsumer& midiConsumer;

    std::vector<int> getActiveMidiNotes();
    void calculateKeysInfo();
    void drawActiveNotes(Graphics& g, std::vector<int> activeNotes);
    int getKeyClicked(Point<int> point);

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    void fireNoteOn(int noteNumber);
    void fireNoteOff(int noteNumber);

    // stores some metadata for keys
    std::array<KeyInfo, MAX_KEYS> keysInfo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRoll)
};

