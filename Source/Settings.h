#ifndef SOOMPLER_SOURCE_SETTINGS_H
#define SOOMPLER_SOURCE_SETTINGS_H

#include <JuceHeader.h>

namespace Settings {

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 300;
const auto BUTTON_OPEN_FILE_POSITION = Rectangle<int>(WINDOW_WIDTH/2-50, 233, 100, 40);
const auto BUTTON_PLAY_SAMPLE_POSITION = Rectangle<int>(WINDOW_WIDTH/2-50, 180, 100, 40);

const auto SAMPLE_NAME_COLOR = Colours::black;
constexpr auto SAMPLE_NAME_TEXT_X = (WINDOW_WIDTH / 2);
constexpr auto SAMPLE_NAME_TEXT_Y = 120;
constexpr auto SAMPLE_NAME_FONT_SIZE = 17.0f;

}

#endif //SOOMPLER_SOURCE_SETTINGS_H
