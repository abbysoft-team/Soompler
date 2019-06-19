/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Strings.h"

//==============================================================================
SoomplerAudioProcessor::SoomplerAudioProcessor() : AudioProcessor (BusesProperties()
                                                   .withOutput ("Output", AudioChannelSet::stereo(), true)),
                                                   ChangeListener(),
                                                   loadedSample(nullptr)
{
    synth.addVoice(new SamplerVoice());
    synth.setCurrentPlaybackSampleRate(44100);

    formatManager.registerBasicFormats();

    transportSource.addChangeListener(this);

    currentSample = -1;                             // -1 means that it wont played until someone turn on note
}

SoomplerAudioProcessor::~SoomplerAudioProcessor()
{
    delete loadedSample;
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
    if (readerSource.get() == nullptr)
    {
        buffer.clear();
        return;
    }

    AudioSourceChannelInfo audioSource;
    audioSource.buffer = &buffer;
    audioSource.startSample = 0;
    audioSource.numSamples = buffer.getNumSamples();
    transportSource.getNextAudioBlock(audioSource);
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
    this->loadedSample = new File(sampleFile);

    synth.removeSound(0);
    SynthesiserSound::Ptr sound = getSampleData(loadedSample);
//    if (sound != nullptr)
//    {
//        synth.addSound(sound);
//    } else {
//        loadedSample = nullptr;
//    }
}

void SoomplerAudioProcessor::playSample()
{
    if (loadedSample == nullptr)
    {
        DBG("loaded sample in nullptr and cannot be played");
        return;
    }

    changeTransportState(Starting);

    currentSample = 0;
}

SynthesiserSound::Ptr SoomplerAudioProcessor::getSampleData(File* sampleFile)
{
    auto* soundBuffer = sampleFile->createInputStream();
    AudioFormat* format = getFormatForFileOrNullptr(sampleFile);
    if (format == nullptr)
    {
        return nullptr;
    }

    auto formatReader = formatManager.createReaderFor(*sampleFile);

    setTransportSource(formatReader);

//    BigInteger midiNotes;
//    midiNotes.setRange(0, 126, true);
//    return new SamplerSound(sampleFile->getFileName(), *formatReader, midiNotes, 0x40, 0.0, 0.0, 10.0);
    return nullptr;
}

AudioFormat *SoomplerAudioProcessor::getFormatForFileOrNullptr(File *sampleFile)
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
    if (transportState != newState)
    {
        transportState = newState;

        switch (transportState)
        {
            case Playing:
                break;
            case Starting:
                transportSource.start();
                break;
            default:
                break;
        }
    }
}

void SoomplerAudioProcessor::setTransportSource(AudioFormatReader* reader)
{
    auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoomplerAudioProcessor();
}
