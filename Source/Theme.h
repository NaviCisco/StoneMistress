/*
  ==============================================================================

    Theme.h
    Created: 12 Feb 2025 2:04:55pm
    Author:  Ivan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MyLookAndFeel : public LookAndFeel_V4
{
public:
    MyLookAndFeel()
    {
        //setColour(Slider::thumbColourId, Colours::orange);

    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        // Calculate center and radius
        const float radius = jmin(width, height) * 0.5f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;

        // Adjust indicator position
        const float innerRadius = radius * 0.6f; // Moves the base inward (adjust as needed)
        const float pointerLen = radius * 0.45f;  // Shorter pointer so it stays inside
        const float pointerThickness = 2.0f;

        // Create indicator shape
        Path p;
        p.addRectangle(pointerThickness * -0.5f, -innerRadius, pointerThickness, pointerLen);

        // Rotate & position the indicator correctly
        const float angle = jmap(sliderPosProportional, 0.0f, 1.0f, rotaryStartAngle, rotaryEndAngle);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

        // Draw the indicator
        g.setColour(Colours::white);
        g.fillPath(p);
    }

    void drawToggleButton(Graphics& g, ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        // Leave blank so that the checkbox is not visible!
    }


private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLookAndFeel)
};