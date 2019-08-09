/*
  ==============================================================================

    SampleBrowser.h
    Created: 9 Aug 2019 5:05:47pm
    Author:  artyom

  ==============================================================================
*/
#include <JuceHeader.h>
#include "FileListener.h"

#pragma once

class SampleBrowser : public Component, FileBrowserListener {

public:
    SampleBrowser(FileListener& listener);
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
        void selectionChanged();
        void fileClicked(const File &file, const MouseEvent &e);
        void fileDoubleClicked(const File &file);
        void browserRootChanged(const File &newRoot);

        // Component interface
public:
        void paint(Graphics &g);
};
