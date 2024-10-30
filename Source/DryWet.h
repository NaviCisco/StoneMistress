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

    void prepareToPlay(double sampleRate, int maxBlockSize)
    {
        drySignal.setSize(2, maxBlockSize);
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

    void mixDrySignal()
    {
    }

    void releaseResources()
    {
        drySignal.setSize(0, 0);
    }

private:

    AudioBuffer<float> drySignal;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)

};