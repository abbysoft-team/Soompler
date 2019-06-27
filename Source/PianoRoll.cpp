#include "PianoRoll.h"
#include "Settings.h"

void drawNoteTips(Graphics& g);
void drawNoteDelimiters(Graphics& g);

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

