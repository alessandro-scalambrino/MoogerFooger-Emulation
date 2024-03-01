/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyTheme.h"
#include "Meter.h"


typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PluginEditor  : public juce::AudioProcessorEditor
{
public:
    //==============================================================================
    PluginEditor (DelayFXAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~PluginEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    // Binary resources:
    static const char* logo_png;
    static const int logo_pngSize;
    static const char* drive_png;
    static const int drive_pngSize;
    static const char* mix_png;
    static const int mix_pngSize;
    static const char* delay_png;
    static const int delay_pngSize;
    static const char* lfo_png;
    static const int lfo_pngSize;
    static const char* texture_jpg;
    static const int texture_jpgSize;
    static const char* _159374_jpg;
    static const int _159374_jpgSize;
    static const char* lim_png;
    static const int lim_pngSize;
    static const char* logo_small_png;
    static const int logo_small_pngSize;
    static const char* _159374_jpg2;
    static const int _159374_jpg2Size;
    static const char* synth_def_png;
    static const int synth_def_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DelayFXAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    std::unique_ptr<SliderAttachment> timeAttachment;
    std::unique_ptr<SliderAttachment> fbAttachment;
    std::unique_ptr<SliderAttachment> dwAttachment;
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> amtAttachment;
    std::unique_ptr<SliderAttachment> wfAttachment;
    std::unique_ptr<SliderAttachment> lsAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;

    MyLookAndFeel temaBellissimo;
    MyLookAndFeel temaBellissimo_con_4_tick;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> timeSlider;
    std::unique_ptr<juce::Slider> fbSlider;
    std::unique_ptr<juce::Slider> dwSlider;
    std::unique_ptr<juce::Slider> rateSlider;
    std::unique_ptr<juce::Slider> amtSlider;
    std::unique_ptr<juce::Slider> wfSlider;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::Slider> lsSlider;
    std::unique_ptr<Meter> meterLed;
    juce::Path internalPath1;
    juce::Image cachedImage_logo_small_png_2;
    juce::Image cachedImage_synth_def_png_3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

