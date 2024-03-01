#pragma once
#include <JuceHeader.h>
#include "DryWet.h"
#include "Delays.h"
#include "Oscillators.h"
#include "Filters.h"
#include "OverHex.h"




class DelayFXAudioProcessor  : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DelayFXAudioProcessor();
    ~DelayFXAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    Atomic<float> envelope;
    Atomic<float>* observedEnvelope = nullptr;


private:
    void parameterChanged(const String& paramID, float newValue) override;

    AudioProcessorValueTreeState parameters;

    DryWet drywetter;
    ModDelay delay;
    NaiveOscillator LFO;
    ParametrModulation timeAdapter;
    StereoFilter filterin;
    StereoFilter filterout;
    OversampledSaturation saturator;
    OversampledSaturation delaySaturator;
    AudioBuffer<float> modulationSignal;
    float alpha = 0.0f;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayFXAudioProcessor)
};
