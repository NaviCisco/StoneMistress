/*
  ==============================================================================

    Delays.h
    Created: 30 Sep 2024 11:14:52am
    Author:  Ivan F. Muñoz G.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define MAX_DELAY_TIME 0.0016

class Chorus
{
public:

    Chorus(double defaultModDelayTime = MAX_DELAY_TIME)
    {
        maxDelayTime = defaultModDelayTime;
    }

    ~Chorus() {}

    void prepareToPlay(double newSampleRate, int maxBlockSize)
    {
        sampleRate = newSampleRate;
        memorySize = roundToInt(maxDelayTime * sampleRate) + maxBlockSize;
        delayMemory.setSize(2, memorySize);
        delayMemory.clear();
    }

    void releaseResources()
    {
        delayMemory.setSize(0, 0);
        memorySize = 0;
    }

    void processBlock(AudioBuffer<float>& buffer, AudioBuffer<double>& modulationBuffer)
    {
        const auto numSamples = buffer.getNumSamples();
        const auto numCh = buffer.getNumChannels();

        auto bufferData = buffer.getArrayOfWritePointers();
        auto delayData = delayMemory.getArrayOfWritePointers();
        auto modData = modulationBuffer.getArrayOfWritePointers(); // Delay time is modulated by the left Channel of the mod. buffer!

        for (int smp = 0; smp < numSamples; ++smp)
        {
            for (int ch = 0; ch < numCh; ++ch)
            {
                auto dt = modData[0][smp];

                auto readIndex = writeIndex - (dt * sampleRate);

                auto integerPart = static_cast<int>(readIndex);
                auto fractionalPart = readIndex - integerPart;

                auto A = (integerPart + memorySize) % memorySize;
                auto B = (A + 1) % memorySize;
                auto alpha = fractionalPart / (2.0 - fractionalPart);

                delayData[ch][writeIndex] = bufferData[ch][smp];

                auto sampleValue = alpha * (delayData[ch][B] - oldSample[ch]) + delayData[ch][A];
                oldSample[ch] = sampleValue;

                bufferData[ch][smp] = static_cast<float>(sampleValue);

                delayData[ch][writeIndex] += bufferData[ch][smp];
            }

            ++writeIndex %= memorySize;
        }
    }

private:

    AudioBuffer<float> delayMemory;
    
    double sampleRate = 1.0;
    double maxDelayTime;
    int memorySize = 0;
    int writeIndex = 0;

    float oldSample[2] = { 0.0f, 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Chorus)

};