#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleViewer.h"
#include "PluginProcessor.h"
#include "PianoRoll.h"

class MainPanel  : public Component,
                   public Slider::Listener,
                   public Button::Listener,
                   public TransportStateListener

{
public:
    MainPanel (SoomplerAudioProcessor& processor);
    ~MainPanel();

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    float getVolume() const;

private:
    std::unique_ptr<Slider> volumeKnob;
    std::unique_ptr<Label> volumeKnobLabel;
    std::unique_ptr<ImageButton> openFileButton;
    std::unique_ptr<ImageButton> aboutButton;
    std::unique_ptr<SampleViewer> sampleViewer;
    std::unique_ptr<Label> loadSampleTip;
    std::unique_ptr<PianoRoll> pianoRoll;

    Image backgroundImage;

    SoomplerAudioProcessor& processor;

    void openFileButtonClicked();
    void aboutButtonClicked();

    void transportStateChanged(TransportState state);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

