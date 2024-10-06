#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
StoneMistressAudioProcessor::StoneMistressAudioProcessor()
    : parameters(*this, nullptr, "STONEMISTRESS_PARAMS", Parameters::createParameterLayout()),
    lfo(Parameters::defaultRate)
{
    Parameters::addListenerToAllParameters(parameters, this);
}

StoneMistressAudioProcessor::~StoneMistressAudioProcessor()
{
}

//==============================================================================
void StoneMistressAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lfo.prepareToPlay(sampleRate);
    modulation.setSize(2, samplesPerBlock);
}

void StoneMistressAudioProcessor::releaseResources()
{
    modulation.setSize(0, 0);
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

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = inCh; i < outCh; ++i)
        buffer.clear (i, 0, numSamples);

    // Scaletta

    // Generazione dell'onda triangolare.
    lfo.getNextAudioBlock(modulation, numSamples);
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
    if (paramID == "RT")
    {
        lfo.setFrequency(newValue);
    }

    if (paramID == "PD")
    {
    }

    if (paramID == "CD")
    {
    }

    if (paramID == "CLR")
    {
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StoneMistressAudioProcessor();
}
