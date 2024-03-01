#pragma once
#include <JuceHeader.h>

// Useful constants
#define MAX_DELAY_TIME 5.00f
#define TIME_SMOOTHING 0.1f
#define FBK_SMOOTHING  0.1f
#define LEVEL_SMOOTHING_TIME 0.1f
#define GLIDE_TIME 0.1f
#define SMOOTHING_TIME 0.1f

//filter parameters
#define MAX_NUM_CH 2
#define DEFAULT_R 0.25
#define NAME_R "release"
#define DEFAULT_F 3000.0f
#define DEFAULT_Q 0.707f
#define NAME_Q "quality"

//meter parameters
#define FPS       25
#define DB_FLOOR -48.0f
#define RT         0.5f

//0verex parameters
#define TARGET_SAMPLING_RATE 192000.0
#define DEFAULT_SUBTLESAT 0.3f



// Actual parameters
#define NAME_DW "dw"
#define NAME_DT "dt"
#define NAME_FB "fb"
#define NAME_FREQ "lfoFreq"
#define NAME_MOD "timeMod"
#define NAME_WF "waveform"
#define NAME_LS "longshort"
#define NAME_SAT "gain"

#define DEFAULT_DW 0.5f
#define DEFAULT_DT 0.5f
#define DEFAULT_FB 0.0f
#define DEFAULT_FREQ 1.0f
#define DEFAULT_MOD 0.0f
#define DEFAULT_WF 0
#define DEFAULT_LS 0
#define DEFAULT_SAT 0.1f




namespace Parameters
{
	static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
	{
		std::vector<std::unique_ptr<RangedAudioParameter>> params;

		
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_DW, "Dry/Wet", 0.0f, 1.0f, DEFAULT_DW));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_FB, "Feedback", 0.0f, 1.0f, DEFAULT_FB));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_DT, "Delay time (s)", NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 0.001f, 0.3f), DEFAULT_DT));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_FREQ, "LFO Freq (Hz)", NormalisableRange<float>(0.1f, 5.0f, 0.01f, 0.3f), DEFAULT_FREQ));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_MOD, "Mod amount (s)", NormalisableRange<float>(0.0f, MAX_DELAY_TIME / 2.0f, 0.001f), DEFAULT_MOD));
		params.push_back(std::make_unique<AudioParameterChoice>(NAME_WF, "LFO shape", StringArray{ "Sin", "Tri", "Saw up", "Saw down","Square", "S&h"}, DEFAULT_WF));
		params.push_back(std::make_unique<AudioParameterChoice>(NAME_LS, "LongShort", StringArray{ "Dark", "Bright" }, DEFAULT_LS));
		params.push_back(std::make_unique<AudioParameterFloat>(NAME_SAT, "Gain", NormalisableRange<float>(0.1f, 10.0f, 0.1f, 0.5f), DEFAULT_SAT));

		return {params.begin(), params.end()};
	}
}