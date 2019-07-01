/*
  ==============================================================================

    ModelViewController.h
    Created: 26 Feb 2019 1:45:54pm
    Author:  asmprog

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SpectralRandomiserAudioProcessor;
class SpectralRandomiserAudioProcessorEditor;

class ModelViewController : public Timer
{
public:
    ModelViewController(SpectralRandomiserAudioProcessorEditor* editor,
                        SpectralRandomiserAudioProcessor* processor);
    
    ~ModelViewController();
    
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    }m_state;
    void changeState(TransportState newState);
    void timerCallback();

    
    
    
private:
    SpectralRandomiserAudioProcessorEditor* m_editor;
    SpectralRandomiserAudioProcessor* m_processor;
};
