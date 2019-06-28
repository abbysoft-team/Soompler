#include "PianoRoll.h"
#include "Settings.h"

void drawNoteTips(Graphics& g);
void drawNoteDelimiters(Graphics& g);
void drawBlackNotes(Graphics& g);
void drawBlackNote(int coord, Graphics& g);
void drawActiveNotes(Graphics& g);

PianoRoll::PianoRoll ()
{
    setSize (Settings::PIANO_ROLL_WIDTH, Settings::PIANO_ROLL_HEIGHT);
}

PianoRoll::~PianoRoll()
{
}

//==============================================================================
void PianoRoll::paint (Graphics& g)
{
    g.setColour(Settings::PIANO_ROLL_WHITE_COLOR);
    g.setGradientFill(Settings::PIANO_ROLL_GRADIENT);
    g.fillRect(0, 0, Settings::PIANO_ROLL_WIDTH, Settings::PIANO_ROLL_HEIGHT);

    drawNoteDelimiters(g);
    drawBlackNotes(g);
    drawActiveNotes(g);
    drawNoteTips(g);
}

void drawNoteDelimiters(Graphics& g)
{
    g.setColour(Settings::PIANO_ROLL_DELIMITER_COLOR);

    // end of all keys
    g.drawLine(0, 0, Settings::PIANO_ROLL_WIDTH - 1, 0, 1);

    auto currentDelimiterX = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH;
    while (currentDelimiterX < Settings::PIANO_ROLL_WIDTH) {
        g.setColour(Settings::PIANO_ROLL_DELIMITER_COLOR);
        g.drawLine(currentDelimiterX, 0, currentDelimiterX, Settings::PIANO_ROLL_HEIGHT-1, 1);
        g.setColour(Colour(230, 230, 230));
        g.drawLine(currentDelimiterX - 1, 0, currentDelimiterX - 1, Settings::PIANO_ROLL_HEIGHT - 1, 1);
        g.setColour(Colour(215, 215, 215));
        g.drawLine(currentDelimiterX + 1, 0, currentDelimiterX + 1, Settings::PIANO_ROLL_HEIGHT - 1, 1);

        currentDelimiterX += Settings::PIANO_ROLL_WHITE_NOTE_WIDTH;
    }
}

void drawBlackNotes(Graphics &g)
{
    // pattern from C#
    int blackKeyPattern[] = { 2, 1, 2, 1, 1 };
    static auto blackKeyOffset = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH;

    //g.setGradientFill(Settings::PIANO_ROLL_BLACK_KEY_GRADIENT);

    auto currentBlackKeyCoord = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH - Settings::PIANO_ROLL_BLACK_NOTE_WIDTH / 2;
    auto currentPatternIndex = 0;
    while (currentBlackKeyCoord < Settings::PIANO_ROLL_WIDTH) {
        drawBlackNote(currentBlackKeyCoord, g);

        if (currentPatternIndex == 4) {
            currentPatternIndex = -1;
        }

        currentBlackKeyCoord += blackKeyOffset * blackKeyPattern[++currentPatternIndex];
    }

}

void drawBlackNote(int coord, Graphics &g)
{
    static auto shapeOffsetX = Settings::PIANO_ROLL_BLACK_NOTE_WIDTH * 0.13;
    static auto shapeOffsetWidth = Settings::PIANO_ROLL_BLACK_NOTE_WIDTH - shapeOffsetX * 2;
    static auto shapeOffsetY = Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT * 0.08;
    static auto shapeOffsetHeight = Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT - shapeOffsetY;

    g.setColour(Settings::PIANO_ROLL_BLACK_NOTE_COLOR);
    g.fillRect(coord, 0, Settings::PIANO_ROLL_BLACK_NOTE_WIDTH, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT);
    g.setColour(Settings::PIANO_ROLL_DELIMITER_COLOR);

    // small spaces between black and white keys
    g.drawLine(coord - 1, 0, coord -1, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT + 1);
    g.drawLine(coord + Settings::PIANO_ROLL_BLACK_NOTE_WIDTH + 1, 0,
               coord + Settings::PIANO_ROLL_BLACK_NOTE_WIDTH + 1, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT + 1);

    // 3d shape of black keys
    g.setColour(Settings::PIANO_ROLL_BLACK_NOTE_TOP_COLOR);
    g.fillRect((float) coord + shapeOffsetX, (float) 0, (float) shapeOffsetWidth, (float) shapeOffsetHeight);

    g.setColour(Settings::PIANO_ROLL_BLACK_NOTE_HIGHLIGHTS_COLOR);
    auto shapeLineX = coord + Settings::PIANO_ROLL_BLACK_NOTE_WIDTH - shapeOffsetX;

    g.drawLine(shapeLineX, 0, shapeLineX, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT - shapeOffsetY);
    g.drawLine(coord + shapeOffsetX, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT - shapeOffsetY,
               coord + shapeOffsetX + shapeOffsetWidth, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT - shapeOffsetY);


}

void drawActiveNotes(Graphics& g)
{

}

void drawNoteTips(Graphics& g)
{
    static auto spaceBetweenCNotes = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH * 7;

    g.setFont(Settings::PIANO_ROLL_TIPS_FONT);
    g.setColour(Settings::PIANO_ROLL_NOTE_TIPS_COLOR);

    auto currentCNote = 0;
    auto currentCNoteCoord  = Settings::PIANO_ROLL_TIPS_OFFSET_X;
    String noteName = "";
    while (currentCNoteCoord < Settings::PIANO_ROLL_WIDTH) {
        noteName = String("C");
        noteName.append(std::to_string(currentCNote), 3);
        g.drawSingleLineText(noteName, currentCNoteCoord, Settings::PIANO_ROLL_TIPS_OFFSET_Y);

        currentCNoteCoord += spaceBetweenCNotes;
        currentCNote++;
    }

}

void PianoRoll::resized()
{
}

#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PianoRoll" componentName="Piano roll"
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="Settings::PIANO_ROLL_WIDTH" initialHeight="Settings::PIANO_ROLL_HEIGHT">
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

