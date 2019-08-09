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
    volume.reset(new SoomplerSlider(Slider::SliderStyle::LinearBarVertical, Slider::NoTextBox));
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

    fb.flexDirection = FlexBox::Direction::column;
    fb.justifyContent = FlexBox::JustifyContent::center;

    FlexItem::Margin topMargin;
    topMargin.top = 10.f;

    fb.items.add(FlexItem(*(volume.get())).withFlex(1).withMaxWidth(20).withMargin(topMargin));
    if (stopButton->isVisible()) {
        fb.items.add(FlexItem(*(stopButton.get())).withMinHeight(20).withMaxWidth(20).withMargin(topMargin));
    }
    if (playButton->isVisible()) {
        fb.items.add(FlexItem(*(playButton.get())).withMinHeight(20).withMaxWidth(20).withMargin(topMargin));
    }
    fb.items.add(FlexItem(*(autoplayButton.get())).withMinHeight(20).withMargin(FlexItem::Margin(10.0f, 0.0f, 10.0f, 0.0f)));

    fb.performLayout(getLocalBounds());
}
