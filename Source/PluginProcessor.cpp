#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
StoneMistressAudioProcessor::StoneMistressAudioProcessor()
    : parameters(*this, nullptr, "STONEMISTRESS_PARAMS", Parameters::createParameterLayout()),
    lfo(Parameters::defaultRate),
    modulator(Parameters::defaultPhaserDepth, Parameters::defaultChorusDepth),
    phaser()
{
    Parameters::addListenerToAllParameters(parameters, this);
}

StoneMistressAudioProcessor::~StoneMistressAudioProcessor()
{
}

//==============================================================================
void StoneMistressAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    drywet.prepareToPlay(samplesPerBlock);
    lfo.prepareToPlay(sampleRate);
    phaserModulationBuffer.setSize(2, samplesPerBlock);
    chorusModulationBuffer.setSize(2, samplesPerBlock);
    modulator.prepareToPlay(sampleRate);
    smallStoneBuffer.setSize(2, samplesPerBlock);
    smallStoneBuffer.clear();
    phaser.prepareToPlay(sampleRate);
    chorus.prepareToPlay(sampleRate, samplesPerBlock);
}

void StoneMistressAudioProcessor::releaseResources()
{
    phaserModulationBuffer.setSize(0, 0);
    chorusModulationBuffer.setSize(0, 0);
    smallStoneBuffer.setSize(0, 0);
    chorus.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StoneMistressAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void StoneMistressAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto inCh  = getTotalNumInputChannels();
    auto outCh = getTotalNumOutputChannels();

    auto const numSamples = buffer.getNumSamples();
    auto const numCh = buffer.getNumChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = inCh; i < outCh; ++i)
        buffer.clear (i, 0, numSamples);

    // TO DO LIST.

    // 1. Generate the sine LFO Signal for one unit.
    lfo.getNextAudioBlock(phaserModulationBuffer, numSamples);
    // 2. Copy the same LFO signal into another buffer.
    for (int ch = 0; ch < 2; ++ch)
    {
        chorusModulationBuffer.copyFrom(ch, 0, phaserModulationBuffer, ch, 0, numSamples);
    }
    // 2. LFO Signal is scaled accordingly.
    modulator.processBlock(phaserModulationBuffer, numSamples, "p");
    modulator.processBlock(chorusModulationBuffer, numSamples, "c");
    // 3. Set modulation bounds for the Chorus buffer.
    FloatVectorOperations::min(chorusModulationBuffer.getWritePointer(0), chorusModulationBuffer.getWritePointer(0), Parameters::maxDelayTime, numSamples);
    FloatVectorOperations::min(chorusModulationBuffer.getWritePointer(1), chorusModulationBuffer.getWritePointer(1), Parameters::maxDelayTime, numSamples);
    // 4. Make copies of the main buffer.
    drywet.copyDrySignal(buffer);
    
    for (int ch = 0; ch < numCh; ++ch)
    {
        smallStoneBuffer.copyFrom(ch, 0, buffer, ch, 0, numSamples);
    }

    // 5. Feed the buffers in the respective processing units and PROCESS!
    phaser.processBlock(smallStoneBuffer, phaserModulationBuffer, numSamples);
    chorus.processBlock(buffer, chorusModulationBuffer);
    // 6. Mix everything.
    drywet.mixDrySignal(buffer, smallStoneBuffer);

}
//==============================================================================
bool StoneMistressAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StoneMistressAudioProcessor::createEditor()
{
    return new StoneMistressAudioProcessorEditor (*this);
}

//==============================================================================
void StoneMistressAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void StoneMistressAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void StoneMistressAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == Parameters::nameRate)
    {
        lfo.setRate(newValue);
    }

    if (paramID == Parameters::namePhaserDepth)
    {
        modulator.setPhaserDepth(newValue);
    }

    if (paramID == Parameters::nameChorusDepth)
    {
        modulator.setChorusDepth(newValue);
    }

    if (paramID == Parameters::nameColor)
    {
        phaser.setColor();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StoneMistressAudioProcessor();
}
