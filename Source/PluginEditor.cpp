/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpectralRandomiserAudioProcessorEditor::SpectralRandomiserAudioProcessorEditor (SpectralRandomiserAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), m_controller(this, &p)
{
    addAndMakeVisible(&m_openButton);
    m_openButton.setButtonText(String("Open..."));
    
    addAndMakeVisible(&m_playButton);
    m_playButton.setButtonText(String("Play"));
    m_playButton.setColour(TextButton::buttonColourId, Colours::green);
    m_playButton.setEnabled(false);
    
    addAndMakeVisible(&m_stopButton);
    m_stopButton.setButtonText(String("Stop"));
    m_stopButton.setColour(TextButton::buttonColourId, Colours::red);
    m_stopButton.setEnabled(false);
	
	addAndMakeVisible(&m_processButton);
	m_processButton.setButtonText(String("Process"));
	m_processButton.setColour(TextButton::buttonColourId, Colours::blue);
	m_processButton.setEnabled(true);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
	
}

SpectralRandomiserAudioProcessorEditor::~SpectralRandomiserAudioProcessorEditor()
{
}

//==============================================================================
void SpectralRandomiserAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setOpacity(1.0f);
    

}

void SpectralRandomiserAudioProcessorEditor::resized()
{
    auto windowSize = getLocalBounds();
    
    m_openButton.setBounds(windowSize.removeFromTop(200).reduced(30));
    m_playButton.setBounds(windowSize.removeFromLeft(200).reduced(50));
    m_stopButton.setBounds(windowSize.removeFromRight(400).reduced(50));
	m_processButton.setBounds(windowSize.removeFromRight(200).reduced(50));
    
}

void SpectralRandomiserAudioProcessorEditor::timerCallback()
{
    
}
