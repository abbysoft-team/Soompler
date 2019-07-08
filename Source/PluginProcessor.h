/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TransportStateListener.h"
#include "MidiEventSupplier.h"
#include "MidiEventConsumer.h"
#include "TransportInfo.h"
#include "SampleInfo.h"

//==============================================================================
/**
*/
class SoomplerAudioProcessor  :
        public AudioProcessor,
        ChangeListener,
        public MidiEventSupplier,
        public MidiEventConsumer,
        public TransportInfoOwner,
        public SampleInfoListener
{
public:
    //==============================================================================
    SoomplerAudioProcessor();
    ~SoomplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    std::optional<File> getLoadedSample() const {
        return loadedSample;
    }

    AudioThumbnail& getThumbnail() {
        return thumbnail;
    }

    void loadSample(File);

    void playSample();

    void stopSamplePlayback();

    void setTransportStateListener(TransportStateListener*);

    double getCurrentAudioPosition();

    int64 getTotalLengthOfSample() const {
        return transportSource.getTotalLength();
    }

    void updateTransportState();

    void processTransport(AudioBuffer<float>& buffer);

    // percent of a sample length
    void setSampleStartPosition(int64 sample);
    void setSampleEndPosition(int64 sample);
    void newSampleInfoRecieved(std::shared_ptr<SampleInfo> info);

    void setVolume(double volume);

    void notifyTransportStateChanged(TransportState state);

    std::vector<int> getActiveNotes() override;

    void noteOn(int noteNumber);
    void noteOff(int noteNumber);

    std::shared_ptr<TransportInfo> getTransportInfo() override;

    void setSampleInfoListener(std::shared_ptr<SampleInfoListener> sampleInfoListener);

private:
    //==============================================================================

    std::optional<File> loadedSample;
    Synthesiser synth;
    std::unique_ptr<SynthesiserSound> synthSound;
    int currentSample;

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    TransportState transportState;
    TransportStateListener* transportStateListener;

    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;

    int64 startSample;
    int64 endSample;

    MidiBuffer lastMidiEvents;

    double volume;

    SampleInfo sampleInfo;
    std::shared_ptr<SampleInfoListener> sampleInfoListener;

    SynthesiserSound::Ptr getSampleData(std::optional<File> sampleFile);
    AudioFormat* getFormatForFileOrNullptr(std::optional<File> sampleFile);
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void changeTransportState(TransportState newState);
    void setTransportSource(AudioFormatReader*);
    double getSynthCurrentPosition();
    MidiBuffer filterMidiMessagesForChannel(const MidiBuffer &input, int channel);
    void notifySampleInfoListeners();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoomplerAudioProcessor)
};
