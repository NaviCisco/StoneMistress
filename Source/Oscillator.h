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
	LFO(double defaultRate = 7.0, double defaultPhaseDelta = 0.8)
	{
		rate.setTargetValue(defaultRate);
		phaseDelta = defaultPhaseDelta;
	}

	~LFO() {}

	void prepareToPlay(double sampleRate)
	{
		rate.reset(sampleRate, 0.02);
		samplePeriod = 1.0 / sampleRate;
	}

	void setRate(double newValue)
	{
		// No zero-frequency allowed
		jassert(newValue > 0);
		rate.setTargetValue(newValue);
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
		leftSample = 4.0f * abs(currentPhase - 0.5f) - 1.0f;
		rightSample = 4.0 * abs((currentPhase + phaseDelta) - 0.5) - 1.0;

		phaseIncrement = rate.getNextValue() * samplePeriod;
		currentPhase += phaseIncrement;
		currentPhase -= static_cast<int>(currentPhase);
	}

private:
	SmoothedValue<double, ValueSmoothingTypes::Multiplicative> rate;

	double currentPhase = 0;
	double phaseIncrement = 0;
	double samplePeriod = 1.0;
	double phaseDelta = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFO)
};

class timeModulation {
public:

	timeModulation()
	{
	}

	~timeModulation() {}

	void prepareToPlay(double sampleRate)
	{
		delayTime.reset(sampleRate, 0.02);
		phaserDepth.reset(sampleRate, 0.02);
		chorusDepth.reset(sampleRate, 0.02);
	}

	void setPhaserDepth(const double newValue)
	{
		phaserDepth.setTargetValue(newValue);
	}

	void setChorusDepth(const double newValue)
	{
		chorusDepth.setTargetValue(newValue);
	}

	void setDelayTime(const double newValue)
	{
	}

private:

	SmoothedValue<double, ValueSmoothingTypes::Linear> delayTime;
	SmoothedValue<double, ValueSmoothingTypes::Linear> phaserDepth;
	SmoothedValue<double, ValueSmoothingTypes::Linear> chorusDepth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(timeModulation)

};