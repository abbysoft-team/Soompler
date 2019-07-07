#include "PianoRoll.h"
#include "Settings.h"

void drawNoteTips(Graphics& g);
void drawNoteDelimiters(Graphics& g);
void drawBlackNotes(Graphics& g);
void drawActiveNoteMask(KeyInfo keyInfo, Graphics& g);
void drawBlackNote(int coord, Graphics& g);

static float getAverageKeyWidth();

PianoRoll::PianoRoll (MidiEventSupplier& midiSupplier, MidiEventConsumer& midiConsumer)
    : midiSupplier(midiSupplier), midiConsumer(midiConsumer)
{
    setSize (Settings::PIANO_ROLL_WIDTH, Settings::PIANO_ROLL_HEIGHT);

    calculateKeysInfo();
}

void PianoRoll::calculateKeysInfo()
{
    constexpr auto blackKeyOffset = Settings::PIANO_ROLL_BLACK_NOTE_WIDTH / 2.0;
    constexpr auto whiteKeyOffset = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH;

    // structure of keys 0 - white key 1 - black
    bool blackKeyPattern[] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0};

    auto currentIndex = 0;
    auto nextWhiteKeyX = 0;
    // init only keys after 48 (C2), cos it's first on the screen
    for (int i = 48; i < MAX_KEYS; i++) {
        // cycling through pattern array
        if (currentIndex == 12) {
            currentIndex = 0;
        }


        if (blackKeyPattern[currentIndex]) {
            KeyInfo nextKey;
            nextKey.height = Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT;
            nextKey.width = Settings::PIANO_ROLL_BLACK_NOTE_WIDTH;
            nextKey.x = nextWhiteKeyX - blackKeyOffset;
            nextKey.isWhite = false;
            nextKey.number = i;

            keysInfo[i] = nextKey;
        } else {
            KeyInfo nextKey;
            nextKey.height = Settings::PIANO_ROLL_HEIGHT;
            nextKey.width = whiteKeyOffset;
            nextKey.isWhite = true;
            nextKey.number = i;
            nextKey.x = nextWhiteKeyX;

            keysInfo[i] = nextKey;

            nextWhiteKeyX += whiteKeyOffset;
        }

        currentIndex++;
    }
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
    drawActiveNotes(g, getActiveMidiNotes());
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
        // piano shadow line color 1
        g.setColour(Colour(230, 230, 230));
        g.drawLine(currentDelimiterX - 1, 0, currentDelimiterX - 1, Settings::PIANO_ROLL_HEIGHT - 1, 1);
        // piano shadow line color 2
        g.setColour(Colour(215, 215, 215));
        g.drawLine(currentDelimiterX + 1, 0, currentDelimiterX + 1, Settings::PIANO_ROLL_HEIGHT - 1, 1);

        currentDelimiterX += Settings::PIANO_ROLL_WHITE_NOTE_WIDTH;
    }
}

void drawBlackNotes(Graphics &g)
{
    // pattern from C#
    int blackKeyPattern[] = { 2, 1, 2, 1, 1 };
    static constexpr auto blackKeyOffset = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH;

    //g.setGradientFill(Settings::PIANO_ROLL_BLACK_KEY_GRADIENT);

    auto currentBlackKeyCoord = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH - Settings::PIANO_ROLL_BLACK_NOTE_WIDTH / 2;
    auto currentPatternIndex = 0;
    while (currentBlackKeyCoord < Settings::PIANO_ROLL_WIDTH) {
        drawBlackNote(currentBlackKeyCoord, g);

        // cycling through pattern array
        // currentIndex+1 used, so index 4 is out of bounds
        if (currentPatternIndex == 4) {
            currentPatternIndex = -1;
        }

        currentBlackKeyCoord += blackKeyOffset * blackKeyPattern[++currentPatternIndex];
    }

}

void drawBlackNote(int coord, Graphics &g)
{
    static auto shapeOffsetX = Settings::PIANO_ROLL_BLACK_NOTE_WIDTH * 0.12;
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
    g.fillRect((float) coord + shapeOffsetX, 0.f, (float) shapeOffsetWidth, (float) shapeOffsetHeight);

    g.setColour(Settings::PIANO_ROLL_BLACK_NOTE_HIGHLIGHTS_COLOR);
    auto shapeLineX = coord + Settings::PIANO_ROLL_BLACK_NOTE_WIDTH - shapeOffsetX;

    g.drawLine(shapeLineX, 0, shapeLineX, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT - shapeOffsetY);
    g.drawLine(coord + shapeOffsetX, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT - shapeOffsetY,
               coord + shapeOffsetX + shapeOffsetWidth, Settings::PIANO_ROLL_BLACK_NOTE_HEIGHT - shapeOffsetY);


}

void PianoRoll::drawActiveNotes(Graphics& g, std::vector<int> activeNotes)
{
    // its first key, so it always the same
    // C2
    static auto minNoteNumber = FIRST_VISIBLE_KEY;

    float keyCoordX = 0;
    for (auto noteNumber : activeNotes) {
        if (noteNumber < minNoteNumber) {
            continue;
        }

        keyCoordX = keysInfo[noteNumber].x;
        if (keyCoordX > Settings::PIANO_ROLL_WIDTH) {
            continue;
        }

        drawActiveNoteMask(keysInfo[noteNumber], g);
    }
}

void PianoRoll::mouseDown(const MouseEvent &event)
{
    auto position = event.getPosition();
    auto keyNumber = getKeyClicked(position);
    midiConsumer.noteOn(keyNumber);
}

void PianoRoll::mouseUp(const MouseEvent &event)
{
    auto position = event.getPosition();
    auto keyNumber = getKeyClicked(position);
    midiConsumer.noteOff(keyNumber);
}

int PianoRoll::getKeyClicked(Point<int> point)
{
    for (int i = FIRST_VISIBLE_KEY; i < MAX_KEYS; i++) {
        if (keysInfo[i].contains(point)) {
            return keysInfo[i].number;
        }
    }
}

void drawActiveNoteMask(KeyInfo keyInfo, Graphics& g)
{
    g.setColour(Settings::PIANO_ROLL_ACTIVE_KEY_MASK_COLOR);
    g.fillRect((float) keyInfo.x, (float) 0, (float) keyInfo.width, (float) keyInfo.height);
}

void drawNoteTips(Graphics& g)
{
    static constexpr auto spaceBetweenCNotes = Settings::PIANO_ROLL_WHITE_NOTE_WIDTH * 7;

    // produces a lot of leaks =(
    //g.setFont(Settings::PIANO_ROLL_TIPS_FONT);
    g.setColour(Settings::PIANO_ROLL_NOTE_TIPS_COLOR);

    auto currentCNote = 2;
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

std::vector<int> PianoRoll::getActiveMidiNotes()
{
    return midiSupplier.getActiveNotes();
}

