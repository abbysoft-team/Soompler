#ifndef SOOMPLER_SOURCE_SETTINGS_H
#define SOOMPLER_SOURCE_SETTINGS_H

#include <JuceHeader.h>

namespace Settings {

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 300;
const auto BUTTON_OPEN_FILE_POSITION = Rectangle<int>(WINDOW_WIDTH/2-100, 233, 100, 40);
const auto BUTTON_PLAY_SAMPLE_POSITION = Rectangle<int>(WINDOW_WIDTH/2 + 5, 233, 100, 40);
const auto THUMBNAIL_BOUNDS = Rectangle<int>(10, 100, WINDOW_WIDTH - 20, 100);
const auto THUMBNAIL_HEADER_BOUNDS = Rectangle<int>(10, 80, WINDOW_WIDTH - 20, 20);

const auto THUMBNAIL_COLOR = Colour(23, 54, 38);
const auto THUMBNAIL_HEADER_COLOR = Colour((uint8) 255, (uint8) 128, (uint8) 128, (uint8) 200);
const auto THUMBNAIL_BOUNDS_COLOR = Colour(256, 128, 128);
const auto SAMPLE_NAME_COLOR = THUMBNAIL_COLOR;
const auto POSITION_LINE_COLOR = THUMBNAIL_HEADER_COLOR;
const auto RANGE_LINES_COLOR = Colour((uint8) 246, (uint8) 66, (uint8) 66, (uint8) 200);
const auto NOT_ACTIVE_SAMPLE_REGION_MASK_COLOR = Colour((uint8) 0, (uint8) 0, (uint8) 0, (uint8) 50);

constexpr auto SAMPLE_NAME_TEXT_X = (WINDOW_WIDTH / 2);
constexpr auto SAMPLE_NAME_TEXT_Y = 95;
constexpr auto SAMPLE_NAME_FONT_SIZE = 17.0f;
constexpr auto MAX_SAMPLE_NAME_LENGTH = 60;

}

#endif //SOOMPLER_SOURCE_SETTINGS_H
