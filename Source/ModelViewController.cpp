/*
  ==============================================================================

    ModelViewController.cpp
    Created: 26 Feb 2019 1:45:54pm
    Author:  asmprog

  ==============================================================================
*/

#include "ModelViewController.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

ModelViewController::ModelViewController(SpectralRandomiserAudioProcessorEditor* editor, SpectralRandomiserAudioProcessor* processor):
m_editor(editor),
m_processor(processor)
{
    //Do gui shit here with gets and sets of my processor using lambdas
    //open button
    m_editor->m_openButton.onClick = [this]
    {
        m_processor->openButtonClicked();
        m_editor->m_playButton.setEnabled(true);

        
    };
    
    //play button
    m_editor->m_playButton.onClick = [this]
    {
        if (m_processor->m_transportSource.isPlaying())
        {
            changeState(Pausing);
        }
        else changeState(Starting);
    };
    
    //stop button
    m_editor->m_stopButton.onClick = [this]
    {
        changeState(Stopping);
       
    };

	m_editor->m_processButton.onClick = [this]
	{
		m_processor->m_canProcess = true;
	};
}

ModelViewController::~ModelViewController()
{
    
}

void ModelViewController::changeState(TransportState newState)
{
    
    
    if(m_state != newState)
    {
        m_state = newState;
        switch(m_state)
        {
            case Stopped:
                m_processor->m_transportSource.setPosition(0.0);
                break;
                
            case Starting:
                m_processor->m_transportSource.start();
                m_processor->m_isPlaying = 1;
                changeState(Playing);
                break;
                
            case Playing:
                m_editor->m_stopButton.setEnabled(true);
                m_editor->m_playButton.setButtonText("Pause");
                
                startTimer(10);
                
                break;
                
            case Pausing:
                m_processor->m_transportSource.stop();
                changeState(Paused);
                break;
                
            case Paused:
                m_editor->m_playButton.setButtonText("Play");
                break;
                
            case Stopping:
                m_processor->m_transportSource.stop();
                changeState(Stopped);
                m_editor->m_playButton.setButtonText("Play");
                m_editor->m_stopButton.setEnabled(false);
                break;
                
        }
    }
}

void ModelViewController::timerCallback()
{
    if (m_processor->m_transportSource.hasStreamFinished())
    {
        changeState(Stopping);
        DBG("Stopped");
        stopTimer();
    }
    
}

