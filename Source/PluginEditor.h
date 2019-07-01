/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ModelViewController.h"

//==============================================================================
/**
*/
class SpectralRandomiserAudioProcessorEditor  : public AudioProcessorEditor,
                                                public Timer

{
public:
    SpectralRandomiserAudioProcessorEditor (SpectralRandomiserAudioProcessor&);
    ~SpectralRandomiserAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SpectralRandomiserAudioProcessor& processor;

    TextButton m_openButton, m_playButton, m_stopButton, m_processButton;
    
    
    //put at bottom
    ModelViewController m_controller;
    friend class ModelViewController;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralRandomiserAudioProcessorEditor)
};
