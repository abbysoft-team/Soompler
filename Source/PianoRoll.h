#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiEventSupplier.h"
#include "MidiEventConsumer.h"
#include <array>
#include "SampleInfo.h"

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
        Rectangle<int> rect(x, Settings::PIANO_ROLL_RANGE_MARKERS_HEIGHT, width, height);
        return rect.contains(point);
    }
};

enum MarkerType
{
    NO_MARKER,
    MIN_NOTE,
    MAX_NOTE,
    ROOT_NOTE
};

//==============================================================================
/**

*/
class PianoRoll  : public Component, public SampleInfoListener
{
public:
    PianoRoll (MidiEventSupplier& midiSupplier, MidiEventConsumer& midiConsumer);
    ~PianoRoll();

    void paint (Graphics& g) override;
    void resized() override;

    void newSampleInfoRecieved(std::shared_ptr<SampleInfo> info) override;

private:
    static constexpr auto MAX_KEYS = 120;
    static constexpr auto FIRST_VISIBLE_KEY = 48;

    MidiEventSupplier& midiSupplier;
    MidiEventConsumer& midiConsumer;

    // stores some metadata for keys
    std::array<KeyInfo, MAX_KEYS> keysInfo;

    std::shared_ptr<SampleInfo> sample;

    Path rootMarker;
    Path minMarker;
    Path maxMarker;

    // Which marker is dragged now
    MarkerType draggedMarker;

    std::vector<int> getActiveMidiNotes();
    void calculateKeysInfo();
    void drawActiveNotes(Graphics& g, std::vector<int> activeNotes);
    int getKeyClicked(Point<int> point);

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;
    void mouseMove(const MouseEvent& event) override;

    void fireNoteOn(int noteNumber);
    void fireNoteOff(int noteNumber);

    void drawNoteRangeAndRoot(Graphics& g);
    void createMarkers(std::shared_ptr<SampleInfo> info);
    Path createMarker(int noteNum, bool root);

    void drawDisabledNotesMask(Graphics& g);

    void rootMarkerDragged(Point<int> position);
    void minMarkerDragged(Point<int> position);
    void maxMarkerDragged(Point<int> position);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRoll)

};

