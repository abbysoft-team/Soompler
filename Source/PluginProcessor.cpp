/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Strings.h"
#include "juce_audio_formats/codecs/juce_MP3AudioFormat.h"

//==============================================================================
SoomplerAudioProcessor::SoomplerAudioProcessor() : AudioProcessor (BusesProperties()
                                                   .withInput  ("Input",  AudioChannelSet::stereo(), true)
                                                   .withOutput ("Output", AudioChannelSet::stereo(), true)),
                                                   loadedSample(nullptr)
{
    synth.addVoice(new SamplerVoice());

    formatManager.registerBasicFormats();
    formatManager.registerFormat(new MP3AudioFormat());

    currentSample = -1;                             // -1 means that it wont played until someone turn on note
}

SoomplerAudioProcessor::~SoomplerAudioProcessor()
{
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SoomplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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

    if (currentSample == 0) {
        synth.noteOn(0, 0x40, 100);
        currentSample == -1;
    }

    auto busCount = getBusCount(false);
    for (auto busIndex = 0; busIndex < busCount; ++busIndex) {
        auto audioBusBuffer = getBusBuffer(buffer, false, busIndex);
        auto midiChannelBuffer = filterMidiMessagesForChannel(midiMessages, busIndex + 1);
        synth.renderNextBlock(audioBusBuffer, midiChannelBuffer, 0, audioBusBuffer.getNumSamples());
    }
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
    if (sound != nullptr)
    {
        synth.addSound(sound);
    } else {
        loadedSample = nullptr;
    }
}

void SoomplerAudioProcessor::playSample()
{
    if (loadedSample == nullptr)
    {
        DBG("loaded sample in nullptr and cannot be played");
        return;
    }

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

    std::unique_ptr<AudioFormatReader> formatReader(
                format->createReaderFor(soundBuffer, true));

    BigInteger midiNotes;
    midiNotes.setRange(0, 126, true);
    return new SamplerSound(sampleFile->getFileName(), *formatReader, midiNotes, 0x40, 0.0, 0.0, 10.0);
}

AudioFormat *SoomplerAudioProcessor::getFormatForFileOrNullptr(File *sampleFile)
{
    AudioFormat* format = formatManager.findFormatForFileExtension(sampleFile->getFileExtension());

    if (format == nullptr) {
        DBG(sampleFile->getFileExtension());
        NativeMessageBox::showMessageBox(
                    AlertWindow::AlertIconType::WarningIcon,
                    UNSUPPORTED_FILE_EXTENSION_ERROR_TITLE,
                    UNSUPPORTED_FILE_EXTENSION_ERROR_MESSAGE);
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



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoomplerAudioProcessor();
}
