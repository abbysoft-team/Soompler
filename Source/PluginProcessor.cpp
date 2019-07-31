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
                                                   stateManager(*this, nullptr, Identifier("SoomplerState"), createParametersLayout()),
                                                   currentSample(0),
                                                   loopMode(false),
                                                   reverse(false),
                                                   thumbnailCache(5),
                                                   thumbnail(256, formatManager, thumbnailCache),
                                                   startSample(0),
                                                   endSample(0)
{    
    synth.addVoice(new soompler::ExtendedVoice(this));
    synth.setCurrentPlaybackSampleRate(44100);

    formatManager.registerBasicFormats();

    transportSource.addChangeListener(this);

    volume = *(stateManager.getRawParameterValue("volume"));
}

AudioProcessorValueTreeState::ParameterLayout SoomplerAudioProcessor::createParametersLayout()
{

    AudioProcessorValueTreeState::ParameterLayout parameterLayout = {
        std::make_unique<AudioParameterFloat>("volume", TRANS("Volume\n"), 0.0f, 1.0f, 0.5f),
        std::make_unique<AudioParameterFloat>("attack", TRANS("Attack\n"), 0.0f, Settings::MAX_ATTACK_TIME, 0.0f),
        std::make_unique<AudioParameterFloat>("decay", TRANS("Decay\n"), 0.0f, Settings::MAX_DECAY_TIME, 0.0f),
        std::make_unique<AudioParameterFloat>("sustain", TRANS("Sustain\n"), 0.0f, 1.0f, 1.0f),
        std::make_unique<AudioParameterFloat>("release", TRANS("Release\n"), 0.0f, Settings::MAX_RELEASE_TIME, 0.0f),
    };
    
    return parameterLayout;
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

//    // ADSR
//    auto voice = static_cast<soompler::ExtendedVoice*>(synth.getVoice(0));
//    auto adsr = voice->getAdsr();

//    auto adsrValue = adsr.getNextSample();

    transportSource.getNextAudioBlock(audioSource);
}

std::shared_ptr<File> SoomplerAudioProcessor::getLoadedSample() const {
    return loadedSample;
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

void SoomplerAudioProcessor::newSampleInfoRecieved(std::shared_ptr<SampleInfo> info)
{
    this->sampleInfo = info;
    setSampleStartPosition(info->startSample);
    setSampleEndPosition(info->endSample);
}

//TODO remove
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
    // set current adsr params
    auto adsr = ADSR::Parameters();
    adsr.attack = getFloatParameter("attack");
    adsr.decay = getFloatParameter("decay");
    adsr.sustain = getFloatParameter("sustain");
    adsr.release = getFloatParameter("release");

    auto sound = static_cast<soompler::ExtendedSound*>(synth.getSound(0).get());
    sound->setAdsrParams(adsr);

    synth.noteOn(0, noteNumber, getFloatParameter("volume"));
}

void SoomplerAudioProcessor::noteOff(int noteNumber)
{
    synth.noteOff(0, noteNumber, getFloatParameter("volume"), true);
}

void SoomplerAudioProcessor::setRootNote(int rootNote)
{
    auto sound = static_cast<soompler::ExtendedSound*> (synth.getSound(0).get());
    sound->setRootNote(rootNote);
}

void SoomplerAudioProcessor::setNoteRange(const BigInteger &noteRange)
{
    auto sound = static_cast<soompler::ExtendedSound*> (synth.getSound(0).get());
    sound->setMidiRange(noteRange);
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
    // save state
    ValueTree loadedSampleNameValue = ValueTree("loadedSampleName");
    ValueTree loadedSampleLengthValue = ValueTree("loadedSampleLength");
    ValueTree startSampleValue = ValueTree("startSample");
    ValueTree endSampleValue = ValueTree("endSample");
    ValueTree reverseValue = ValueTree("reverse");
    ValueTree loopModeValue = ValueTree("loopMode");

    stateManager.state.appendChild(loadedSampleNameValue, nullptr);
    stateManager.state.appendChild(loadedSampleLengthValue, nullptr);
    stateManager.state.appendChild(startSampleValue, nullptr);
    stateManager.state.appendChild(endSampleValue, nullptr);
    stateManager.state.appendChild(loopModeValue, nullptr);
    stateManager.state.appendChild(reverseValue, nullptr);

    if (loadedSample != nullptr) {
        stateManager.state.setProperty("loadedSampleName", loadedSample->getFileName(), nullptr);
    }
    if (sampleInfo != nullptr) {
        stateManager.state.setProperty("loadedSampleLength", sampleInfo->lengthInSeconds, nullptr);
    }
    
    stateManager.state.setProperty("startSample", startSample, nullptr);
    stateManager.state.setProperty("endSample", endSample, nullptr);
    stateManager.state.setProperty("loopMode", loopMode, nullptr);
    stateManager.state.setProperty("reverse", reverse, nullptr);

    auto state = stateManager.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SoomplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes - sizeof(bool)));

    if (xmlState.get() == nullptr) {
        return;
    }

    if (xmlState->hasTagName(stateManager.state.getType())) {
        stateManager.replaceState(ValueTree::fromXml(*xmlState));
    }

    // restore variables
    String sampleName = stateManager.state.getPropertyAsValue("loadedSampleName", nullptr).getValue();
    float sampleLength = stateManager.state.getPropertyAsValue("loadedSampleLength", nullptr).getValue();
    int64 startSample = stateManager.state.getPropertyAsValue("startSample", nullptr).getValue();
    int64 endSample = stateManager.state.getPropertyAsValue("endSample", nullptr).getValue();
    bool looping = stateManager.state.getPropertyAsValue("loopMode", nullptr).getValue();
    bool reverse = stateManager.state.getPropertyAsValue("reverse", nullptr).getValue();

    sampleInfo = std::make_shared<SampleInfo>(sampleLength, getSampleRate(), sampleName);
    sampleInfo->startSample = startSample;
    sampleInfo->endSample = endSample;

    this->startSample = startSample;
    this->endSample = endSample;
    this->loopMode = looping;
    this->reverse = reverse;
}

void SoomplerAudioProcessor::loadSample(File sampleFile)
{
    this->loadedSample = std::make_shared<File>(sampleFile);

    auto sound = getSampleData(loadedSample);
    if (sound != nullptr)
    {
        synth.removeSound(0);
        synth.addSound(sound);
    }

    setSampleStartPosition(0);
    setSampleEndPosition(transportSource.getTotalLength());
    notifyTransportStateChanged(TransportState::Ready);

    stateManager.state.setProperty("loadedSample", var(sampleFile.getFullPathName()), nullptr);

    sampleInfo = std::make_shared<SampleInfo>(transportSource.getLengthInSeconds(), getSampleRate(), sampleFile.getFileName());

    notifySampleInfoListeners();
}

void SoomplerAudioProcessor::notifySampleInfoListeners()
{
    for (auto sampleInfoListener : sampleInfoListeners) {
        sampleInfoListener->newSampleInfoRecieved(sampleInfo);
    }
}

void SoomplerAudioProcessor::playSample()
{
    if (loadedSample == nullptr)
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

SynthesiserSound::Ptr SoomplerAudioProcessor::getSampleData(std::shared_ptr<File> sampleFile)
{
    if (sampleFile == nullptr) {
        return nullptr;
    }
    
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
    midiNotes.setRange(Settings::DEFAULT_MIN_NOTE, Settings::DEFAULT_MAX_NOTE - Settings::DEFAULT_MIN_NOTE + 1, true);
    return new soompler::ExtendedSound(sampleFile->getFileName(), *formatReader, midiNotes, Settings::DEFAULT_ROOT_NOTE,
                                       Settings::DEFAULT_ATTACK_TIME, Settings::DEFAULT_RELEASE_TIME, Settings::MAX_SAMPLE_LENGTH);
}

AudioFormat *SoomplerAudioProcessor::getFormatForFileOrNullptr(std::shared_ptr<File> sampleFile)
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
            synth.getVoice(0)->stopNote(0, false);
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

std::shared_ptr<TransportInfo> SoomplerAudioProcessor::getTransportInfo()
{
    auto info = std::make_shared<TransportInfo>(this->getSampleRate());
    info->setAudioPosition(getCurrentAudioPosition());

    return info;
}

void SoomplerAudioProcessor::addSampleInfoListener(std::shared_ptr<SampleInfoListener> sampleInfoListener)
{
    this->sampleInfoListeners.push_back(sampleInfoListener);
}

void SoomplerAudioProcessor::setAdsrParams(ADSR::Parameters params)
{
    auto sound = static_cast<soompler::ExtendedSound*>(synth.getSound(0).get());
    if (sound == nullptr) {
        return;
    }

    sound->setAdsrParams(params);
}

void SoomplerAudioProcessor::setLoopEnabled(bool loopEnable) {
    auto voice = static_cast<soompler::ExtendedVoice*> (synth.getVoice(0));
    voice->enableLooping(loopEnable);

    loopMode = loopEnable;
}

void SoomplerAudioProcessor::reverseSample()
{
    auto sound = static_cast<soompler::ExtendedSound*>(synth.getSound(0).get());
    if (sound == nullptr || !thumbnail.isFullyLoaded()) {
        return;
    }

    sound->reverse();
    thumbnail.reset(2, getSampleRate());

    auto newAudioData = sound->getAudioData();
    thumbnail.addBlock(thumbnail.getNumSamplesFinished(), *newAudioData, 0, newAudioData->getNumSamples());

    reverse = !reverse;
}

AudioProcessorValueTreeState &SoomplerAudioProcessor::getStateManager()
{
    return stateManager;
}

float SoomplerAudioProcessor::getFloatParameter(const String &paramId)
{
    return *(stateManager.getRawParameterValue(paramId));
}

std::shared_ptr<SampleInfo> SoomplerAudioProcessor::getCurrentSampleInfo()
{
    return sampleInfo;
}

bool SoomplerAudioProcessor::isLoopModeOn() const {
    return loopMode;
}


bool SoomplerAudioProcessor::isSampleReversed() const {
    return reverse;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoomplerAudioProcessor();
}
