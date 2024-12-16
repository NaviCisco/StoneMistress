/*
  ==============================================================================

    Parameters.h
    Created: 30 Sep 2024 11:17:23am
    Author:  Ivan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Parameters
{
    //CONSTANTS
    static const double maxDelayTime = 5.1;

    // PARAMETER IDs
    static const String nameRate = "RT";
    static const String namePhaserDepth = "PD";
    static const String nameChorusDepth = "CD";
    //static const String nameColor = "CLR";

    // PARAMETER DEFAULTS
    static const float defaultRate = 0.5f;
    static const float defaultPhaserDepth = 250.0f;
    static const float defaultChorusDepth = 3.0f;
    //static const bool defaultColor = false;

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

        parameters.push_back(std::make_unique<AudioParameterFloat>(nameRate, "Rate", NormalisableRange<float>(0.1f, 8.0f, 0.01f), defaultRate));
        parameters.push_back(std::make_unique<AudioParameterFloat>(namePhaserDepth, "Phaser Depth", NormalisableRange<float>(0.000f, 6000.0f, 1.0f), defaultPhaserDepth));
        parameters.push_back(std::make_unique<AudioParameterFloat>(nameChorusDepth, "Chorus Depth", NormalisableRange<float>(0.0f, 15.0f, 1.0f), defaultChorusDepth));
        //parameters.push_back(std::make_unique<AudioParameterBool>(nameColor, "Color", defaultColor));

        return { parameters.begin(), parameters.end() };
    }

    static void addListenerToAllParameters(AudioProcessorValueTreeState& valueTreeState, AudioProcessorValueTreeState::Listener* listener)
    {
        std::unique_ptr<XmlElement> xml(valueTreeState.copyState().createXml());

        for (auto* element : xml->getChildWithTagNameIterator("PARAM"))
        {
            const String& id = element->getStringAttribute("id");

            valueTreeState.addParameterListener(id, listener);
        }
    }
};