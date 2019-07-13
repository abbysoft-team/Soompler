#ifndef SOOMPLER_SOURCE_SETTINGS_H
#define SOOMPLER_SOURCE_SETTINGS_H

#include <JuceHeader.h>

namespace Settings {

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 340;
const auto BUTTON_OPEN_FILE_POSITION = Rectangle<int>(10, 10, 100, 40);
const auto BUTTON_PLAY_SAMPLE_POSITION = Rectangle<int>(WINDOW_WIDTH/2 + 5, BUTTON_OPEN_FILE_POSITION.getY(), 100, 40);
const auto THUMBNAIL_BOUNDS = Rectangle<int>(10, 100, WINDOW_WIDTH - 20, 100);
constexpr auto THUMBNAIL_HEADER_HEIGHT = 20;
const auto SAMPLE_VIEWER_BOUNDS = Rectangle<int>(10, 100, THUMBNAIL_BOUNDS.getWidth(), THUMBNAIL_BOUNDS.getHeight() + THUMBNAIL_HEADER_HEIGHT);
const auto VOLUME_KNOB_POSITION = Rectangle<int>(70, 210, 50, 50);

const auto THUMBNAIL_COLOR = Colour(23, 54, 38);
const auto THUMBNAIL_HEADER_COLOR = Colour((uint8) 255, 128, 128, 0.7f);
const auto THUMBNAIL_BOUNDS_COLOR = Colour(255, 128, 128);
const auto SAMPLE_NAME_COLOR = THUMBNAIL_COLOR;
const auto POSITION_LINE_COLOR = THUMBNAIL_HEADER_COLOR;
const auto RANGE_LINES_COLOR = Colour((uint8) 246, (uint8) 66, (uint8) 66, (uint8) 200);
const auto NOT_ACTIVE_SAMPLE_REGION_MASK_COLOR = Colour((uint8) 0, (uint8) 0, (uint8) 0, (uint8) 156);

// relative to SampleViewer widget
const auto SAMPLE_NAME_TEXT_X = (THUMBNAIL_BOUNDS.getWidth() / 2);
constexpr auto SAMPLE_NAME_TEXT_Y = (THUMBNAIL_HEADER_HEIGHT / 2) + 4;

constexpr auto SAMPLE_NAME_FONT_SIZE = 17.0f;
constexpr auto MAX_SAMPLE_NAME_LENGTH = 60;
constexpr auto RANGE_LINES_WIDTH = 5.0f;
constexpr int MAX_SAMPLE_LENGTH = 30.0f; // in sec

// piano roll constants
const auto PIANO_ROLL_WHITE_COLOR = Colour(255, 255, 255);
const auto PIANO_ROLL_GRADIENT_COLOR = Colour(200, 200, 200);
const auto PIANO_ROLL_NOTE_TIPS_COLOR = Colour(104, 118, 130);
const auto PIANO_ROLL_DELIMITER_COLOR = Colour(174, 176, 180);
const auto PIANO_ROLL_BLACK_NOTE_COLOR = Colour(20, 20, 20);
const auto PIANO_ROLL_BLACK_NOTE_TOP_COLOR = Colour((uint8) 154, (uint8) 156, (uint8) 160, (uint8) 51);
const auto PIANO_ROLL_BLACK_NOTE_HIGHLIGHTS_COLOR = Colour((uint8) 230, (uint8) 230, (uint8) 230, (uint8) 128);
const auto PIANO_ROLL_ACTIVE_KEY_MASK_COLOR = Colour((uint8) 220, (uint8) 128, (uint8) 128, (uint8) 150);
const auto PIANO_ROLL_TIPS_FONT = Font(10);

constexpr auto PIANO_ROLL_WIDTH = WINDOW_WIDTH;
constexpr auto PIANO_ROLL_HEIGHT = 64;
constexpr auto PIANO_ROLL_WHITE_NOTE_WIDTH = 30;
constexpr auto PIANO_ROLL_BLACK_NOTE_WIDTH = 20;
constexpr auto PIANO_ROLL_BLACK_NOTE_HEIGHT = PIANO_ROLL_HEIGHT * 0.6;
constexpr auto PIANO_ROLL_NOTE_COUNT = PIANO_ROLL_WIDTH / PIANO_ROLL_WHITE_NOTE_WIDTH;
constexpr auto PIANO_ROLL_TIPS_OFFSET_X = 5;
constexpr auto PIANO_ROLL_TIPS_OFFSET_Y = PIANO_ROLL_HEIGHT - 10;

const auto PIANO_ROLL_BOUNDS = Rectangle<int>(0, WINDOW_HEIGHT - PIANO_ROLL_HEIGHT,
                                              PIANO_ROLL_WIDTH, PIANO_ROLL_HEIGHT);

const auto PIANO_ROLL_GRADIENT = ColourGradient(PIANO_ROLL_WHITE_COLOR, Point<float>(PIANO_ROLL_WIDTH / 2, PIANO_ROLL_HEIGHT - 1),
                                                PIANO_ROLL_GRADIENT_COLOR, Point<float>(PIANO_ROLL_WIDTH / 2, 0),
                                                false);

// Main Panel

constexpr auto MAIN_PANEL_HEIGHT = 40;
const auto MAIN_PANEL_GRADIENT = ColourGradient(Colour((uint8) 255, (uint8) 255, (uint8) 255, (uint8) 100),
                                                Point<float>(WINDOW_WIDTH / 2, 0),
                                                Colour((uint8) 150, (uint8) 170, (uint8) 150, (uint8) 100),
                                                Point<float>(WINDOW_WIDTH / 2, MAIN_PANEL_HEIGHT),
                                                false);

// Buttons
const auto BUTTON_OPAQUE_COLOR = Colour((uint8) 0, (uint8) 0, (uint8) 0, (uint8) 0);
const auto BUTTON_OVER_COLOR = Colour((uint8) 255, (uint8) 255, (uint8) 255, (uint8) 65);
const auto BUTTON_DOWN_COLOR = Colour((uint8) 255, (uint8) 255, (uint8) 255, (uint8) 120);

// GUI Editor
const auto GUI_EDITOR_COLOR = THUMBNAIL_COLOR;
const auto GUI_EDITOR_SELECTION_COLOR = Colours::yellow;
constexpr auto GUI_EDITOR_GRID_SIZE = 10;
constexpr auto GUI_EDITOR_SIZE_STEP_COEFF_INCR = 1.05;
constexpr auto GUI_EDITOR_SIZE_STEP_COEFF_DECR = 0.95;

}

#endif //SOOMPLER_SOURCE_SETTINGS_H
