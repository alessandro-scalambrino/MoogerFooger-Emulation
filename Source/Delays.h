#pragma once
#include <JuceHeader.h>
#include "OverHex.h"
#include "parameters.h"
#include "PluginProcessor.h"

class DelayBase
{
public:
	DelayBase() {};
	~DelayBase() {};

	void prepareToPlay(double sampleRate, int maxNumSamples)
	{
		sr = sampleRate;
		memorySize = roundToInt(MAX_DELAY_TIME * sampleRate) + maxNumSamples; //define max buffersize
		delayMemory.setSize(2, memorySize);
		initialize();

		
		
	}

	void releaseResurces()
	{
		delayMemory.setSize(0, 0);
		memorySize = 0;
	}

	void processBlock(AudioBuffer<float>& buffer)
	{
		store(buffer);
		auto numSamples = buffer.getNumSamples();
		moveDelayedTo(buffer);
		updateWriteHead(buffer.getNumSamples());
	}


protected:

	virtual void initialize() = 0;

	void store(const AudioBuffer<float>& buffer) //scrive sul buffer i campioni da ritardare-->readhead
	{
		const auto numInputSamples = buffer.getNumSamples();
		const auto numChannels = buffer.getNumChannels();

		for (int ch = 0; ch < numChannels; ++ch)
		{
			if (writeIndex + numInputSamples <= memorySize)
				delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, numInputSamples);
			else
			{
				const int fittingSamples = memorySize - writeIndex;
				const int remainingSamples = numInputSamples - fittingSamples;

				delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, fittingSamples);
				delayMemory.copyFrom(ch, 0, buffer, ch, fittingSamples, remainingSamples);
			}
		}
	}

	virtual void moveDelayedTo(AudioBuffer<float>& buffer) = 0;

	void updateWriteHead(int leap)
	{
		writeIndex += leap;
		writeIndex %= memorySize;
	}

	AudioBuffer<float> delayMemory;
	int memorySize = 0;
	int writeIndex = 0;
	double sr = 1;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayBase)
};


class ModDelay : public DelayBase
{
public:
	ModDelay() {}
	~ModDelay() {}

	void setFeedback(float newValue)
	{
		feedback.setTargetValue(newValue);
	}

	void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& delayTimeBuffer)
	{
		store(buffer);
		moveDelayedTo(buffer, delayTimeBuffer);
		updateWriteHead(buffer.getNumSamples());
		
	}

private:

	void moveDelayedTo(AudioBuffer<float>& buffer) override
	{
		// If you reach this assertion you're trying to use the wrong overload
		jassertfalse;
	}

	void moveDelayedTo(AudioBuffer<float>& buffer, AudioBuffer<float>& delayTimeBuffer)

	{
		
		const auto numOutputSamples = buffer.getNumSamples();
		const auto numChannels = buffer.getNumChannels();
		const auto numModChannels = delayTimeBuffer.getNumChannels(); //numero canali buffer delay
		
		

		auto** outputData = buffer.getArrayOfWritePointers();
		auto** delayData = delayMemory.getArrayOfWritePointers();
		auto** delayTimeArray = delayTimeBuffer.getArrayOfReadPointers();

		for (int smp = 0; smp < numOutputSamples; ++smp) //iteriamo sui sample
		{
			auto fb = feedback.getNextValue();

			for (int ch = 0; ch < numChannels; ++ch) //iteriamo sui canali del buffer
			{
				auto dt = delayTimeArray[jmin(ch, numModChannels - 1)][smp];

				auto actualWriteIndex = (writeIndex + smp) % memorySize;    //definizione read/write head
				auto actualReadIndex = memorySize + actualWriteIndex - (dt * sr);

				auto integerPart = (int)actualReadIndex; 
				auto fractionalPart = actualReadIndex - integerPart; //read index letto piu di una volta per sample

				//inidice del frame da cui devo leggere e il successivo per interp
				auto A = (integerPart + memorySize) % memorySize;
				auto B = (A + 1) % memorySize;
				//coefficiente del filtro allpass 
				const auto alpha = fractionalPart / (2.0f - fractionalPart);
				//
				// Leggo dal bufer del delay

					// NO INTERPOLATION
					//auto sampleValue = delayData[ch][integerPart]; 

					// LINEAR INTERPOLATION
					//auto sampleValue = delayData[ch][A] * (1.0 - fractionalPart) +
					//				   delayData[ch][B] * fractionalPart;

					// ALLPASS FILTER
				auto sampleValue = alpha * (delayData[ch][B] - oldSample[ch]) + delayData[ch][A]; //coeff *( vecchio campione in input - vecchio campione in output + campione corrente
				oldSample[ch] = sampleValue;

				

				
				
				
				
				//scrivo sul buffer in uscita
				outputData[ch][smp] = sampleValue;

				// Scrivo sul delay buffer il feedback + soft clip
				auto fbValue = sampleValue * fb;
				if (fbValue > 1)
				{
					fbValue = 2.0f / 3.0;
				}
				else
				{
					if (fbValue < -1)
						fbValue = -2.0f / 3.0f;
					else
						fbValue -= (fbValue * fbValue * fbValue) / 3.0f;
				}
				delayData[ch][actualWriteIndex] += fbValue;


				
				
				

			}
		}
	}

	void initialize() override
	{
		feedback.reset(sr, FBK_SMOOTHING);
	}

	float oldSample[2] = { 0.0f, 0.0f };

	
	SmoothedValue<float, ValueSmoothingTypes::Linear> feedback;
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModDelay)
};

class DelayBlock
{
public:
	DelayBlock() {};
	~DelayBlock() {};

	void prepareToPlay(double sampleRate, int maxNumSamples)
	{
		sr = sampleRate;
		memorySize = roundToInt(MAX_DELAY_TIME * sampleRate) + maxNumSamples;
		delayMemory.setSize(2, memorySize);
		setTime(delayTime);
	}

	void releaseResurces()
	{
		delayMemory.setSize(0, 0);
		memorySize = 0;
	}

	void processBlock(AudioBuffer<float>& buffer)
	{
		store(buffer);
		moveDelayedTo(buffer);
		updateWriteHead(buffer.getNumSamples());
	}

	void setTime(float newValue)
	{
		delayTime = newValue;
	}

private:

	void store(const AudioBuffer<float>& buffer)
	{
		const auto numInputSamples = buffer.getNumSamples();
		const auto numChannels = buffer.getNumChannels();

		for (int ch = 0; ch < numChannels; ++ch)
		{
			if (writeIndex + numInputSamples <= memorySize)
				delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, numInputSamples);
			else
			{
				const int fittingSamples = memorySize - writeIndex;
				const int remainingSamples = numInputSamples - fittingSamples;

				delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, fittingSamples);
				delayMemory.copyFrom(ch, 0, buffer, ch, fittingSamples, remainingSamples);
			}
		}
	}

	void moveDelayedTo(AudioBuffer<float>& buffer)
	{
		const auto numOutputSamples = buffer.getNumSamples();
		const auto numChannels = buffer.getNumChannels();

		const auto readIndex = (writeIndex - roundToInt(delayTime * sr) + memorySize) % memorySize;

		for (int ch = 0; ch < numChannels; ++ch)
		{
			if (readIndex + numOutputSamples <= memorySize)
				buffer.copyFrom(ch, 0, delayMemory, ch, readIndex, numOutputSamples);
			else
			{
				const int fittingSamples = memorySize - readIndex;
				const int remainingSamples = numOutputSamples - fittingSamples;

				buffer.copyFrom(ch, 0, delayMemory, ch, readIndex, fittingSamples);
				buffer.copyFrom(ch, fittingSamples, delayMemory, ch, 0, remainingSamples);
			}
		}
	}

	void updateWriteHead(int leap)
	{
		writeIndex += leap;
		writeIndex %= memorySize;
	}

	AudioBuffer<float> delayMemory;
	int memorySize = 0;
	int writeIndex = 0;
	float delayTime = 0.5;
	double sr = 1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayBlock);
};
