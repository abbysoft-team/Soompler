/*
  ==============================================================================

    SamplePreviewComponent.h
    Created: 9 Aug 2019 6:21:41pm
    Author:  artyom

  ==============================================================================
*/
#include <JuceHeader.h>
#include "SoomplerSlider.h"
#include "SoomplerImageButton.h"
#include "SoomplerToggleButton.h"

#pragma once

class SamplePreviewComponent : public FilePreviewComponent {
public:
    SamplePreviewComponent();
    ~SamplePreviewComponent() = default;

    // FilePreviewComponent interface
public:
    void selectedFileChanged(const File &newSelectedFile);

private:
    std::shared_ptr<SoomplerSlider> volume;
    std::shared_ptr<SoomplerImageButton> playButton;
    std::shared_ptr<SoomplerImageButton> stopButton;
    std::shared_ptr<SoomplerToggleButton> autoplayButton;

    // Component interface
public:
    void resized();
};
