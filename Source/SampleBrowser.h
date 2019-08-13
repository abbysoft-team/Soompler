/*
  ==============================================================================

    SampleBrowser.h
    Created: 9 Aug 2019 5:05:47pm
    Author:  artyom

  ==============================================================================
*/
#include <JuceHeader.h>
#include "FileListener.h"
#include "PluginProcessor.h"

#pragma once

class SampleBrowser : public Component, public FileBrowserListener {

public:
    SampleBrowser(FileListener& listener, SoomplerAudioProcessor &processor);
    ~SampleBrowser() = default;

private:
        std::shared_ptr<FilePreviewComponent> previewComponent;
        std::unique_ptr<FileBrowserComponent> browser;
        FileListener& fileListener;
        Image background;

        // Component interface
public:
        void resized();

        // FileBrowserListener interface
public:
        void selectionChanged() override;
        void fileClicked(const File &file, const MouseEvent &e) override;
        void fileDoubleClicked(const File &file) override;
        void browserRootChanged(const File &newRoot) override;

        // Component interface
public:
        void paint(Graphics &g);
};
