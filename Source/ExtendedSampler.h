/*
  ==============================================================================

    ExtendedSound.h
    Created: 27 Jun 2019 4:53:19pm
    Author:  AbbySoft

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace soompler {

/**
    Wrapper around SamplerSound

    @see SamplerVoice, Synthesiser, SynthesiserSound

    @tags{Audio}
*/
class ExtendedSound : public SynthesiserSound
{
public:
    //==============================================================================
    ExtendedSound (const String& name,
                  AudioFormatReader& source,
                  const BigInteger& midiNotes,
                  int midiNoteForNormalPitch,
                  double attackTimeSecs,
                  double releaseTimeSecs,
                  double maxSampleLengthSeconds);

    /** Destructor. */
    ~ExtendedSound() override;

    //==============================================================================
    /** Returns the sample's name */
    const String& getName() const noexcept                  { return name; }

    /** Returns the audio sample data.
        This could return nullptr if there was a problem loading the data.
    */
    AudioBuffer<float>* getAudioData() const noexcept       { return data.get(); }

    //==============================================================================
    /** Changes the parameters of the ADSR envelope which will be applied to the sample. */
    void setEnvelopeParameters (ADSR::Parameters parametersToUse)    { params = parametersToUse; }

    //==============================================================================
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;

private:
    //==============================================================================
    friend class ExtendedVoice;

    String name;
    std::unique_ptr<AudioBuffer<float>> data;
    double sourceSampleRate;
    BigInteger midiNotes;
    int length = 0, midiRootNote = 0;

    ADSR::Parameters params;

    JUCE_LEAK_DETECTOR (ExtendedSound)
};


//==============================================================================
/**
    Wrapper around ExtendedSound

    @see SamplerSound, Synthesiser, SynthesiserVoice

    @tags{Audio}
*/
class JUCE_API  ExtendedVoice    : public SynthesiserVoice
{
public:
    //==============================================================================
    ExtendedVoice();

    /** Destructor. */
    ~ExtendedVoice() override;

    //==============================================================================
    bool canPlaySound (SynthesiserSound*) override;

    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int pitchWheel) override;
    void stopNote (float velocity, bool allowTailOff) override;

    void pitchWheelMoved (int newValue) override;
    void controllerMoved (int controllerNumber, int newValue) override;

    void renderNextBlock (AudioBuffer<float>&, int firstSampleToPlay, int numSamples) override;

    void setStartSample(int64 sample);
    void setEndSample(int64 sample);

private:
    //==============================================================================
    double pitchRatio = 0;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;

    int64 firstSampleToPlay = 0;
    int64 endSample = 0;

    ADSR adsr;

    JUCE_LEAK_DETECTOR (ExtendedVoice)
};

}
