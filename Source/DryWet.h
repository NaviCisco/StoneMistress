/*
  ==============================================================================

    DryWet.h
    Created: 30 Sep 2024 11:16:05am
    Author:  Ivan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DryWet
{
public:

    DryWet()
    {
    }

    ~DryWet() {}

    void prepareToPlay(int maxBlockSize)
    {
        drySignal.setSize(2, maxBlockSize);
        drySignal.clear();
    }

    void copyDrySignal(AudioBuffer<float>& sourceBuffer)
    {
        const int numCh = sourceBuffer.getNumChannels();
        const int numSamples = sourceBuffer.getNumSamples();
        for (int ch = 0; ch < numCh; ++ch)
        {
            drySignal.copyFrom(ch, 0, sourceBuffer, ch, 0, numSamples);
        }
    }

    /** Mixes all the buffers together.
    
        @param outputBuffer     Technically, this is the same buffer containing the raw chorus data.
        @param phaserBuffer     Contains the phaser unit data.
    */
    void mixDrySignal(AudioBuffer<float>& outputBuffer, AudioBuffer<float>& phaserBuffer)
    {
        auto numCh = outputBuffer.getNumChannels(); // Both outputBuffer and phaserBuffer can have 2 channels at most.
        auto numSamples = outputBuffer.getNumSamples();

        drySignal.applyGain(0.4);
        outputBuffer.applyGain(0.6);
        phaserBuffer.applyGain(0.6);

        for (int ch = 0; ch < numCh; ++ch)
        {
            outputBuffer.addFrom(ch, 0, drySignal, ch, 0, numSamples);
            outputBuffer.addFrom(ch, 0, phaserBuffer, ch, 0, numSamples);
        }
    }

    void releaseResources()
    {
        drySignal.setSize(0, 0);
    }

private:

    AudioBuffer<float> drySignal;

    //float dryLevel = 0.0;
    //float wetPhaserLevel = 1.0;
    //float wetChorusLevel = 0.3;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)

};