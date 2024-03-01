

#pragma once
#include <JuceHeader.h>
#include "parameters.h"



class Meter : public Component, public Timer
{
public:
    Meter() 
    {
        alpha = exp(-1.0f / (FPS * RT));
        startTimerHz(FPS);
    }

    ~Meter()
    {
        stopTimer();
    }

    void paint(Graphics& g) override
    {
        auto W = getWidth();
        auto H = getHeight();

       

        
        g.setColour(Colours::darkgrey);
        g.drawEllipse(0, 0, W, H, 8);

        if (observedEnvelope != nullptr)
        {
            auto envelopeSnapshot = observedEnvelope->get();
            observedEnvelope->set(envelopeSnapshot * alpha);

            auto peak = Decibels::gainToDecibels(envelopeSnapshot);

           
            
           
            g.setColour(peak >= 0.0f ? Colours::darkred : peak >= -6.0f ? Colours::yellow : peak <= -48.0f ? Colours::black : Colours::green);
           
            

            g.fillEllipse(0, 0, W, H);
        }

    }

    void resized() override {}

    void connectTo(Atomic<float>& targetVariable) // connected to processor.envelope in plugineditor.cpp
    {
        observedEnvelope = &targetVariable;
    }

private:

    void timerCallback() override
    {
        repaint();
    }

    float alpha = 0.0f;

    Atomic<float>* observedEnvelope = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter);
};