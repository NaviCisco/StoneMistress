/*
  ==============================================================================

    SmallStone.h
    Created: 19 Oct 2024 8:43:00pm
    Author:  Ivan F. Muñoz G.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Filters.h"

#define FEEDBACK 0.8
#define STAGES 4

// Small Stone EH4800 Phase Shifter Pedal emulation. When the COLOR switch is engaged, a feedback line is enabled.
class SmallStone {
public:

    SmallStone()
        : chain{
        AllPass(25.0),
        AllPass(25.0),
        AllPass(50.0),
        AllPass(50.0),
        }
    {
    }

    ~SmallStone() {}

    void prepareToPlay(double newSampleRate)
    {
        samplePeriod = 1 / newSampleRate;
        feedbackSignal.setSize(2, 1);
        feedbackSignal.clear();
        for (auto& stage : chain)
        {
            stage.setSamplePeriod(samplePeriod);
        }
    }

    void releaseResources()
    {
        feedbackSignal.setSize(0, 0);
    }

    /** This is where the magic takes place.
    
        @param buffer       The buffer with the audio data.
        @param modulation   The buffer containing the modulation data.
    */
    void processBlock(AudioBuffer<float>& buffer, AudioBuffer<double>& modulation, const int numSamples)
    {
        const auto numCh = buffer.getNumChannels();

        auto bufferData = buffer.getArrayOfWritePointers();
        auto modData = modulation.getArrayOfWritePointers();

        for (int smp = 0; smp < numSamples; ++smp)
        {
            for (int ch = 0; ch < numCh; ++ch)
            {
                auto sampleValue = bufferData[ch][smp];
                auto modValue = modData[ch][smp];

                if (colorSwitch) // Adds feedback up at first stage.
                {
                    sampleValue += FEEDBACK * feedbackSignal.getSample(ch, 0);
                }

                for (int stage = 0; stage < STAGES; ++stage)
                {
                    sampleValue = chain[stage].processSample(sampleValue, ch, modValue);
                }

                if (colorSwitch)
                {
                    feedbackSignal.setSample(ch, 0, sampleValue);
                }

                bufferData[ch][smp] = static_cast<float>(sampleValue);
            }
        }
    }

    void setColor()
    {
        colorSwitch = !colorSwitch;
    }

private:

    AllPass chain[4];
    AudioBuffer<float> feedbackSignal; // 1 Sample big.

    double samplePeriod = 1.0;
    bool colorSwitch = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmallStone)
};