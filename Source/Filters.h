#pragma once
#include <JuceHeader.h>
#include "parameters.h"


class StereoFilter
{
public:
	StereoFilter()
	{
		for (int f = 0; f < MAX_NUM_CH; ++f)
			iirFilters.add(new IIRFilter());
	}

	~StereoFilter() {}

	void prepareToPlay(double sr)
	{
		sampleRate = sr;

		reset();

		updateCoefficients();
	}

	void processBlock(AudioBuffer<float>& buffer, const int numSamples)
	{
		for (int ch = buffer.getNumChannels(); --ch >= 0;)
			iirFilters.getUnchecked(ch)->processSamples(buffer.getWritePointer(ch), numSamples);
	}

	void setFrequency(float newValue)
	{
		frequency = jmin(newValue, (float)(sampleRate * 0.499));
		updateCoefficients();
	}

	void setQuality(float newValue)
	{
		quality = newValue;
		updateCoefficients();
	}

	void reset()
	{
		for (int f = iirFilters.size(); --f >= 0;)
			iirFilters.getUnchecked(f)->reset();
	}

private:

	void updateCoefficients()
	{
		const IIRCoefficients iirCoeffs = IIRCoefficients::makeLowPass(sampleRate, frequency, quality);

		for (int f = iirFilters.size(); --f >= 0;)
			iirFilters.getUnchecked(f)->setCoefficients(iirCoeffs);
	}

	float frequency = DEFAULT_F;
	float quality = DEFAULT_Q;
	double sampleRate = 48000.0;

	OwnedArray<IIRFilter> iirFilters;
};