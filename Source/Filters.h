/*
  ==============================================================================

    Filters.h
    Created: 13 Oct 2024 7:33:18pm
    Author:  Ivan F. Muñoz G.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/* 
 * Creates a simple stereo All Pass Filter with 90° phase shift at selected Break Frequency. The terms "Center Frequency" and 
 * "Cutoff Frequency" are synonyms.
 * AllPass = a*x[n] + x[n - 1] - a*y[n - 1]
*/
class AllPass {
public:
    /*
    * @param defaultBreakFrequency The frequency at which a phase shift of 90° occurs.
    */
    AllPass(double defaultBreakFrequency = 1000)
        : breakFrequency(defaultBreakFrequency)
    {
    }

    ~AllPass() {}

    /** Division is an expensive operation when it comes to machine computation power. Since the All Pass Coefficient calculation
        involves a division by 1/sampleRate = samplePeriod, it is reasonable to  turn that division into a multiplication.
        For this reason, the sample period is stored instead.

        @param newSamplePeriod  New sample period.
    */
    void setSamplePeriod(double newSamplePeriod)
    {
        samplePeriod = newSamplePeriod;
    }

    float processSample(float x, int ch, double modulation)
    {
        float modCoefficient = calculateCoefficient(modulation);

        float y = modCoefficient * x + x1[ch] - modCoefficient * y1[ch];

        x1[ch] = x;
        y1[ch] = y;

        return y;
    }

    float calculateCoefficient(float modValue = 0)
    {
        auto tangent = tan(MathConstants<float>::pi * (breakFrequency + modValue) * samplePeriod);
        return ((tangent - 1) / (tangent + 1));
    }


private:

    std::array<float, 2> y1 = { 0.0f, 0.0f }; // y[n - 1]
    std::array<float, 2> x1 = { 0.0f, 0.0f }; // x[n - 1]

    double samplePeriod = 1.0;
    double breakFrequency;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AllPass)
};