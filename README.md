# additiveSynth
Additive synthesizer with reverb effect


A virtual synthesizer using additive sound synthesis (set amplitude for the first sine waves sin(nx), n = 1, .., 30 in a fourier series and sum). It has a vibrato effect and an ADSR amplitude envelope filter (attack, decay, sustain, release). It is possible to connect a MIDI keyboard. There is a sequencer. In this synthesizer I have included a reverb effect, where it is possible to set parameters for up to 10:th order reflections.

To compile: cl ads.c -link user32.lib Gdi32.lib Comdlg32.lib

The soundprocessing takes place in the fillBuffer()-function.
