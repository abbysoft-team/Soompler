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
    processor.setVolume(mainPanel.getVolume());
    repaint();
}
