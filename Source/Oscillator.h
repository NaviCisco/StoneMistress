/*
  ==============================================================================

    Oscillator.h
    Created: 30 Sep 2024 11:16:27am
    Author:  Ivan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LFO
{
public:
	LFO(double defaultFrequency = 7.0, double defaultPhaseDelta = 0.8)
	{
		frequency.setTargetValue(defaultFrequency);
		phaseDelta = defaultPhaseDelta;
	}

	~LFO() {}

	void prepareToPlay(double sampleRate)
	{
		frequency.reset(sampleRate, 0.02);
		samplePeriod = 1.0 / sampleRate;
	}

	void setFrequency(double newValue)
	{
		// No zero-frequency allowed
		jassert(newValue > 0);
		frequency.setTargetValue(newValue);
	}

	void getNextAudioBlock(AudioBuffer<double>& buffer, const int numSamples)
	{
		auto chorusChannelData = buffer.getWritePointer(0);
		auto phaserChannelData = buffer.getWritePointer(1);

		for (int smp = 0; smp < numSamples; ++smp)
		{
			float leftSample = 0.0f;
			float rightSample = 0.0f;
			
			getNextAudioSample(leftSample, rightSample);

			phaserChannelData[smp] = leftSample;
			chorusChannelData[smp] = rightSample;
		}
	}

	void getNextAudioSample(float& leftSample, float& rightSample)
	{
		auto leftSampleValue = 0.0;
		auto rightSampleValue = 0.0;

		leftSampleValue = 4.0f * abs(currentPhase - 0.5f) - 1.0f;
		rightSampleValue = 4.0 * abs((currentPhase + phaseDelta) - 0.5) - 1.0;

		leftSample = leftSampleValue;
		rightSample = rightSampleValue;

		phaseIncrement = frequency.getNextValue() * samplePeriod;
		currentPhase += phaseIncrement;
		currentPhase -= static_cast<int>(currentPhase);

	}

private:
	SmoothedValue<double, ValueSmoothingTypes::Multiplicative> frequency;

	double currentPhase = 0;
	double phaseIncrement = 0;
	double samplePeriod = 1.0;
	double phaseDelta = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFO)
};