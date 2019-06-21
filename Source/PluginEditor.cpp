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
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT);

    // init components state
    openFileButton.setButtonText(Strings::BUTTON_OPEN_FILE_TEXT);
    openFileButton.addListener(this);

    playSampleButton.setButtonText(Strings::BUTTON_PLAY_SAMPLE_TEXT);
    playSampleButton.addListener(this);
    playSampleButton.setEnabled(false);                         // make enabled when sample is loaded

    stopSampleButton.setButtonText(Strings::BUTTON_STOP_SAMPLE_TEXT);
    stopSampleButton.addListener(this);

    addAndMakeVisible(openFileButton);
    addAndMakeVisible(playSampleButton);
    addAndMakeVisible(stopSampleButton);

    stopSampleButton.setVisible(false);

    // load background image
    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    // subscribe to all transport events from processor
    processor.setTransportStateListener(this);
    // subscribe to thumbnail events, to catch thumbnail fully loaded time
    processor.getThumbnail().addChangeListener(this);
}

SoomplerAudioProcessorEditor::~SoomplerAudioProcessorEditor()
{
}

//==============================================================================
void SoomplerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.drawImage(backgroundImage, Rectangle<float>(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT));

    drawThumbnail(g);
    drawSampleNameOrMessage(g);
}

void SoomplerAudioProcessorEditor::drawThumbnail(Graphics &g)
{
    AudioThumbnail& thumbnail = processor.getThumbnail();
    if (thumbnail.getNumChannels() == 0)
    {
        return;
    }

    g.setColour(Settings::THUMBNAIL_BOUNDS_COLOR);
    g.drawRect(Settings::THUMBNAIL_BOUNDS);
    g.setColour(Settings::THUMBNAIL_COLOR);

    auto audioLength = thumbnail.getTotalLength();
    thumbnail.drawChannels(g,
                           Settings::THUMBNAIL_BOUNDS,
                           0.0,
                           audioLength,
                           1.0f);

    g.setColour(Settings::THUMBNAIL_HEADER_COLOR);
    g.fillRect(Settings::THUMBNAIL_HEADER_BOUNDS);

    // draw position line
    g.setColour(Settings::POSITION_LINE_COLOR);

    auto audioPosition = processor.getCurrentAudioPosition();
    auto drawPosition = ((audioPosition / audioLength)
                         * Settings::THUMBNAIL_BOUNDS.getWidth() + Settings::THUMBNAIL_BOUNDS.getX());

    g.drawLine(drawPosition, Settings::THUMBNAIL_BOUNDS.getY(), drawPosition, Settings::THUMBNAIL_BOUNDS.getBottom(), 3.0f);
}

void SoomplerAudioProcessorEditor::drawSampleNameOrMessage(Graphics &g)
{
    int y;
    if (processor.getLoadedSample() != nullptr) {
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
    repaint();
}

void SoomplerAudioProcessorEditor::resized()
{
    openFileButton.setBounds(Settings::BUTTON_OPEN_FILE_POSITION);
    playSampleButton.setBounds(Settings::BUTTON_PLAY_SAMPLE_POSITION);
    stopSampleButton.setBounds(Settings::BUTTON_PLAY_SAMPLE_POSITION);
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

        playSampleButton.setEnabled(true);

        repaint();
    }
}

void SoomplerAudioProcessorEditor::playSampleButtonClicked()
{
    processor.playSample();
    playSampleButton.setVisible(false);
    stopSampleButton.setVisible(true);
}

void SoomplerAudioProcessorEditor::stopSampleButtonClicked()
{
    processor.stopSamplePlayback();
}

String SoomplerAudioProcessorEditor::getLoadedSampleNameOrPlaceholder()
{
    File* loadedSample = processor.getLoadedSample();

    if (loadedSample == nullptr) {
        return Strings::NO_SAMPLE_LOADED_TEXT;
    } else {
        return getCroppedNameIfNeeded(loadedSample->getFileName());
    }
}

String SoomplerAudioProcessorEditor::getCroppedNameIfNeeded(String fileName)
{
    if (fileName.length() <= Settings::MAX_SAMPLE_NAME_LENGTH) {
        return fileName;
    }

    String result =  fileName.substring(0, Settings::MAX_SAMPLE_NAME_LENGTH - 4);
    result.append("...", 3);

    return result;
}

void SoomplerAudioProcessorEditor::transportStateChanged(TransportState state)
{
    switch (state) {
    case Starting:
        startTimer(40);
        break;
    case Stopped:
        stopSampleButton.setVisible(false);
        playSampleButton.setVisible(true);
        stopTimer();
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
    repaint();
}
