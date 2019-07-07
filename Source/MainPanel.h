#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleViewer.h"
#include "PluginProcessor.h"
#include "PianoRoll.h"

class MainPanel  : public Component,
                   public Slider::Listener,
                   public Button::Listener
{
public:
    MainPanel (SoomplerAudioProcessor& processor);
    ~MainPanel();

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

private:
    std::unique_ptr<Slider> volumeKnob;
    std::unique_ptr<Label> volumeKnobLabel;
    std::unique_ptr<ImageButton> openFileButton;
    std::unique_ptr<ImageButton> aboutButton;
    std::unique_ptr<SampleViewer> sampleViewer;
    std::unique_ptr<Label> label;
    std::unique_ptr<PianoRoll> pianoRoll;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

