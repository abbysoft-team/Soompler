/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Strings.h"
#include "ExtendedSampler.h"

//==============================================================================
SoomplerAudioProcessor::SoomplerAudioProcessor() : AudioProcessor (BusesProperties()
                                                   .withOutput ("Output", AudioChannelSet::stereo(), true)),
                                                   ChangeListener(),
                                                   thumbnailCache(5),
                                                   thumbnail(512, formatManager, thumbnailCache),
                                                   startSample(0),
                                                   endSample(0),
                                                   volume(0.5)
{
    auto listener = std::shared_ptr<ChangeListener>((ChangeListener*) this);
    synth.addVoice(new soompler::ExtendedVoice(listener));
    synth.setCurrentPlaybackSampleRate(44100);

    formatManager.registerBasicFormats();

    transportSource.addChangeListener(this);

    currentSample = 0;                             // -1 means that it wont played until someone turn on note
}

SoomplerAudioProcessor::~SoomplerAudioProcessor()
{
    transportSource.releaseResources();
}

//==============================================================================
const String SoomplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoomplerAudioProcessor::acceptsMidi() const
{
    return true;
}

bool SoomplerAudioProcessor::producesMidi() const
{
    return false;
}

bool SoomplerAudioProcessor::isMidiEffect() const
{
    return false;
}

double SoomplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoomplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoomplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoomplerAudioProcessor::setCurrentProgram (int index)
{
}

const String SoomplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SoomplerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SoomplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SoomplerAudioProcessor::releaseResources()
{
    transportSource.releaseResources();
}

bool SoomplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    return true;
}

void SoomplerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    if (readerSource.get() == nullptr) {
        buffer.clear();
        return;
    }

    if (midiMessages.isEmpty() && !synth.getVoice(0)->isVoiceActive()) {
        processTransport(buffer);
        return;
    }

    // TODO maybe this all multi-bus thing is unnesesary?
//    auto busCount = getBusCount(false);
//    for (auto busNr = 0; busNr < busCount; ++busNr) {
//        auto midiChannelBuffer = filterMidiMessagesForChannel(MidiBuffer, busNr + 1);
//        auto audioBusBuffer = getBusBuffer(buffer, false, busNr);

//        synth.renderNextBlock();
//    }

    lastMidiEvents = filterMidiMessagesForChannel(midiMessages, 1);
    auto audioBusBuffer = getBusBuffer(buffer, false, 0);

    synth.renderNextBlock(audioBusBuffer, lastMidiEvents, 0, audioBusBuffer.getNumSamples());
}

void SoomplerAudioProcessor::processTransport(AudioBuffer<float>& buffer)
{
    AudioSourceChannelInfo audioSource;
    audioSource.buffer = &buffer;
    audioSource.startSample = 0;
    audioSource.numSamples = buffer.getNumSamples();
    transportSource.getNextAudioBlock(audioSource);
}

void SoomplerAudioProcessor::setSampleStartPosition(int64 sample)
{
    startSample = sample;
    transportSource.setNextReadPosition(startSample);

    auto voice = static_cast<soompler::ExtendedVoice*> (synth.getVoice(0));
    voice->setStartSample(sample);
}

void SoomplerAudioProcessor::setSampleEndPosition(int64 sample)
{
    endSample = sample;

    auto voice = static_cast<soompler::ExtendedVoice*> (synth.getVoice(0));
    voice->setEndSample(sample);
}

void SoomplerAudioProcessor::setVolume(double volume)
{
    this->volume = volume;
    transportSource.setGain(volume);

    auto voice = static_cast<soompler::ExtendedVoice*>(synth.getVoice(0));
    voice->setVolume(volume);
}

void SoomplerAudioProcessor::notifyTransportStateChanged(TransportState state)
{
    if (transportStateListener == nullptr) {
        return;
    }

    transportStateListener->transportStateChanged(state);
}

std::vector<int> SoomplerAudioProcessor::getActiveNotes()
{
    std::vector<int> result;

    // TODO replace when multi-voice synth is realized
    int noteNumber = synth.getVoice(0)->getCurrentlyPlayingNote();

    if (noteNumber >= 0) {
        result.push_back(noteNumber);
    }

    return result;
}

void SoomplerAudioProcessor::noteOn(int noteNumber)
{
    synth.noteOn(0, noteNumber, volume);
}

void SoomplerAudioProcessor::noteOff(int noteNumber)
{
    synth.noteOff(0, noteNumber, volume, true);
}

MidiBuffer SoomplerAudioProcessor::filterMidiMessagesForChannel(const MidiBuffer &input, int channel)
{
    MidiBuffer output;
    int samplePosition;
    MidiMessage msg;

    for (MidiBuffer::Iterator it(input); it.getNextEvent(msg, samplePosition);)
    {
        if (msg.getChannel() == channel) output.addEvent(msg, samplePosition);
    }

    return output;
}

//==============================================================================
bool SoomplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SoomplerAudioProcessor::createEditor()
{
    return new SoomplerAudioProcessorEditor (*this);
}

//==============================================================================
void SoomplerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SoomplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SoomplerAudioProcessor::loadSample(File sampleFile)
{
    this->loadedSample = sampleFile;

    auto sound = getSampleData(loadedSample);
    if (sound != nullptr)
    {
        synth.removeSound(0);
        synth.addSound(sound);
    }

    setSampleStartPosition(0);
    setSampleEndPosition(transportSource.getTotalLength());
    notifyTransportStateChanged(TransportState::Ready);
}

void SoomplerAudioProcessor::playSample()
{
    if (!loadedSample.has_value())
    {
        DBG("sample is not loaded and cannot be played");
        return;
    }

    changeTransportState(Starting);
}

void SoomplerAudioProcessor::stopSamplePlayback()
{
    changeTransportState(Stopping);
}

void SoomplerAudioProcessor::setTransportStateListener(TransportStateListener* listener)
{
    this->transportStateListener = listener;
}

double SoomplerAudioProcessor::getCurrentAudioPosition()
{
    if (synth.getVoice(0)->isVoiceActive()) {
        return getSynthCurrentPosition();
    }
    return transportSource.getCurrentPosition();
}

void SoomplerAudioProcessor::updateTransportState()
{
    if (!transportSource.isPlaying()) {
        // sample is not in listen mode
        return;
    }

    if (transportSource.getNextReadPosition() >= endSample) {
        // TODO handle when looping on
        transportSource.stop();
    }
}

SynthesiserSound::Ptr SoomplerAudioProcessor::getSampleData(std::optional<File> sampleFile)
{
    //auto* soundBuffer = sampleFile->createInputStream();
    AudioFormat* format = getFormatForFileOrNullptr(sampleFile);
    if (format == nullptr)
    {
        return nullptr;
    }

    auto formatReader = formatManager.createReaderFor(*sampleFile);

    setTransportSource(formatReader);
    thumbnail.setSource(new FileInputSource(*sampleFile));

    BigInteger midiNotes;
    midiNotes.setRange(0, 126, true);
    return new soompler::ExtendedSound(sampleFile->getFileName(), *formatReader, midiNotes, 0x48, 0.0, 0.0, Settings::MAX_SAMPLE_LENGTH);
}

AudioFormat *SoomplerAudioProcessor::getFormatForFileOrNullptr(std::optional<File> sampleFile)
{
    AudioFormat* format = formatManager.findFormatForFileExtension(sampleFile->getFileExtension());

    if (format == nullptr) {
        DBG(sampleFile->getFileExtension());
        NativeMessageBox::showMessageBox(
                    AlertWindow::AlertIconType::WarningIcon,
                    Strings::UNSUPPORTED_FILE_EXTENSION_ERROR_TITLE,
                    Strings::UNSUPPORTED_FILE_EXTENSION_ERROR_MESSAGE);
    }

    return format;
}

void SoomplerAudioProcessor::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
        {
            changeTransportState(Playing);
        } else {
            changeTransportState(Stopped);
        }
    }
}

void SoomplerAudioProcessor::changeTransportState(TransportState newState)
{
    if (transportState == newState)
    {
        return;
    }


    transportState = newState;

    notifyTransportStateChanged(newState);

    switch (transportState)
    {
        case Playing:
            break;
        case Starting:
            transportSource.start();
            break;
        case Stopping:
        case Stopped:
            transportSource.stop();
            transportSource.setNextReadPosition(startSample);
            break;
        default:
            break;
   }
}

void SoomplerAudioProcessor::setTransportSource(AudioFormatReader* reader)
{
    auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
}

double SoomplerAudioProcessor::getSynthCurrentPosition()
{
    auto voice = static_cast<soompler::ExtendedVoice*>(synth.getVoice(0));
    return voice->getCurrentPosition();
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoomplerAudioProcessor();
}
