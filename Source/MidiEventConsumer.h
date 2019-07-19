/*
  ==============================================================================

    MidiEventConsumer.h
    Created: 29 Jun 2019 2:09:37pm
    Author:  apopov

  ==============================================================================
*/

#pragma once

class MidiEventConsumer
{
public:
    virtual void noteOn(int noteNumber) = 0;
    virtual void noteOff(int noteNumber) = 0;

    virtual void setRootNote(int rootNote) = 0;
    virtual void setNoteRange(const BigInteger& notes) = 0;
    
    virtual ~MidiEventConsumer() = default;
};
