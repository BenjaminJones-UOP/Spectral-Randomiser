/*
  ==============================================================================

    BinScrambler.h
    Created: 3 Apr 2019 5:31:15pm
    Author:  asmprog

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

typedef dsp::Complex<float>* DSPComplex;
typedef std::vector<DSPComplex> VectorComplex;

//=================================================================================

class BinScrambler 
{
public:
    
    BinScrambler(float fftOrder):
    m_performFFT(fftOrder),
    m_fftOrder(fftOrder),
    m_fftSize(m_performFFT.getSize())
    {
		
        //sets the sizes for the vectors
        for (int i = 0; i < m_fftSize; ++i){
            processedBuffer.push_back(0);
			//m_input.push_back(DSPComplex(0));
            //m_output.push_back(DSPComplex(0));
            //m_spectral.push_back(DSPComplex(0));
        }
		
    }
    
    ~BinScrambler()
    {
        
    }
    
	
	//PROCESS FUNCTION THROWS THE RUN TIME ERROR
    void process(AudioBuffer<float>& bufferFill)
    {
        float* bufferData[int(m_fftSize*2)]; //create a float array at two times the fft size
        DBG("Setting buffer...");
        for (int i = 0; i < m_fftSize*2; ++i) {
            bufferData[i] = bufferFill.getWritePointer(0, i);
            //Debug to check if the incoming buffer matches the buffer from the processor
            //std::cout << "Line number in set buffer = " << i << std::endl;
            std::cout << *bufferData[i] << std::endl;
        }
        //fill the data from the incoming buffer
        //bufferData now contains the buffer data
        
        //Performs FFT on the buffer array into interleaved real and imag
        //Real is all even numbers i.e. 0,2,4,6 etc.
        //Imag is all odd numbers i.e. 1,3,5,7 etc.
        m_performFFT.performRealOnlyForwardTransform(*bufferData);
        
        DBG("Performed forward transform");
        
        //Real is the x axis
        //Imag is the y axis
        //Magnitude is the hypotenuse of the x and y
        //Phase is the angle between the x and y, acos(magnitudeData[mag]/bufferData[real])
        
        float magnitudeData[(int)m_fftSize];
        
        //Getting the magnitude data from the spectral data
        for (int real = 0, imag = 1, mag = 0; real < m_fftSize*2; real+=2, imag+=2, mag = real/2) {
            magnitudeData[mag] = sqrtf((pow(*bufferData[real], 2.0)) + (pow(*bufferData[imag], 2.0)));
        }
        
        //Manipulate magnitude data here
        //Noise gate?
        for(int i = 0; i< m_fftSize/2; ++i){
            if(magnitudeData[i]<=0.1f){
                magnitudeData[i] = 0.0f;
            }
        }
        
        //Putting the processed data back into the buffer
        //new real = magnitude * cos(acos(magnitude/old real)) (aka magnitude/old real) cos(cos-1) counteract
        //new imag = magnitude * sin(asin(magnitude/old imag)) (aka magnitude/old imag)
        
        for(int real = 0, imag = 1, mag = 0; real < m_fftSize*2; real+=2, imag+=2, mag = real/2){
            *bufferData[real] = magnitudeData[mag] * (magnitudeData[mag] / *bufferData[real]);
            *bufferData[imag] = magnitudeData[mag] * (magnitudeData[mag] / *bufferData[imag]);
        }
        
        m_performFFT.performRealOnlyInverseTransform(*bufferData);
        
        DBG("Performed inverse transform");
        
//        for(int i = 0; i < m_fftSize; ++i){
//            std::cout << "processed buffer = " << *bufferData[i] << std::endl;
//        }
        
        DBG("buffer from spectral");
        
        //memcpy(*processedBuffer, *bufferData, sizeof(m_fftSize));
        
        //Make sure processedBuffer is in range
        for(int i = 0; i < m_fftSize; ++i){
            processedBuffer[i] = bufferData[i];
            if(*processedBuffer[i]<=-0.99f){
                *processedBuffer[i]=-0.99f;
            }
            if(*processedBuffer[i]>=0.99f){
                *processedBuffer[i]=0.99f;
            }
            std::cout << "processed buffer = " << *processedBuffer[i] << std::endl;
        }
        
        DBG("Processed buffer");
        
        
        //Setting complex input from buffer
        //Attempt at Juce's out of place FFT perform function
        
//        for (int i = 0; i < m_fftSize; ++i) {
//
//        }
//
//        //perform fft on input into spectral
//        for (int i = 0; i < m_fftSize; ++i) {
//            const dsp::Complex<float>* tempInput(m_input[i]);
//            dsp::Complex<float>* spectralDataTemp = nullptr;
//            //spectralDataTemp = new dsp::Complex<float>(0);
//            //..............................Get stuck here - write access violation
//            m_performFFT.perform(tempInput, spectralDataTemp, false);
//            m_spectral[i] = spectralDataTemp;
//            std::cout << "Line number = " << i << std::endl;
//        }
//
//        DBG("Buffer into spectral...");
//        //do stuff to spectral here
//
//        //perform fft back from spectral into output
//        for (int i = 0; i < m_fftSize; ++i) {
//            const dsp::Complex<float>* spectralDataTemp(m_spectral[i]);
//            dsp::Complex<float>* tempOutput = nullptr;
//            //tempOutput = new dsp::Complex<float>(0);
//
//            m_performFFT.perform(spectralDataTemp, tempOutput, true);
//            m_output[i] = tempOutput;
//        }

		//DBG("Spectral into output...");

    };
    
    AudioSourceChannelInfo* getProcessedBuffer(){
		AudioSourceChannelInfo* buffer;
        AudioBuffer<float> tempBuffer(2, m_fftSize);
        
        DBG("creating buffer from processed buffer");
        
        //tempBuffer.copyFrom(0, 0, *processedBuffer.data(), processedBuffer.size());
        
        for (int i = 0; i < m_fftSize; ++i) {
            tempBuffer.addSample(0, i, *processedBuffer[i]);
            //std::cout << "Line number in processing buffer = " << i << std::endl;
        }
        
        DBG("Buffer created");

		//tempBuffer = new AudioBuffer<float>;
        
//        for (int i = 0; i < m_fftSize; ++i) {
//            float sample;
//            sample = m_output[i]->real();
//
//            tempBuffer.addSample(0, i, sample);
//        }
        
        DBG("Returning buffer");
        
		buffer = new AudioSourceChannelInfo(&tempBuffer, 0, tempBuffer.getNumSamples());

		return buffer;
    }
    
    //process:
    //Take the buffer and put it into a vector of complex numbers
    //---Put buffer into input.real
    //forward fft the vector
    //Take each individual bin and randomise/scramble it
    //inverse fft the vector back into the buffer
    //play the buffer
    
    
    //This was to set the input of the buffer into the vector of complex floats for the out of place FFT perform function
    
    /*
	VectorComplex setInput(AudioBuffer<float> bufferFill) {
        VectorComplex input(m_fftSize);
        DSPComplex buffer[int(m_fftSize)];
		
		DBG("Allocating input memory...");

		for (int i = 0; i < bufferFill.getNumSamples(); ++i) {
			input.push_back(DSPComplex(0));
		}

		DBG("Inside setting input...");
        
        //error here
		for (int i = 0; i < bufferFill.getNumSamples(); ++i) {
            buffer[i] = DSPComplex(bufferFill.getReadPointer(0, i));
		}
        
        
        
        DBG("Set buffer array");
        
        for (int i = 0; i < bufferFill.getNumSamples(); ++i) {
            input.at(i) = buffer[i];
        }
        
		return input;
	}*/


private:
    
    dsp::FFT m_performFFT;
    
    float m_fftOrder;
    float m_fftSize;
    std::vector<float*> processedBuffer;
    
    VectorComplex m_input, m_spectral, m_output;

    float m_writePos;
    
    bool nextFFTBlockReady = false;
    
    
};
