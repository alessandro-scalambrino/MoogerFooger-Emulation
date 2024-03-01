# Moogerfooger Analog Delay VST Plugin

## Overview
Moogerfooger Analog Delay VST Plugin is a digital recreation inspired by the Moog Moogerfooger MF-104M Analog Delay unit. This plugin is developed using C++ and the JUCE framework, aiming to replicate the warm analog delay sound with modulation and saturation effects.
<div style="text-align: center; margin-top: 20px;">
  <img src="https://github.com/alessandro-scalambrino/MoogerFooger-Emulation/assets/156260456/9bd1b14a-e47a-4d94-a9fe-d7869479938b" alt="Moogerfooger Analog Delay GUI" width="400">
</div>


### Features
- **Bucket Brigade Delay (BBD) Emulation**: The plugin emulates the BBD delay line used in the Moogerfooger MF-104M, where the analog signal is moved along a series of capacitors, creating a warm and rich delay effect.
- **Modulation**: Modulate the delay time using LFOs with various shapes (Sine, Triangle, Saw up, Saw down, Noise) to create dynamic and evolving delays.
- **Saturator and Softclipper**: Drive the input signal with a saturator (arctg saturation with oversampling) for warmth and character. A softclipper in the feedback line prevents signal clipping.
- **Filters**: Filters affect the signal before and after the delay line, shaping the tone of the delayed signal. Two cutoff modes are available (short mode: 3000Hz, long mode: 17000Hz).
- **LED Meter**: Display signal levels with different colors (red for >=0dbfs, yellow for -6db < sig < 0db, green for < -6db).
- **Plugin GUI**: Intuitive GUI with parameters for Drive, Cutoff, Time, Feedback, Waveform (Sine, Triangle, Saw up, Saw down, Noise), Amount, Rate, Mix, and Dark/Bright switch for tone shaping.

## Usage
1. Build your own vst from source code.
2. Open your DAW and load the Moogerfooger Analog Delay plugin on a track.
3. Adjust parameters to create analog-style delay effects with modulation, saturation, and filtering.
4. Experiment with different LFO shapes and rates for dynamic effects.
5. Enjoy the warm sound of the Moogerfooger Analog Delay!

## Development
- **Framework**: Developed using C++ and the JUCE framework.
- **Version**: v1.0
- **Developed by**: Alessandro Scalambrino
- **University**: Lim UniMi (https://www.lim.di.unimi.it/)
- **Contact**: alessandro.scalambrino.work@gmail.com

