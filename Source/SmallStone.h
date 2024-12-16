/*
  ==============================================================================

    SmallStone.h
    Created: 19 Oct 2024 8:43:00pm
    Author:  Ivan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Filters.h"

#define FEEDBACK 0.20
#define STAGES 4

// Quick emulation of the Small Stone EH4800 Phase Shifter Pedal. 4 Stages on normal operation; when the COLOR switch is up, a
// 5th stage with feedback is added.
class SmallStone {
public:

    SmallStone()
        : chain{
        AllPass(1000.0),
        AllPass(1000.0),
        AllPass(5000.0),
        AllPass(5000.0),
        // This stage is only enabled when the COLOR switch is UP
        // AllPass(8000.0)
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
        auto modData = modulation.getArrayOfWritePointers(); // Phaser is modulated by right Channel of the modulation buffer!

        for (int smp = 0; smp < numSamples; ++smp)
        {
            for (int ch = 0; ch < numCh; ++ch)
            {
                auto sampleValue = bufferData[ch][smp];
                auto modValue = modData[1][smp];

                /*if (colorSwitch) // Adds feedback up at first stage.
                {
                    sampleValue += FEEDBACK * feedbackSignal.getSample(ch, 0);
                }*/

                for (int stage = 0; stage < STAGES; ++stage) // 5th stage also enables a feedback line.
                {
                    //DBG("Preocessing Stage Number " << stage);
                    sampleValue = chain[stage].processSample(sampleValue, ch, modValue);
                }

                /*if (colorSwitch)
                {
                    feedbackSignal.setSample(ch, 0, sampleValue);
                }*/

                bufferData[ch][smp] = static_cast<float>(sampleValue);
            }
        }
    }

    void setColor()
    {
        colorSwitch = !colorSwitch;
    }

private:

    AllPass chain[5];
    AudioBuffer<float> feedbackSignal; // 1 Sample big.
    AudioBuffer<float> copySignal; // Contains a copy of the AudioProcessor's buffer.

    double samplePeriod = 1.0;
    bool colorSwitch = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SmallStone)
};