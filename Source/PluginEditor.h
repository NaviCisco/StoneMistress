/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Theme.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
/**
*/
class StoneMistressAudioProcessorEditor  : public juce::AudioProcessorEditor, public MouseListener
{
public:
    StoneMistressAudioProcessorEditor (StoneMistressAudioProcessor&, AudioProcessorValueTreeState&);
    ~StoneMistressAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void setupSlider(Slider& slider, int x, int y, int side);
    void setupToggle(ToggleButton& toggle, int x, int y, int side);

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    StoneMistressAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& valueTreeState;

    Slider rateSlider;
    Slider phaserDepthSlider;
    Slider chorusDepthSlider;
    ToggleButton colorSwitch;

    MyLookAndFeel myTheme;

    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> phaserDepthAttachment;
    std::unique_ptr<SliderAttachment> chorusDepthAttachment;
    std::unique_ptr<ButtonAttachment> colorAttachment;

    Image colorSwitchOnImage;
    Image colorSwitchOffImage;
    Image backgroundImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoneMistressAudioProcessorEditor)
};
