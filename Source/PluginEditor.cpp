/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
StoneMistressAudioProcessorEditor::StoneMistressAudioProcessorEditor (StoneMistressAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setupSlider(rateSlider, 343.5, 114.8, 51.4); // Mancano Le coordinate come argomento!
    setupSlider(phaserDepthSlider, 93.2, 166.2, 51.4);
    setupSlider(chorusDepthSlider, 100, 263.9, 51.4);

    setupToggle(colorSwitch, 132.6, 325.76, 33.16);

    colorSwitch.onClick = [this]()
        {
            repaint(); // Force a full repaint when button state changes
        };

    rateAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameRate, rateSlider));
    phaserDepthAttachment.reset(new SliderAttachment(valueTreeState, Parameters::namePhaserDepth, phaserDepthSlider));
    chorusDepthAttachment.reset(new SliderAttachment(valueTreeState, Parameters::nameChorusDepth, chorusDepthSlider));
    colorAttachment.reset(new ButtonAttachment(valueTreeState, Parameters::nameColor, colorSwitch));

    colorSwitchOnImage = ImageCache::getFromMemory(BinaryData::ToggleSwitchUp_png, BinaryData::ToggleSwitchUp_pngSize);
    colorSwitchOffImage = ImageCache::getFromMemory(BinaryData::ToggleSwitchDown_png, BinaryData::ToggleSwitchDown_pngSize);
    backgroundImage = ImageCache::getFromMemory(BinaryData::StoneMistressGUI3D7_png, BinaryData::StoneMistressGUI3D7_pngSize);

    //rateSlider.setLookAndFeel(&myTheme);  Applica il tema al singolo componente
    //Applica il tema al padre e quindi a tutti i componenti figli.
    this->setLookAndFeel(&myTheme);

    setSize (401.6, 477);
}

StoneMistressAudioProcessorEditor::~StoneMistressAudioProcessorEditor()
{
    //rateSlider.setLookAndFeel(nullptr);
    this->setLookAndFeel(nullptr);
}

//==============================================================================
void StoneMistressAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }

    if (colorSwitch.getToggleState())
    {
        g.drawImage(colorSwitchOnImage, 134.76, 320.8, 26.13, 35.4, 0, 0, colorSwitchOnImage.getWidth(), colorSwitchOnImage.getHeight());
    }
    else
    {
        g.drawImage(colorSwitchOffImage, 132.6, 333.43, 28.23, 28.46, 0, 0, colorSwitchOffImage.getWidth(), colorSwitchOffImage.getHeight());
    }

    /*g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);*/
}

void StoneMistressAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void StoneMistressAudioProcessorEditor::setupSlider(Slider& slider, int x, int y, int side)
{
    float dpiScale = 300 / 72;

    int correctedSide = side * dpiScale * 0.38;

    int correctedX = x - (correctedSide * 0.5);
    int correctedY = y - (correctedSide * 0.5);

    //int correctKnobWidth = w * dpiScale;
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    addAndMakeVisible(&slider);

    slider.setBounds(correctedX, correctedY, correctedSide, correctedSide);
}

void StoneMistressAudioProcessorEditor::setupToggle(ToggleButton& toggle, int x, int y, int side)
{
    float dpiScale = 300 / 72;

    int correctedSide = side * dpiScale * 0.38;

    int correctedX = x - (correctedSide * 0.5);
    int correctedY = y - (correctedSide * 0.5);

    toggle.setClickingTogglesState(true);
    toggle.setBounds(x + 8, y, side, side);
    addAndMakeVisible(toggle);
}