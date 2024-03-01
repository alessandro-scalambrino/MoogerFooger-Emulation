#pragma once
#include <JuceHeader.h>
#include "Parameters.h"



class ParametrModulation
{
public:
	ParametrModulation() {}
	~ParametrModulation() {}

	void prepareToPlay(double sr)
	{
		parameter.reset(sr, SMOOTHING_TIME);
		modAmount.reset(sr, SMOOTHING_TIME);
	}

	void setParameter(float newValue)
	{
		parameter.setTargetValue(newValue);
	}

	void setModAmount(float newValue)
	{
		modAmount.setTargetValue(newValue);
	}

	void processBlock(AudioBuffer<float>& buffer, const int numSamples)
	{
		auto bufferData = buffer.getArrayOfWritePointers();
		auto numCh = buffer.getNumChannels();

		// Scalo la modulante tra 0 e 1
		for (int ch = 0; ch < numCh; ++ch)
		{
			FloatVectorOperations::add(bufferData[ch], 1.0, numSamples);
			FloatVectorOperations::multiply(bufferData[ch], 0.5f, numSamples);
		}

		modAmount.applyGain(buffer, numSamples);

		// Sommo alla modulante il tempo di delay proveniente dal parametro
		if (parameter.isSmoothing())
			for (int smp = 0; smp < numSamples; ++smp)
			{
				const auto param = parameter.getNextValue();

				for (int ch = 0; ch < numCh; ++ch)
					bufferData[ch][smp] += param;
			}
		else
			for (int ch = 0; ch < numCh; ++ch)
				FloatVectorOperations::add(bufferData[ch], parameter.getCurrentValue(), numSamples);

		// Controllo di essere dentro al tempo di delay massimo
		for (int ch = 0; ch < numCh; ++ch)
			FloatVectorOperations::min(bufferData[ch], bufferData[ch], MAX_DELAY_TIME, numSamples);
		
	}

	

private:
	SmoothedValue<float, ValueSmoothingTypes::Linear> modAmount;
	SmoothedValue<float, ValueSmoothingTypes::Linear> parameter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametrModulation)

};

class NaiveOscillator : ParametrModulation
{
public:
	NaiveOscillator() {};
	~NaiveOscillator() {};

	void prepareToPlay(double sr)
	{
		samplePeriod = 1.0 / sr; //delta time
		frequency.reset(sr, GLIDE_TIME);
		smoothedSampleValue.reset(sr, 0.01f);
	}

	void setFrequency(float newValue)
	{
		frequency.setTargetValue(newValue);
	}

	void setWaveform(float newValue)
	{
		waveform = roundToInt(newValue);
	}


	void getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples)
	{
		const int numCh = buffer.getNumChannels();
		auto bufferData = buffer.getArrayOfWritePointers();


		for (int smp = 0; smp < numSamples; ++smp)
		{
			const auto sampleValue = getNextAudioSample();

			for (int ch = 0; ch < numCh; ++ch)
				bufferData[ch][smp] = sampleValue;
		}
	}

	float getNextAudioSample()
	{
		auto sampleValue = 0.0f;
		

		switch (waveform)
		{
		case 0: // Sine sin(2pi*[ft+p])
			sampleValue = sin(MathConstants<float>::twoPi * currentPhase);
			break;
		case 1: // Triangular 4*mod([ft+p]-0.5)-1
			sampleValue = 4.0f * abs(currentPhase - 0.5f) - 1.0f;
			break;
		case 2: // Saw up
			sampleValue = 2.0f * currentPhase - 1.0f;
			break;
		case 3: // Saw down
			sampleValue = -2.0f * currentPhase - 1.0f;
			break;
		case 4: //square
			sampleValue = (currentPhase > 0.5) - (currentPhase < 0.5);
		case 5: // s&h
			sampleValue = (oldPhase > currentPhase) ? (2.0f * (dice.nextFloat() - 0.5f)) : oldSample;
			oldSample = sampleValue;
			oldPhase = currentPhase;
			break;
		default:
			break;
		}
		//we need to maintain a state for our wave generation by storing the current phase angle and the amount by which the phase angle needs to increment for each output sample. 
		//This size of this change per sample ("delta") is dependent on the sample rate of the output and the frequency of the sine wave we want to generate.
		phaseIncrement = frequency.getNextValue() * samplePeriod; //  freq*deltatime
		currentPhase += phaseIncrement;
		currentPhase -= static_cast<int>(currentPhase); //riporta la fase tra 0,1
		oldValue = sampleValue;
		return sampleValue;
	}

private:

	int waveform = 0;

	double samplePeriod = 1.0;
	float currentPhase = 0.0f; //store valore istantaneo del fasore
	float phaseIncrement = 0.0f;
	float oldValue = 0.0f;
	SmoothedValue<float, ValueSmoothingTypes::Multiplicative> frequency;
	SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedSampleValue;
	Random dice;
	float oldPhase = 0.0f;
	float oldSample = 0.0f;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NaiveOscillator)
};