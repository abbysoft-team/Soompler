#include "MainPanel.h"

//==============================================================================
MainPanel::MainPanel (SoomplerAudioProcessor& processor)
{
    setName ("mainPanel");
    volumeKnob.reset (new Slider ("volume knob"));
    addAndMakeVisible (volumeKnob.get());
    volumeKnob->setTooltip (TRANS("volume"));
    volumeKnob->setRange (0, 10, 0);
    volumeKnob->setSliderStyle (Slider::Rotary);
    volumeKnob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    volumeKnob->addListener (this);

    volumeKnob->setBounds (68, 210, 50, 50);

    volumeKnobLabel.reset (new Label ("volume knob label",
                                      TRANS("volume\n")));
    addAndMakeVisible (volumeKnobLabel.get());
    volumeKnobLabel->setFont (Font (15.00f, Font::plain));
    volumeKnobLabel->setJustificationType (Justification::centredLeft);
    volumeKnobLabel->setEditable (false, false, false);
    volumeKnobLabel->setColour (TextEditor::textColourId, Colours::black);
    volumeKnobLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    volumeKnobLabel->setBounds (64, 192, 64, 24);

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

    aboutButton.reset (new ImageButton ("about program"));
    addAndMakeVisible (aboutButton.get());
    aboutButton->setTooltip (TRANS("About program"));
    aboutButton->setButtonText (TRANS("new button"));
    aboutButton->addListener (this);

    aboutButton->setImages (false, true, true,
                            ImageCache::getFromMemory (BinaryData::about_png, BinaryData::about_pngSize), 1.000f,
                            Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));
    aboutButton->setBounds (56, 8, 39, 32);

    sampleViewer.reset (new SampleViewer(processor.getThumbnail(), processor));
    addAndMakeVisible (sampleViewer.get());
    sampleViewer->setName ("SampleViewer");

    sampleViewer->setBounds (20, 115, 460, 75);

    label.reset (new Label ("new label",
                            TRANS("Load sample by clicking load sample button in the upper left corner\n")));
    addAndMakeVisible (label.get());
    label->setFont (Font (27.90f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colour (0xff0a0000));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (20, 136, 460, 56);

    pianoRoll.reset (new PianoRoll(processor, processor));
    addAndMakeVisible (pianoRoll.get());
    pianoRoll->setName ("piano roll component");

    pianoRoll->setBounds (0, 335, 500, 65);

    setSize (500, 400);
}

MainPanel::~MainPanel()
{
    volumeKnob = nullptr;
    volumeKnobLabel = nullptr;
    openFileButton = nullptr;
    aboutButton = nullptr;
    sampleViewer = nullptr;
    label = nullptr;
    pianoRoll = nullptr;
}

//==============================================================================
void MainPanel::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
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
    }
    else if (buttonThatWasClicked == aboutButton.get())
    {
    }
}
