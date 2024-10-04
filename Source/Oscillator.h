/*
  ==============================================================================

    Oscillator.h
    Created: 30 Sep 2024 11:16:27am
    Author:  Ivan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class NaiveOscillator
{
public:
	NaiveOscillator(double defaultFrequency = 440.0, int defaultWaveform = 0, double defaultPhaseDelta = 0.0)
	{
		waveform = defaultWaveform;
		frequency.setTargetValue(defaultFrequency);
		phaseDelta = defaultPhaseDelta;
	}

	~NaiveOscillator() {}

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

	void setWaveform(int newValue)
	{
		waveform = newValue;
	}

	virtual void getNextAudioBlock(AudioBuffer<double>& buffer, const int numSamples)
	{
		jassert(buffer.getNumChannels() == 2);
		auto leftChannelData = buffer.getWritePointer(0);
		auto rightChannelData = buffer.getWritePointer(1);

		for (int smp = 0; smp < numSamples; ++smp)
		{
			float leftSample = 0.0f;
			float rightSample = 0.0f;
			
			getNextAudioSample(leftSample, rightSample);

			leftChannelData[smp] = leftSample;
			rightChannelData[smp] = rightSample;
		}
	}

	void getNextAudioSample(float& leftSample, float& rightSample)
	{
		auto leftSampleValue = 0.0;
		auto rightSampleValue = 0.0;

		switch (waveform)
		{
		case 0: // Sinusoidale
			leftSampleValue = sin(MathConstants<double>::twoPi * currentPhase);
			break;
		case 1: // Triangular
			leftSampleValue = 4.0f * abs(currentPhase - 0.5f) - 1.0f;
			rightSampleValue = 4.0 * abs((currentPhase + phaseDelta) - 0.5) - 1.0;
			break;
		case 2: // Saw UP
			leftSampleValue = 2.0 * currentPhase - 1.0;
			break;
		case 3: // Saw down
			leftSampleValue = -2.0 * currentPhase + 1.0;
			break;
		case 4: // Square
			leftSampleValue = (currentPhase > 0.5) - (currentPhase < 0.5);
			break;
		default:
			// WTF MAN!
			jassertfalse;
			break;
		}

		leftSample = leftSampleValue;
		rightSample = rightSampleValue;

		phaseIncrement = frequency.getNextValue() * samplePeriod;
		currentPhase += phaseIncrement;
		currentPhase -= static_cast<int>(currentPhase);

	}

private:
	int waveform;
	SmoothedValue<double, ValueSmoothingTypes::Multiplicative> frequency;

	double currentPhase = 0;
	double phaseIncrement = 0;
	double samplePeriod = 1.0;
	double phaseDelta = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NaiveOscillator)
};

class SMLFO : public NaiveOscillator
{
public:
	SMLFO(double defaultFrequency = 8.0)
		: NaiveOscillator(defaultFrequency, 1, 0.8) // 1 per onda triangolare
	{
	}

	~SMLFO() {}

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SMLFO)

};