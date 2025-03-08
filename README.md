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
The phaser unit is modeled after the Electro-Harmonix Small Stone EH4800 Phase Shifter. The phase shifting effect is achieved by chaining together four All Pass filters; for this reason, it is also said to be a "four stages pedal". Specifically, the plugin makes use of four First-Order IIR All-Pass filters. The Transfer Function is:
<p align="center">
$$H_{AP} = \frac{a_1 + z^{-1}}{1 + a_1z^{-1}}$$
</p>
The $a_1$ coefficient controls the _break frequency_ of the All-Pass filter, that is, the frequency at which a phase shift of -$\frac{pi}{2}$
A color switch, enables a feedback line that adds 80% of the output.

## How to install
### Windows
Simply copy the .vst3 file in your system VST3 folder. Usually this is located at:
<p align="center">
C:\Program Files\Common Files\VST3
</p>
