/*
  ==============================================================================

    Oscillator.h
    Created: 30 Sep 2024 11:16:27am
    Author:  Ivan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#define CHORUS_DELAY_TIME 0.010

class LFO
{
public:
	LFO(double defaultRate = 7.0, double defaultPhaseDelta = 0.5)
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

	/* Left Channel/Channel 0 = Chorus Unit.
	   Right Channel/Channel 1 = Phaser Unit.
	*/
	void getNextAudioBlock(AudioBuffer<double>& buffer, const int numSamples)
	{
		auto data = buffer.getArrayOfWritePointers();

		for (int smp = 0; smp < numSamples; ++smp)
		{
			double leftSample = 0.0f;
			double rightSample = 0.0f;

			getNextAudioSample(leftSample, rightSample);

			data[0][smp] = leftSample;
			data[1][smp] = rightSample;
		}
	}

	void getNextAudioSample(double& leftSample, double& rightSample)
	{
		//Small Stone LFO Is a SineWave
		leftSample = 4.0 * abs(currentPhase - std::floor(currentPhase + 0.5)) - 1.0;
		rightSample = 4.0 * abs((currentPhase + phaseDelta) - std::floor(currentPhase + phaseDelta + 0.5)) - 1.0;

		phaseIncrement = rate.getNextValue() * samplePeriod;
		currentPhase += phaseIncrement;
		currentPhase -= static_cast<int>(currentPhase);
	}

private:
	SmoothedValue<double, ValueSmoothingTypes::Multiplicative> rate;

	double currentPhase = 0;
	double phaseIncrement = 0;
	double samplePeriod = 1.0;
	double phaseDelta = 0.5;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFO)
};

class ParameterModulation {
public:

	ParameterModulation(const double defaultPhaserDepth = 0.030, const double defaultChorusDepth = 0.0)
	{
		phaserDepth.setCurrentAndTargetValue(defaultPhaserDepth);
		chorusDepth.setCurrentAndTargetValue(defaultChorusDepth);
	}

	~ParameterModulation() {}

	void prepareToPlay(double sampleRate)
	{
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

	void processBlock(AudioBuffer<double>& buffer, const int numSamples, const String unit)
	{
		auto data = buffer.getArrayOfWritePointers();
		const auto numCh = buffer.getNumChannels();

		for (int ch = 0; ch < numCh; ++ch)
		{
			FloatVectorOperations::add(data[ch], 1.0, numSamples); // At this stage, LFO is in range [0, +2]
			FloatVectorOperations::multiply(data[ch], 0.5, numSamples); // At this stage, LFO is in range [0, +1]
		}

		if (unit == "p")
		{
			phaserDepth.applyGain(data[0], numSamples);
			phaserDepth.applyGain(data[1], numSamples);
		}
		else if (unit == "c")
		{
			chorusDepth.applyGain(data[0], numSamples);
			chorusDepth.applyGain(data[1], numSamples);
			FloatVectorOperations::add(data[0], CHORUS_DELAY_TIME, numSamples);
			FloatVectorOperations::add(data[1], CHORUS_DELAY_TIME, numSamples);
		}
	}

private:

	SmoothedValue<double, ValueSmoothingTypes::Linear> phaserDepth;
	SmoothedValue<double, ValueSmoothingTypes::Linear> chorusDepth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterModulation)

};

// This class allows the LFO and Modulation classes to communicate with each other. If the RATE value is below a certain
// threshold, the pedal will enter "Filter Matrix" mode, so that the PHASER unit's delay time will be manually set via the RATE
// knob. This reults in the ability for the user to manually move the notches across the frequency spectrum. If the RATE knob points
// to a value above the threshold, Filter Matrix mode will be disengaged, that is, the Phaser's All Pass coefficients will be dynamically 
// modulated by the LFO as usual.

class FilterMatrix
{
public:
	FilterMatrix(LFO& lfo, ParameterModulation& modulator)
	{
	}

	~FilterMatrix() {}

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterMatrix)

};