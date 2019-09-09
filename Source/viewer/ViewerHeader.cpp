/*
  ==============================================================================

    ViewerHeader.cpp
    Created: 9 Sep 2019 1:53:02pm
    Author:  artyom

  ==============================================================================
*/

#include "ViewerHeader.h"
#include "../Settings.h"

ViewerHeader::ViewerHeader()
{

}

void ViewerHeader::paint(Graphics &g)
{
    g.setColour(Settings::THUMBNAIL_HEADER_COLOR);
    g.fillRect(0, 0, getWidth(), getHeight());

    g.setColour(Settings::SAMPLE_NAME_COLOR);
    g.setFont(Settings::SAMPLE_NAME_FONT_SIZE);
    g.drawSingleLineText(getCroppedNameIfNeeded(),
                             Settings::SAMPLE_NAME_TEXT_X,
                             Settings::SAMPLE_NAME_TEXT_Y,
                         Justification::horizontallyCentred);
}

void ViewerHeader::newSampleInfoRecieved(std::shared_ptr<SampleInfo> info)
{
    samples.push_back(info);
}

String ViewerHeader::getCroppedNameIfNeeded()
{
    if (samples.empty()) {
        return "";
    }

    auto fileName = samples[0]->sampleName;
    if (fileName.length() <= Settings::MAX_SAMPLE_NAME_LENGTH) {
      return fileName;
    }

    String result =  fileName.substring(0, Settings::MAX_SAMPLE_NAME_LENGTH - 4);
    result.append("...", 3);

    return result;
}
