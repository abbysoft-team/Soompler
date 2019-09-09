#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../viewer/SampleViewer.h"
#include "../PluginProcessor.h"
#include "PianoRoll.h"
#include "GuiEditor.h"
#include "ToggledImageButton.h"
#include "SoomplerSlider.h"
#include "SoomplerImageButton.h"
#include "SoomplerToggleButton.h"
#include "LinearPanel.h"
#include "SoomplerKnob.h"

class MainPanel  : public Component,
                   public Slider::Listener,
                   public Button::Listener,
                   public TransportStateListener,
                   public FileDragAndDropTarget,
                   private Timer

{
public:
    MainPanel (SoomplerAudioProcessor& processor);
    ~MainPanel();

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void transportStateChanged(TransportState state) override;

    float getVolume() const;

private:

    AudioProcessorValueTreeState& stateManager;
    
    std::unique_ptr<ButtonAttachment> loopAttachment;
    std::unique_ptr<ButtonAttachment> reverseAttachment;
    
    std::unique_ptr<SoomplerKnob> volumeKnob;
    std::unique_ptr<SoomplerKnob> attackKnob;
    std::unique_ptr<SoomplerKnob> decayKnob;
    std::unique_ptr<SoomplerKnob> sustainKnob;
    std::unique_ptr<SoomplerKnob> releaseKnob;
    
    std::unique_ptr<LinearPanel> adsrPanel;
    
    std::unique_ptr<Label> loadSampleTip;

    std::shared_ptr<SampleViewer> sampleViewer;
    std::shared_ptr<PianoRoll> pianoRoll;

    std::unique_ptr<SoomplerImageButton> openFileButton;
    std::unique_ptr<SoomplerImageButton> aboutButton;
    std::unique_ptr<ToggledImageButton> loopButton;

    std::unique_ptr<SoomplerToggleButton> reverseButton;
    
    ADSR::Parameters adsrParams;

    Image backgroundImage;

    SoomplerAudioProcessor& processor;

    // ability to edit components for this panel
    GuiEditor editor;

    bool sampleLoaded;

    void restoreMainPanelState();

    void openFileButtonClicked();
    void aboutButtonClicked();
    void playSampleButtonClicked();
    void stopSampleButtonClicked();
    void loopButtonClicked();
    void reverseButtonClicked();
    
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;
    
    void timerCallback() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

