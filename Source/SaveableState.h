/*
  ==============================================================================

    SaveableState
    Created: 14 Aug 2019 12:35:13pm
    Author:  artyom

  ==============================================================================
*/

#pragma once

#include "StateBundle.h"

class SaveableState {
public:
    virtual void saveStateToMemory(StateBundle &bundle) = 0;
    virtual void getStateFromMemory(StateBundle &bundle) = 0;

    ~SaveableState() = default;
};
