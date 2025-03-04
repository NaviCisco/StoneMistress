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

    /** Mixes dry and wet signal buffers.
    *  output = (drySignal + outputBuffer) * mixLevel
    
        @param outputBuffer     Wet signal.
    */
    void mixDrySignal(AudioBuffer<float>& outputBuffer)
    {
        auto numCh = outputBuffer.getNumChannels();
        auto numSamples = outputBuffer.getNumSamples();

        for (int ch = 0; ch < numCh; ++ch)
        {
            outputBuffer.addFrom(ch, 0, drySignal, ch, 0, numSamples);
        }
        outputBuffer.applyGain(mixLevel);
    }

    void releaseResources()
    {
        drySignal.setSize(0, 0);
    }

private:

    AudioBuffer<float> drySignal;

    float mixLevel = 0.6;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)

};