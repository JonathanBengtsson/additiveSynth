# Additive Synthesizer
Additive synthesizer with reverb effect


A virtual synthesizer using additive sound synthesis (sum of the first 30 sine waves sin(nx), n = 1, .., 30 in a Fourier series, where you can set amplitudes). It has a vibrato effect and an ADSR amplitude envelope filter (attack, decay, sustain, release). It is possible to connect a MIDI keyboard. There is a sequencer. In this synthesizer I have included a reverb effect, where it is possible to set parameters for up to 10:th order reflections.

To compile: cl ads.c -link user32.lib Gdi32.lib Comdlg32.lib

The sound processing takes place in the fillBuffer() function.

A short demonstration: https://youtu.be/jGMJsbsysoo
