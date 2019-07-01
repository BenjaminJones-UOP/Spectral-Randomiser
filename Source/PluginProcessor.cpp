/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ModelViewController.h"

//==============================================================================
SpectralRandomiserAudioProcessor::SpectralRandomiserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
m_testScramble(10)
#endif
{
    m_formatManager.registerBasicFormats();
    m_transportSource.addChangeListener(this);
	m_canProcess = false;
	m_isProcessed = false;
    doOnce = true;
    m_playOriginal = true;
}

SpectralRandomiserAudioProcessor::~SpectralRandomiserAudioProcessor()
{
}

//==============================================================================
const String SpectralRandomiserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpectralRandomiserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpectralRandomiserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpectralRandomiserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpectralRandomiserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpectralRandomiserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SpectralRandomiserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpectralRandomiserAudioProcessor::setCurrentProgram (int index)
{
}

const String SpectralRandomiserAudioProcessor::getProgramName (int index)
{
    return {};
}

void SpectralRandomiserAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SpectralRandomiserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    samplesPerBlock = 1024;
    m_transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void SpectralRandomiserAudioProcessor::releaseResources()
{
    m_transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpectralRandomiserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SpectralRandomiserAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    AudioSourceChannelInfo* bufferToFill = new AudioSourceChannelInfo(&buffer, 0, buffer.getNumSamples());
    
    if(m_readerSource.get() == nullptr)
    {
        bufferToFill->clearActiveBufferRegion();
        return;
    }
    
    if(m_playOriginal){
        m_transportSource.getNextAudioBlock(*bufferToFill);
    }
    
    //Debug to print the buffer, check if buffer exists
    /*
    if(doOnce){
        for(int i = 0; i < currentFileBuffer.getNumSamples(); ++i){
            std::cout<<currentFileBuffer.getSample(0, i)<<std::endl;
        }
        doOnce = false;
    }
    */
    
	if (m_readerSource.get() != nullptr) 
	{
		//DBG("File loaded");
		if (m_canProcess) {
            if(!m_isProcessed){
                DBG("Processing...");
                m_testScramble.process(currentFileBuffer);
                m_isProcessed = true;
            }
            DBG("Processed in plugin processer");
            m_transportSource.getNextAudioBlock(*m_testScramble.getProcessedBuffer());
            DBG("Transport source from new buffer");
            m_playOriginal = false;
		}

//        if (m_isProcessed)
//        {
//            DBG("Processed");
//
//            m_isProcessed = false;
//        }
	}
    
    

}


//Custom/added functions
void SpectralRandomiserAudioProcessor::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == &m_transportSource)
    {
        m_isPlaying = m_transportSource.isPlaying();
        std::cout << "is playing = " << m_isPlaying << std::endl;
        
    }
}

//Open file into the transport source and fill that file's samples into a buffer for processing
void SpectralRandomiserAudioProcessor::openButtonClicked()
{
    FileChooser chooser ("Select a Wave file to play...",{},"*.wav");
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto* reader = m_formatManager.createReaderFor(file);
        
        if(reader != nullptr)
        {
            std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
            currentFileBuffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
            reader->read(&currentFileBuffer, 0, (int)reader->lengthInSamples, 0, true, true);
            m_transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            m_readerSource.reset(newSource.release());
        }
    }
}

//==============================================================================
bool SpectralRandomiserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SpectralRandomiserAudioProcessor::createEditor()
{
    return new SpectralRandomiserAudioProcessorEditor (*this);
}

//==============================================================================
void SpectralRandomiserAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SpectralRandomiserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralRandomiserAudioProcessor();
}
