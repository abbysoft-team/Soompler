#include "MainPanel.h"
#include "Settings.h"
#include "Strings.h"

//==============================================================================
MainPanel::MainPanel (SoomplerAudioProcessor& processor) : processor(processor)
{
    setSize(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT);
    setName("mainPanel");

    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    volumeKnob.reset (new Slider ("volume knob"));
    addAndMakeVisible (volumeKnob.get());
    volumeKnob->setTooltip (TRANS("volume"));
    volumeKnob->setRange (0.0, 1.0, 0.01);
    volumeKnob->setValue(0.5);
    volumeKnob->setSliderStyle (Slider::Rotary);
    volumeKnob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volumeKnob->addListener (this);
    volumeKnob->setBounds (68, 230, 50, 50);

    volumeKnobLabel.reset (new Label ("volume knob label",
                                      TRANS("volume\n")));
    addAndMakeVisible (volumeKnobLabel.get());
    volumeKnobLabel->setFont (Font (15.00f, Font::plain));
    volumeKnobLabel->setJustificationType (Justification::centredLeft);
    volumeKnobLabel->setEditable (false, false, false);
    volumeKnobLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeKnobLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    volumeKnobLabel->setBounds (64, 218, 64, 24);

    openFileButton.reset (new ImageButton ("open file button"));
    addAndMakeVisible (openFileButton.get());
    openFileButton->setTooltip (TRANS("Open Sample\n"));
    openFileButton->setButtonText (TRANS("new button"));
    openFileButton->addListener (this);
    openFileButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::openFile_png, BinaryData::openFile_pngSize),
                               1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
    openFileButton->setBounds (8, 8, 39, 32);

    aboutButton.reset (new ImageButton ("about button"));
    addAndMakeVisible (aboutButton.get());
    aboutButton->setTooltip (TRANS("About Program\n"));
    aboutButton->setButtonText (TRANS("About program"));
    aboutButton->addListener (this);
    aboutButton->setImages (false, true, true,
                               ImageCache::getFromMemory (BinaryData::about_png, BinaryData::about_pngSize),
                               1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000),
                               Image(), 1.000f, Colour (0x00000000));
    aboutButton->setBounds (52, 8, 39, 32);

    sampleViewer.reset (new SampleViewer(processor.getThumbnail(), processor));
    addAndMakeVisible (sampleViewer.get());
    sampleViewer->setName ("SampleViewer");
    sampleViewer->setBounds(Settings::SAMPLE_VIEWER_BOUNDS);
    // not visible until sample is loaded
    sampleViewer->setVisible(false);

    // link processor with sampleViewer
    auto sampleInfoListener = static_cast<SampleInfoListener*> (&processor);
    sampleViewer->setSampleInfoListener(std::shared_ptr<SampleInfoListener>(sampleInfoListener));
    processor.setSampleInfoListener(sampleViewer);

    loadSampleTip.reset (new Label ("new label",
                            TRANS("Load sample by clicking load sample button in the upper left corner\n")));
    addAndMakeVisible (loadSampleTip.get());
    loadSampleTip->setFont (Font (27.90f, Font::plain));
    loadSampleTip->setJustificationType (Justification::centred);
    loadSampleTip->setEditable (false, false, false);
    loadSampleTip->setColour (Label::textColourId, Colour (0xff0a0000));

    loadSampleTip->setBounds (20, 136, 460, 56);

    pianoRoll.reset (new PianoRoll(processor, processor));
    pianoRoll->setBounds (Settings::PIANO_ROLL_BOUNDS);
    pianoRoll->setName ("piano roll component");

    addAndMakeVisible (pianoRoll.get());
}

MainPanel::~MainPanel()
{
    volumeKnob = nullptr;
    volumeKnobLabel = nullptr;
    openFileButton = nullptr;
    aboutButton = nullptr;
    sampleViewer = nullptr;
    loadSampleTip = nullptr;
    pianoRoll = nullptr;
}

//==============================================================================
void MainPanel::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
    g.drawImage(backgroundImage, Rectangle<float>(0, 0, this->getWidth(), this->getHeight()));

    // draw menu panel
    g.setGradientFill(Settings::MAIN_PANEL_GRADIENT);
    g.fillRect(0, 0, this->getWidth(), Settings::MAIN_PANEL_HEIGHT);
}

void MainPanel::resized()
{
}

void MainPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == volumeKnob.get())
    {
    }
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

        //playSampleButton.setEnabled(true);

        repaint();
    }
}

void MainPanel::aboutButtonClicked()
{
    // show about dialog
}

void MainPanel::transportStateChanged(TransportState state)
{
    switch (state) {
    case Ready:
        loadSampleTip->setVisible(false);
        sampleViewer->setVisible(true);
        break;
    case Starting:
        break;
    case Stopped:
        break;
    default:
        break;
    }
}
