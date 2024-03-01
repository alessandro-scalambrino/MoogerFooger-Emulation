#include "PluginProcessor.h"
#include "Parameters.h"
#include "PluginEditor.h"



DelayFXAudioProcessor::DelayFXAudioProcessor()
    : parameters(*this, nullptr, "DelayFXParameters", Parameters::createParameterLayout())
{
    parameters.addParameterListener(NAME_DW, this);
    parameters.addParameterListener(NAME_DT, this);
    parameters.addParameterListener(NAME_FB, this);
    parameters.addParameterListener(NAME_FREQ, this);
    parameters.addParameterListener(NAME_MOD, this);
    parameters.addParameterListener(NAME_WF, this);
    parameters.addParameterListener(NAME_LS, this);
    parameters.addParameterListener(NAME_SAT, this);

    delay.setFeedback(DEFAULT_FB);
    drywetter.setDryWetRatio(DEFAULT_DW);
    LFO.setFrequency(DEFAULT_FREQ);
    LFO.setWaveform(DEFAULT_WF);
    timeAdapter.setModAmount(DEFAULT_MOD);
    timeAdapter.setParameter(DEFAULT_DT);
    filterin.setFrequency(DEFAULT_F);
    filterin.setQuality(DEFAULT_Q);
    filterout.setFrequency(DEFAULT_F);
    filterout.setQuality(DEFAULT_Q);
    saturator.setGain(DEFAULT_SAT);


}

DelayFXAudioProcessor::~DelayFXAudioProcessor()
{
}

//==============================================================================
void DelayFXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    delay.prepareToPlay(sampleRate, samplesPerBlock);
    LFO.prepareToPlay(sampleRate);
    modulationSignal.setSize(2, samplesPerBlock);
    timeAdapter.prepareToPlay(sampleRate);
    filterin.prepareToPlay(sampleRate);
    filterin.setFrequency(DEFAULT_F);
    filterin.setQuality(DEFAULT_Q);
    filterout.prepareToPlay(sampleRate);
    filterout.setFrequency(DEFAULT_F);
    filterout.setQuality(DEFAULT_Q);

   


    

    //Saturation
    auto numCh = jmax(getTotalNumOutputChannels(), getTotalNumInputChannels());
    dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, numCh };
    //input saturator prepare
    saturator.prepare(spec);
    saturator.setGain(DEFAULT_SAT);

    //delay saturator prepare
    delaySaturator.prepare(spec);
    delaySaturator.setGain(DEFAULT_SUBTLESAT);

    setLatencySamples(saturator.getLatency() + delaySaturator.getLatency());
    drywetter.setLatency(delaySaturator.getLatency());
}

void DelayFXAudioProcessor::releaseResources()
{
    drywetter.releaseResources();
    delay.releaseResurces();
    modulationSignal.setSize(0, 0);

  
   
}

void DelayFXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const auto numSamples = buffer.getNumSamples();

    // Genero una modulante
    LFO.getNextAudioBlock(modulationSignal, numSamples);

    // Scalare modulante
    timeAdapter.processBlock(modulationSignal, numSamples);

    //saturazioned del segnale input
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
    saturator.process(context);

   
    //ritardo segnale dry
    // Salvo il segnale in input pulito
    drywetter.setDry(buffer);

    // Applicare delay e filtraggio
    filterin.processBlock(buffer, numSamples);
    delay.processBlock(buffer, modulationSignal);
    //delay saturation
    delaySaturator.process(context);
    //
    filterout.processBlock(buffer, numSamples);
   
    
    // Miscelo il segnale pulito salvato in drywetter con quello processato dal delay
    drywetter.merge(buffer);

    //signal envelope
    envelope.set(jmax(buffer.getMagnitude(0, buffer.getNumSamples()), envelope.get()));

    
}


juce::AudioProcessorEditor* DelayFXAudioProcessor::createEditor()
{
    return new PluginEditor(*this, parameters);
}

//==============================================================================
void DelayFXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelayFXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void DelayFXAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == NAME_DW)
        drywetter.setDryWetRatio(newValue);

    if (paramID == NAME_DT)
        timeAdapter.setParameter(newValue);
    
    if (paramID == NAME_FB)
        delay.setFeedback(newValue);

    if (paramID == NAME_FREQ)
        LFO.setFrequency(newValue);

    if (paramID == NAME_MOD)
        timeAdapter.setModAmount(newValue);

    if (paramID == NAME_WF)
        LFO.setWaveform(newValue);
    if (NAME_WF == "s&h")
        timeAdapter.setModAmount(juce::Random::getSystemRandom().nextInt(juce::Range<int>(0, 3)));

    
    if (paramID == NAME_LS)
        if (newValue == 0)
        {
            filterin.setFrequency(3000);
            filterout.setFrequency(3000);
        }
        else if (newValue == 1)
        {
            filterin.setFrequency(17000);
            filterout.setFrequency(17000);
        }
    if (paramID == NAME_SAT)
            saturator.setGain(newValue);
            

        
        

        
        

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayFXAudioProcessor();
}
