/*
  ==============================================================================

    SamplePreviewComponent.cpp
    Created: 9 Aug 2019 6:21:41pm
    Author:  artyom

  ==============================================================================
*/

#include "SamplePreviewComponent.h"

SamplePreviewComponent::SamplePreviewComponent() : FilePreviewComponent()
{
    volume.reset(new SoomplerSlider(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox));
    autoplayButton.reset(new SoomplerToggleButton(TRANS("autoplay")));
    playButton.reset(new SoomplerImageButton(TRANS("playButton")));
    stopButton.reset(new SoomplerImageButton(TRANS("stopButton")));

    Image stopImage = ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);

    playButton->setImages (false, true, true,
                           ImageCache::getFromMemory (BinaryData::play_png, BinaryData::play_pngSize),
                           1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000));

    stopButton->setImages (false, true, true,
                           ImageCache::getFromMemory (BinaryData::stop_png, BinaryData::stop_pngSize),
                           1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000),
                           Image(), 1.000f, Colour (0x00000000));

    volume->setRange(0.0, 1.0);
    volume->setValue(0.5);

    this->addAndMakeVisible(volume.get());
    this->addAndMakeVisible(autoplayButton.get());
    this->addAndMakeVisible(stopButton.get());
    this->addAndMakeVisible(playButton.get());

    stopButton->setVisible(false);
}

void SamplePreviewComponent::selectedFileChanged(const File &newSelectedFile)
{

}

void SamplePreviewComponent::resized()
{
    FlexBox fb;

    fb.flexDirection = FlexBox::Direction::row;

    fb.items.add(FlexItem(*(volume.get())).withFlex(3));
    if (stopButton->isVisible()) {
        fb.items.add(FlexItem(*(stopButton.get())).withFlex(1));
    }
    if (playButton->isVisible()) {
        fb.items.add(FlexItem(*(playButton.get())).withFlex(1).withMaxHeight(30).withMaxWidth(30));
    }
    fb.items.add(FlexItem(*(autoplayButton.get())).withFlex(1).withMargin(FlexItem::Margin(.0f, 20.0f, .0f, .0f)));

    fb.performLayout(getLocalBounds());
}
