/*
  ==============================================================================

    FileListener.h
    Created: 9 Aug 2019 5:51:04pm
    Author:  artyom

  ==============================================================================
*/
#include <JuceHeader.h>
#pragma once

class FileListener {
public:
    ~FileListener() = default;

    virtual void fileRecieved(const File& file) = 0;

};
