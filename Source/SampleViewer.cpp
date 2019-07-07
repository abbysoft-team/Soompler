/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "SampleViewer.h"
#include "Settings.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...

void drawRangeLine(int xPos, Graphics& g);
// draw faded regions before and after range lines
void fadePreStartRegion(int startRangeBorderX, Graphics& g);
void fadePostEndRegion(int endRangeBorderX, Graphics& g);
bool isIntersectWithRangeLine(Point<int>& point, int rangeLinePos);
//[/MiscUserDefs]

//==============================================================================
SampleViewer::SampleViewer (AudioThumbnail& thumbnail, TransportInfoOwner& transportInfoOwner)
    : thumbnail(thumbnail), transportInfoOwner(transportInfoOwner),
    startRangeX(Settings::THUMBNAIL_BOUNDS.getX()),
    endRangeX(Settings::THUMBNAIL_BOUNDS.getRight()),
    maxRangeX(Settings::THUMBNAIL_BOUNDS.getRight())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SampleViewer::~SampleViewer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

void SampleViewer::setSampleInfoListener(std::shared_ptr<SampleInfoListener> listener)
{
    this->sampleInfoListener = listener;
}

//==============================================================================
void SampleViewer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..

    drawThumbnail(g);
    drawPositionLine(g);

    // start range line with faded region
    drawRangeLine(startRangeX, g);
    fadePreStartRegion(startRangeX, g);
    // end range line
    drawRangeLine(endRangeX, g);
    fadePostEndRegion(endRangeX, g);
    //[/UserPaint]
}


void SampleViewer::drawThumbnail(Graphics &g)
{
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
}


void SampleViewer::drawPositionLine(Graphics &g)
{
    g.setColour(Settings::POSITION_LINE_COLOR);

    // offset exist to ensure that line is visible even near start range line
    constexpr auto lineOffset = 2;

    auto audioPosition = transportInfoOwner.getTransportInfo()->getAudioPositionInSec();
    auto drawPosition = ((audioPosition / currentSample->lengthInSeconds)
                         * Settings::THUMBNAIL_BOUNDS.getWidth() + lineOffset + Settings::THUMBNAIL_BOUNDS.getX());

    g.drawLine(drawPosition, Settings::THUMBNAIL_BOUNDS.getY(), drawPosition, Settings::THUMBNAIL_BOUNDS.getBottom(), 3.0f);
}

void drawRangeLine(int xPos, Graphics& g) {
    g.setColour(Settings::RANGE_LINES_COLOR);
    g.drawLine(xPos, Settings::THUMBNAIL_BOUNDS.getY(), xPos, Settings::THUMBNAIL_BOUNDS.getBottom(), Settings::RANGE_LINES_WIDTH);
}

void fadePreStartRegion(int startRangeBorderX, Graphics& g) {
    g.setColour(Settings::NOT_ACTIVE_SAMPLE_REGION_MASK_COLOR);
    int preStartRegionWidth = startRangeBorderX - Settings::THUMBNAIL_BOUNDS.getX();
    g.fillRect(Settings::THUMBNAIL_BOUNDS.getX(), Settings::THUMBNAIL_BOUNDS.getY(), preStartRegionWidth, Settings::THUMBNAIL_BOUNDS.getHeight());
}

void fadePostEndRegion(int endRangeBorderX, Graphics& g) {
    g.setColour(Settings::NOT_ACTIVE_SAMPLE_REGION_MASK_COLOR);
    int postEndRegionWidth = jmax(0, Settings::THUMBNAIL_BOUNDS.getRight() - endRangeBorderX);
    g.fillRect(endRangeBorderX, Settings::THUMBNAIL_BOUNDS.getY(), postEndRegionWidth, Settings::THUMBNAIL_BOUNDS.getHeight());
}


void SampleViewer::mouseDrag(const MouseEvent &event)
{
    auto position = event.getPosition();
    if (isIntersectWithRangeLine(position, startRangeX)) {

        int rightBorderX = endRangeX - ((int) (Settings::RANGE_LINES_WIDTH*4));
        int leftBorderX = Settings::THUMBNAIL_BOUNDS.getX();

        if (position.getX() < leftBorderX) {
            startRangeX = leftBorderX;
        } else if (position.getX() > rightBorderX) {
            startRangeX = rightBorderX;
        } else {
            startRangeX = position.getX();
        }

        currentSample->startSample = calculateSampleByCoords(startRangeX);
        notifySampleInfoListeners();

        repaint();
    } else if (isIntersectWithRangeLine(position, endRangeX)) {
        int leftBorderX = startRangeX + ((int) (Settings::RANGE_LINES_WIDTH*4));

        if (position.getX() < leftBorderX) {
            endRangeX = leftBorderX;
        } else if (position.getX() > maxRangeX) {
            endRangeX = maxRangeX;
        } else {
            endRangeX = position.getX();
        }

        currentSample->endSample = calculateSampleByCoords(endRangeX);
        notifySampleInfoListeners();

        repaint();
    }
}

void SampleViewer::newSampleInfoRecieved(std::shared_ptr<SampleInfo> info)
{
    this->currentSample = info;

    calculateEndRangeX();
}

void SampleViewer::calculateEndRangeX()
{
    auto sampleLength = currentSample->lengthInSeconds;
    if (sampleLength < Settings::MAX_SAMPLE_LENGTH) {
        // everything is fine already
        return;
    }

    auto partOfSampleAllowed = Settings::MAX_SAMPLE_LENGTH / sampleLength;

    // 513 samples to make sure we dont reach max sample length
    auto lastAllowedSample = Settings::MAX_SAMPLE_LENGTH * currentSample->sampleRate - 513;
    currentSample->endSample = lastAllowedSample;

    notifySampleInfoListeners();

    endRangeX = Settings::THUMBNAIL_BOUNDS.getX() +
            (partOfSampleAllowed * Settings::THUMBNAIL_BOUNDS.getWidth());
    maxRangeX = endRangeX;
}

void SampleViewer::notifySampleInfoListeners()
{
    if (this->sampleInfoListener == nullptr) {
        return;
    }

    this->sampleInfoListener->newSampleInfoRecieved(currentSample);
}

bool isIntersectWithRangeLine(Point<int>& point, int rangeLinePos)
{
    static Rectangle<int> rangeLine;
    rangeLine.setX(rangeLinePos - Settings::RANGE_LINES_WIDTH);
    rangeLine.setY(Settings::THUMBNAIL_BOUNDS.getY());
    rangeLine.setWidth(Settings::RANGE_LINES_WIDTH * 2);
    rangeLine.setHeight(Settings::THUMBNAIL_BOUNDS.getHeight());

    return rangeLine.contains(point);
}

int64 SampleViewer::calculateSampleByCoords(int coordOnThumbnail)
{
    double conversionsError = 0.025;
    auto percentOfLength = coordOnThumbnail * 1.0 / Settings::THUMBNAIL_BOUNDS.getWidth() - conversionsError;
    return ((int64) (percentOfLength * currentSample->lengthInSamples));
}

void SampleViewer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SampleViewer" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

