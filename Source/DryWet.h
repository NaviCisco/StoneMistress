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

    void copyDrySignal(AudioBuffer<float>& sourceBuffer, int numSamples, int inCh, int outCh)
    {
        if (inCh < outCh) // {1,2}: ogni canale di drySignal contiene una copia dell'unico canale sorgente.
        {
            drySignal.copyFrom(0, 0, sourceBuffer, 0, 0, numSamples);
            drySignal.copyFrom(1, 0, sourceBuffer, 0, 0, numSamples);
        }

        if (inCh == outCh)
        {
            drySignal.copyFrom(0, 0, sourceBuffer, 0, 0, numSamples);
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