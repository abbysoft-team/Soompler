/*
  ==============================================================================

    MidiEventSupplier
    Created: 28 Jun 2019 5:02:44pm
    Author:  apopov

  ==============================================================================
*/

#pragma once

/**
 * Interface for midi event producer
 *
 * @brief The MidiEventSupplier class
 */
class MidiEventSupplier {
public:
    virtual std::vector<int> getActiveNotes() = 0;
};
