# Stone Mistress
Lush stereo chorus and phaser inspired by the Electro-Harmonix Small Stone EH4800 and Stereo Electric Mistress pedals\
\
![image alt](https://github.com/NaviCisco/StoneMistress/blob/fbf9a692fab03b4b93e5b63d7dc0cc7425a01049/images/StoneMistressShowcasePNG.png)

## Controls
- **Rate**: Controls the rate of the LFO signal that modulates the speed at which the notches created by the All-Pass filters sweep back and forth across the spectrum.
The same principle is applied to the comb filter created by the delay unit that originates the chorus effect.
- **Phaser depth**: Controls the sweeping range of the All-Pass notches. The more the Depth, the more pronounced the phasing effect.
- **Chorus depth**: Controls the amount of delay time applied by the delay unit. The more you turn the knob clockwise, the more pronounced the chorus effect.
- **Color**: When engaged, feedback is sent back to the Phaser input. Notice how the Stone Mistress carves into the frequency spectrum hollowing out the sound. Fundamentals and harmonics glide in and out for an extremely pronounced phasing/shifting effect.


## Overview
The Stone Mistress consists of a Phaser and a Chorus unit in chain, modulated by a single LFO generator.\
\
![iamge alt](https://github.com/NaviCisco/StoneMistress/blob/73539c8d28a7e338106b711aa7d24e4219356605/images/StoneMistressDiagram.png)

### Phaser
The phaser unit is modeled after the Electro-Harmonix Small Stone EH4800 Phase Shifter. The phase shifting effect is achieved by summing the _dry_ and the _wet_ signal: when the phase shifted signal (the wet signal) adds up to the dry signal, certain frequency components cancel each others out creating _notches_ (the points in the spectrum where the magnitude is 0). These notches are subsequently modulated, effectively producing the typical sweeping tone of a phaser pedal.\
The Small Stone is said to be a "four stages pedal" because it is made up of 4 all-pass filters. Specifically, the plugin is implemented with four first-order Infinite Impulse Response (IIR) all-pass filters. The transfer function of an all-pass filter is:

$$H_{AP} = \frac{a_1 + z^{-1}}{1 + a_1z^{-1}}$$

The $a_1$ coefficient controls the _break frequency_ $f_b$ of the all-pass filter, that is, the frequency at which a phase shift of $-\frac{\pi}{2}$ takes place. When two all-pass filters with the same break frequency are put in series, a total of 180° phase shift (complete phase inversion!) occurs and a notch is created when summing the dry signal. Now you can see the pattern: adding a second pair of all-pass filters set with the same break frequency will make up a second notch.\
In order to obtain the classic sweeping tone of a phaser, the $a_1$ has to be modulated. Given that the $a_1$ coefficient is given by the following formula:

$$a_1 = \frac{\tan(\pi*{f_b}/{f_s})-1}{\tan(\pi*{f_b}/{f_s})+1}$$

where $f_s$ is the sample frequency, the formula for a modulated $a_1$ coefficient is simply:

$$a_{1 mod} = \frac{\tan(\pi*{f_b **+ modValue**}/{f_s})-1}{\tan(\pi*{f_b}/{f_s})+1}$$

$modValue$ [Hz] is the amount of modulation applied per sample. For this plugin, it takes on values in the 0-3000 range.\
Lastly, a color switch, enables a feedback line that adds back to input 80% of the signal coming out of the all-pass chain.\
Full code can be inspected in the Filters.h and SmallStone.h files

### Chorus
The chorus effect is obtained by delaying a copy of the dry signal by a couple milliseconds. When the delayed copy is mixed with the original signal, not only the sound is perceived as wider, but a comb filter is created as well. This is moved back and forth along the spectrum by the LFO.\
The dry signal copy can be delayed up to 50 milliseconds.\
Full code available in the Delays.h file

### LFO
The LFO signal is triangular. There's a 180° phase offset between left and right channel, so that the stereo image can be created. The rate goes up to 8Hz.\
Code available in the Oscillator.h file.

## How to install
### Windows
Simply copy the .vst3 file in your system VST3 folder. Usually this is located at:
<p align="center">
C:\Program Files\Common Files\VST3
</p>

## Issues
On some computers, the plugin GUI might be displayed with a lower DPI resolution inside Ableton. To fix this, right-click on the plugin's name in the plugin list and check/uncheck "Autoscale plugin window"
