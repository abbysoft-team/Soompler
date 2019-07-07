/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Strings.h"

//==============================================================================
SoomplerAudioProcessorEditor::SoomplerAudioProcessorEditor (SoomplerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), mainFont("DejaVu Sans", 12, Font::plain), mainPanel(processor)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT);

    // set OpenGL renderer
    this->glContext.attachTo(*this);

    addAndMakeVisible(mainPanel);

    // init components state
//    auto openButtonImage = ImageCache::getFromMemory(BinaryData::openFile_png, BinaryData::background_pngSize);
//    openFileButton.setImages(true, true, true,
//                             openButtonImage, 1.f, Settings::BUTTON_OPAQUE_COLOR,
//                             openButtonImage, .0f, Settings::BUTTON_OVER_COLOR, openButtonImage,
//                             .0f, Settings::BUTTON_DOWN_COLOR);

//    openFileButton.addListener(this);

//    playSampleButton.setButtonText(Strings::BUTTON_PLAY_SAMPLE_TEXT);
//    playSampleButton.addListener(this);
//    playSampleButton.setEnabled(false);                         // make enabled when sample is loaded

//    stopSampleButton.setButtonText(Strings::BUTTON_STOP_SAMPLE_TEXT);
//    stopSampleButton.addListener(this);

//    volumeKnob.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    volumeKnob.addListener(this);
//    volumeKnob.setRange(0.0, 1.0, 0.01);
//    volumeKnob.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
//    volumeKnob.setName("Volume");
//    volumeKnob.setTooltip("Volume");
//    volumeKnob.setValue(0.5);

//    volumeLabel.setText("Volume", dontSendNotification);
//    volumeLabel.attachToComponent(&volumeKnob, true);

//    addAndMakeVisible(openFileButton);
//    addAndMakeVisible(playSampleButton);
//    addAndMakeVisible(stopSampleButton);
//    addAndMakeVisible(volumeKnob);
//    addAndMakeVisible(volumeLabel);
//    addAndMakeVisible(pianoRoll);

//    stopSampleButton.setVisible(false);

//    // load background image
//    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    // subscribe to all transport events from processor
    processor.setTransportStateListener(this);
    // subscribe to thumbnail events, to catch thumbnail fully loaded time
    processor.getThumbnail().addChangeListener(this);

    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName(mainFont.getTypefaceName());
}

//==============================================================================
void SoomplerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
//    g.drawImage(backgroundImage, Rectangle<float>(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT));
//    g.setFont(mainFont);

    drawSampleNameOrMessage(g);
}

void SoomplerAudioProcessorEditor::drawSampleNameOrMessage(Graphics &g)
{
    int y;
    if (processor.getLoadedSample().has_value()) {
        g.setColour(Settings::SAMPLE_NAME_COLOR);
        y = Settings::SAMPLE_NAME_TEXT_Y;
    } else {
        g.setColour(Colours::black);

        // this is not actuall name of sample, but message to load sample
        y = Settings::SAMPLE_NAME_TEXT_Y + 20;
    }

    g.setFont(Settings::SAMPLE_NAME_FONT_SIZE);
    g.drawSingleLineText(getLoadedSampleNameOrPlaceholder(),
                         Settings::SAMPLE_NAME_TEXT_X,
                         Settings::SAMPLE_NAME_TEXT_Y,
                         Justification::horizontallyCentred);
}

void SoomplerAudioProcessorEditor::timerCallback()
{
    processor.updateTransportState();
    repaint();
}

void SoomplerAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
//    if (slider == &volumeKnob) {
//        processor.setVolume(slider->getValue());
//    }
}

void SoomplerAudioProcessorEditor::resized()
{
//    openFileButton.setBounds(Settings::BUTTON_OPEN_FILE_POSITION);
//    playSampleButton.setBounds(Settings::BUTTON_PLAY_SAMPLE_POSITION);
//    stopSampleButton.setBounds(Settings::BUTTON_PLAY_SAMPLE_POSITION);
//    volumeKnob.setBounds(Settings::VOLUME_KNOB_POSITION);
//    pianoRoll.setBounds(Settings::PIANO_ROLL_BOUNDS);
}

void SoomplerAudioProcessorEditor::buttonClicked(Button *button)
{
    if (button->getButtonText().equalsIgnoreCase(Strings::BUTTON_OPEN_FILE_TEXT)) {
        openFileButtonClicked();
    } else if (button->getButtonText().equalsIgnoreCase(Strings::BUTTON_PLAY_SAMPLE_TEXT)) {
        playSampleButtonClicked();
    } else if (button->getButtonText().equalsIgnoreCase(Strings::BUTTON_STOP_SAMPLE_TEXT)) {
        stopSampleButtonClicked();
    }
}

void SoomplerAudioProcessorEditor::openFileButtonClicked()
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

void SoomplerAudioProcessorEditor::playSampleButtonClicked()
{
    processor.playSample();
//    playSampleButton.setVisible(false);
//    stopSampleButton.setVisible(true);
}

void SoomplerAudioProcessorEditor::stopSampleButtonClicked()
{
    processor.stopSamplePlayback();
}

String getCroppedNameIfNeeded(String fileName)
{
  if (fileName.length() <= Settings::MAX_SAMPLE_NAME_LENGTH) {
    return fileName;
  }

  String result =  fileName.substring(0, Settings::MAX_SAMPLE_NAME_LENGTH - 4);
  result.append("...", 3);

  return result;
}

String SoomplerAudioProcessorEditor::getLoadedSampleNameOrPlaceholder()
{
    std::optional<File> loadedSample = processor.getLoadedSample();

    if (!loadedSample.has_value()) {
        return Strings::NO_SAMPLE_LOADED_TEXT;
    } else {
        return getCroppedNameIfNeeded(loadedSample->getFileName());
    }
}

void SoomplerAudioProcessorEditor::transportStateChanged(TransportState state)
{
    switch (state) {
    case Ready:
        startTimer(40);
        break;
    case Starting:
        break;
    case Stopped:
//        stopSampleButton.setVisible(false);
//        playSampleButton.setVisible(true);
        break;
    default:
        break;
    }
}

void SoomplerAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &processor.getThumbnail()) {
        thumbnailChanged(processor.getThumbnail());
    }
}

void SoomplerAudioProcessorEditor::thumbnailChanged(AudioThumbnail &thumbnail)
{
    // sample loaded
    //processor.setVolume(volumeKnob.getValue());
    repaint();
}
