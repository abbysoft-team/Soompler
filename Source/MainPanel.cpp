#include "MainPanel.h"
#include "Settings.h"
#include "Strings.h"
#include "PluginEditor.h"

//==============================================================================
MainPanel::MainPanel (SoomplerAudioProcessor& processor) : stateManager(processor.getStateManager()),
    processor(processor), editor(this)
{
    setSize(Settings::MAIN_PANEL_WIDTH, Settings::MAIN_PANEL_HEIGHT);
    setName("mainPanel");

    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    volumeKnob.reset (new SoomplerKnob("Volume"));
    addAndMakeVisible(volumeKnob.get());
    editor.addToGuiEditor(volumeKnob.get());
    volumeKnob->setPosition(60, 230);
    volumeKnob->attachTo("volume", stateManager);

    openFileButton.reset (new SoomplerImageButton ("open file button"));
    addAndMakeVisible (openFileButton.get());
    editor.addToGuiEditor (openFileButton.get());
    openFileButton->setTooltip (TRANS("Open Sample\n"));
    openFileButton->setButtonText (TRANS("new button"));
    openFileButton->addListener (this);
    openFileButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::openFile_png, BinaryData::openFile_pngSize),
                               1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
    openFileButton->setBounds (8, 8, 39, 32);

    aboutButton.reset (new SoomplerImageButton ("about button"));
    addAndMakeVisible (aboutButton.get());
    editor.addToGuiEditor (aboutButton.get());
    aboutButton->setTooltip (TRANS("About Program\n"));
    aboutButton->setButtonText (TRANS("About program"));
    aboutButton->addListener (this);
    aboutButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::about_png, BinaryData::about_pngSize),
                               1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
    aboutButton->setBounds (8, 8, 39, 32);
    
    loopButton.reset (new ToggledImageButton ("loop button"));
    addAndMakeVisible (loopButton.get());
    editor.addToGuiEditor (loopButton.get());
    loopButton->setTooltip (TRANS("Loop sample playback"));
    loopButton->setButtonText (TRANS("Loop"));
    loopButton->addListener (this);
    loopButton->setImages (false, true, true,
                            ImageCache::getFromMemory (BinaryData::loop_png, BinaryData::loop_pngSize),
                            1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));
    loopButton->setBounds (450, 60, 30, 30);
    loopAttachment.reset(new ButtonAttachment(stateManager, "loop", *loopButton));    

    sampleViewer.reset (new SampleViewer(processor.getThumbnail(), processor, processor));
    addAndMakeVisible (sampleViewer.get());
    editor.addToGuiEditor (sampleViewer.get());
    sampleViewer->setName ("SampleViewer");
    sampleViewer->setBounds(Settings::SAMPLE_VIEWER_BOUNDS);
    // not visible until sample is loaded
    sampleViewer->setVisible(false);
    processor.addSampleInfoListener(sampleViewer);

    loadSampleTip.reset (new Label ("no sample loaded label",
                                    TRANS(Strings::NO_SAMPLE_LOADED_TEXT)));
    addAndMakeVisible (loadSampleTip.get());
    editor.addToGuiEditor (loadSampleTip.get());
    loadSampleTip->setFont (Font (27.90f, Font::plain));
    loadSampleTip->setJustificationType (Justification::centred);
    loadSampleTip->setEditable (false, false, false);
    loadSampleTip->setColour (Label::textColourId, Colour (0xff0a0000));

    loadSampleTip->setBounds (20, 136, 460, 56);

    pianoRoll.reset (new PianoRoll(processor, processor));
    pianoRoll->setBounds (Settings::PIANO_ROLL_BOUNDS);
    pianoRoll->setName ("piano roll component");
    processor.addSampleInfoListener(pianoRoll);

    addAndMakeVisible (pianoRoll.get());
    editor.addToGuiEditor (pianoRoll.get());

    // ADSR controls
    attackKnob.reset (new SoomplerKnob ("Attack"));
    addAndMakeVisible(attackKnob.get());
    editor.addToGuiEditor(attackKnob.get());
    attackKnob->setPosition(150, 230);
    attackKnob->attachTo("attack", stateManager);
    
    decayKnob.reset (new SoomplerKnob ("Decay"));
    addAndMakeVisible(decayKnob.get());
    editor.addToGuiEditor(decayKnob.get());
    decayKnob->setPosition(200, 230);
    decayKnob->attachTo("decay", stateManager);
    
    sustainKnob.reset (new SoomplerKnob ("Sustain"));
    addAndMakeVisible(sustainKnob.get());
    editor.addToGuiEditor(sustainKnob.get());
    sustainKnob->setPosition(250, 230);
    sustainKnob->attachTo("sustain", stateManager);
    
    releaseKnob.reset (new SoomplerKnob ("Release"));
    addAndMakeVisible(releaseKnob.get());
    editor.addToGuiEditor(releaseKnob.get());
    releaseKnob->setPosition(300, 230);
    releaseKnob->attachTo("release", stateManager);
    
    // Reverse sample
    reverseButton.reset(new SoomplerToggleButton(TRANS("Reverse\n")));
    addAndMakeVisible(reverseButton.get());
    editor.addToGuiEditor(reverseButton.get());
    reverseButton->setBounds(380, 230, 100, 50);
    reverseButton->addListener(this);
    
//    // adsr panel
//    adsrPanel.reset(new LinearPanel(Orientation::HORIZONTAL));
//    adsrPanel->addAndMakeVisible(attackKnob.get());
//    adsrPanel->addAndMakeVisible(decayKnob.get());
//    adsrPanel->addAndMakeVisible(sustainKnob.get());
//    adsrPanel->addAndMakeVisible(releaseKnob.get());
//    adsrPanel->setBounds(0, 0, 200, 50);
//    addAndMakeVisible(adsrPanel.get());

    // hide some controls until sample is loaded
    attackKnob->setVisible(false);
    decayKnob->setVisible(false);
    sustainKnob->setVisible(false);
    releaseKnob->setVisible(false);
    volumeKnob->setVisible(false);
    reverseButton->setVisible(false);
    loopButton->setVisible(false);
    
    // hide deprecated buttons
    openFileButton->setVisible(false);

    // connect knobs to listener
    attackKnob->addListener(this);
    decayKnob->addListener(this);
    sustainKnob->addListener(this);
    releaseKnob->addListener(this);
    volumeKnob->addListener(this);

    // add GUI editor last
    // it ensures that gui overlay will work properly
    editor.initOverlay();
    
    restoreMainPanelState();
}

MainPanel::~MainPanel()
{
    loopAttachment = nullptr;

    attackKnob = nullptr;
    decayKnob = nullptr;
    sustainKnob = nullptr;
    releaseKnob = nullptr;
    volumeKnob = nullptr;
    openFileButton = nullptr;
    aboutButton = nullptr;
    sampleViewer = nullptr;
    loadSampleTip = nullptr;
    pianoRoll = nullptr;
    reverseButton = nullptr;
    loopButton = nullptr;
}

//==============================================================================
void MainPanel::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
    g.drawImage(backgroundImage, Rectangle<float>(0, 0, this->getWidth(), this->getHeight()));

    editor.paintBackOverlay(g);

    // draw menu panel
    g.setGradientFill(Settings::MAIN_MENU_GRADIENT);
    g.fillRect(0, 0, this->getWidth(), Settings::MAIN_MENU_HEIGHT);

}

void MainPanel::resized()
{
}

void MainPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == volumeKnob->getSlider())
    {
        processor.setVolume(volumeKnob->getValue());
    }
    else if (sliderThatWasMoved == attackKnob->getSlider())
    {
        adsrParams.attack = attackKnob->getValue();
    }
    else if (sliderThatWasMoved == decayKnob->getSlider())
    {
        adsrParams.decay = decayKnob->getValue();
    }
    else if (sliderThatWasMoved == sustainKnob->getSlider())
    {
        adsrParams.sustain = sustainKnob->getValue();
    }
    else if (sliderThatWasMoved == releaseKnob->getSlider())
    {
        adsrParams.release = releaseKnob->getValue();
    }

    processor.setAdsrParams(adsrParams);
}

void MainPanel::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == openFileButton.get())
    {
        openFileButtonClicked();
    }
    else if (buttonThatWasClicked == aboutButton.get())
    {
        aboutButtonClicked();
    }
    else if (buttonThatWasClicked == loopButton.get())
    {
        loopButtonClicked();
    }
    else if (buttonThatWasClicked == reverseButton.get()) {
        reverseButtonClicked();
    }
}

float MainPanel::getVolume() const
{
    return volumeKnob.get()->getValue();
}

void MainPanel::openFileButtonClicked()
{
    FileChooser chooser(Strings::OPEN_FILE_DIALOG_TEXT,
                        File::getSpecialLocation(File::userHomeDirectory),
                        "*.mp3;*.wav;*.ogg", false);

    if (chooser.browseForFileToOpen())
    {
        File sampleFile (chooser.getResult());

        processor.loadSample(sampleFile);

        //playButton->setEnabled(true);

        repaint();
    }
}

void MainPanel::aboutButtonClicked()
{
    auto editor = static_cast<SoomplerAudioProcessorEditor*>(processor.getActiveEditor());
    editor->showAboutSplash();
}

void MainPanel::playSampleButtonClicked()
{
    processor.playSample();
}

void MainPanel::stopSampleButtonClicked()
{
    processor.stopSamplePlayback();
}

void MainPanel::timerCallback()
{
    repaint();
}

void MainPanel::transportStateChanged(TransportState state)
{
    switch (state) {
    case Ready:
        loadSampleTip->setVisible(false);

        sampleViewer->setVisible(true);
        attackKnob->setVisible(true);
        decayKnob->setVisible(true);
        sustainKnob->setVisible(true);
        releaseKnob->setVisible(true);
        volumeKnob->setVisible(true);
        reverseButton->setVisible(true);
        loopButton->setVisible(true);
        
        // start gui updates
        startTimer(40);
        break;
    case Starting:
        break;
    case Stopped:
        break;
    default:
        break;
    }
}

void MainPanel::loopButtonClicked() {
    processor.setLoopEnabled(loopButton->isToggled());
}

void MainPanel::reverseButtonClicked() {
    processor.setSampleReversed(reverseButton->isToggle());
}

bool MainPanel::isInterestedInFileDrag(const juce::StringArray &files) {
    for (auto fileName : files) {
        for (auto format : Settings::SUPPORTED_FILE_FORMATS) {
            if (fileName.endsWith(format)) {
                return true;
            }
        }
    }
    
    return false;
}


void MainPanel::filesDropped(const juce::StringArray &files, int x, int y) {
    // by now only first dropped file will be loaded
    auto fileName = files[0];
    
    processor.loadSample(File(fileName));
}

void MainPanel::restoreMainPanelState() {
    // check if sample already loaded
    // (plugin reopened)
    if (processor.getThumbnail().getNumChannels() > 0) {
        // load current sample
        sampleViewer->newSampleInfoRecieved(processor.getCurrentSampleInfo());
        pianoRoll->newSampleInfoRecieved(processor.getCurrentSampleInfo());
        
        bool reversed = processor.isSampleReversed();
        bool looped = processor.isLoopModeOn();
        
        reverseButton->setToggled(reversed);
        loopButton->setToggled(looped);
        
        
        this->transportStateChanged(TransportState::Ready);
    }
}




