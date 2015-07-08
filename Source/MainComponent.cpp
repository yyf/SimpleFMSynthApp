/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"


#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <iostream>

#define NUM_SECONDS   (100)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (256) // 64

//#define NUM_CH (2)

//#define TABLE_SIZE   (200)
#define TABLE_SIZE  (44100)
#define MOD_TABLE_SIZE (44100)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

using namespace std;



typedef struct
{
    float sine[TABLE_SIZE];
    
    float phase;
    
    int mod_phase;
    float mod_Freq;
    float mod_Index;
    
    float amp;
    float car_freq;
    
    char message[20];

}
pa_FM_Data;

PaStreamParameters outputParameters;
PaStream *stream;
PaError err;
pa_FM_Data data;

static int paFMCallback( const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData )
{
    pa_FM_Data *data = (pa_FM_Data*)userData;
    
    float *out = (float*)outputBuffer;
    unsigned long i;
	float mod;
    
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        
		mod = (float) sin( ((double)data->mod_phase/(double)TABLE_SIZE) * M_PI * 2.);
		mod *= data->mod_Index * data->mod_Freq;
        
        *out++ = data->amp * (float) sin (((double) data->phase/(double) TABLE_SIZE) * M_PI * 2.);
        *out++ = data->amp * (float) sin (((double) data->phase/(double) TABLE_SIZE) * M_PI * 2.);
        
        data->phase += (int) ((data->car_freq + mod) * TABLE_SIZE/SAMPLE_RATE);
        if( data->phase >= TABLE_SIZE ) data->phase -= TABLE_SIZE;
        
		data->mod_phase += (int) (data->mod_Freq * MOD_TABLE_SIZE/SAMPLE_RATE);
        if( data->mod_phase >= MOD_TABLE_SIZE ) data->mod_phase -= MOD_TABLE_SIZE;
        
    }
    
    return paContinue;
}



static void StreamFinished( void* userData )
{
    pa_FM_Data *data = (pa_FM_Data *) userData;
	printf( "Stream Completed: %s\n", data->message );
}



// custom function for PA init
static void my_PA_Setup(){
    
    for(int i=0; i<TABLE_SIZE; i++ ){
		data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );  // initialize sine table
	}
    
    data.amp = 1; //initialize amplitude
	data.car_freq = 80;
    data.phase = 2;
	data.mod_phase = 1;
    
    err = Pa_Initialize();
	
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL; //portaudio talks to native api for cross-platform, that's why host api is there, ex JACK or CoreAudio

    printf("PortAudio initialized: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
}



//==============================================================================
MainContentComponent::MainContentComponent()
:
startButton(0),
stopButton(0),
slider_carrier_freq(0),
slider_mod_freq(0),
slider_mod_index(0),
slider_amp(0),
label_carrier_freq(""),
label_mod_freq(""),
label_mod_index(""),
label_amp("")
{
    setSize (600, 400);
    my_PA_Setup();

    startButton = new TextButton ("To start a tone");
    stopButton = new TextButton ("To stop a tone");
    
    slider_carrier_freq = new Slider ("Slider Carrier Freq");
    slider_mod_freq = new Slider ("Slider Mod Freq");
    slider_mod_index = new Slider ("Slider Mod Index");
    slider_amp = new Slider ("Slider Amp");
    
    addAndMakeVisible (startButton);
    addAndMakeVisible (stopButton);
    
    addAndMakeVisible (slider_carrier_freq);
    addAndMakeVisible (slider_mod_freq);
    addAndMakeVisible (slider_mod_index);
    addAndMakeVisible (slider_amp);
    
    addAndMakeVisible (label_carrier_freq);
    addAndMakeVisible (label_mod_freq);
    addAndMakeVisible (label_mod_index);
    addAndMakeVisible (label_amp);
    
    // layout
    startButton->setBounds              (100, 100, 100, 60);
    stopButton->setBounds               (400, 100, 100, 60);
    
    slider_carrier_freq->setBounds      (200, 250, 260, 20);
    slider_mod_freq->setBounds          (200, 275, 260, 20);
    slider_mod_index->setBounds         (200, 300, 260, 20);
    slider_amp->setBounds               (200, 325, 260, 20);
    
    label_carrier_freq.setBounds        (100, 250, 100, 20);
    label_mod_freq.setBounds            (100, 275, 100, 20);
    label_mod_index.setBounds           (100, 300, 100, 20);
    label_amp.setBounds                 (100, 325, 100, 20);
    


    label_carrier_freq.setText ("Car Freq", dontSendNotification);
    label_carrier_freq.setFont (Font (15.0000f, Font::plain));
    //label_amp.setJustificationType (Justification::centredLeft);

    label_carrier_freq.setEditable (false, false, false);
    label_carrier_freq.setColour (TextEditor::textColourId, Colours::black);
    label_carrier_freq.setColour (TextEditor::backgroundColourId, Colour (0x0));


    label_mod_freq.setText ("Mod Freq", dontSendNotification);
    label_mod_freq.setFont (Font (15.0000f, Font::plain));

    label_mod_freq.setEditable (false, false, false);
    label_mod_freq.setColour (TextEditor::textColourId, Colours::black);
    label_mod_freq.setColour (TextEditor::backgroundColourId, Colour (0x0));
    

    label_mod_index.setText ("Mod Index", dontSendNotification);
    label_mod_index.setFont (Font (15.0000f, Font::plain));

    label_mod_index.setEditable (false, false, false);
    label_mod_index.setColour (TextEditor::textColourId, Colours::black);
    label_mod_index.setColour (TextEditor::backgroundColourId, Colour (0x0));
    

    label_amp.setText ("Amplitude", dontSendNotification);
    label_amp.setFont (Font (15.0000f, Font::plain));

    label_amp.setEditable (false, false, false);
    label_amp.setColour (TextEditor::textColourId, Colours::black);
    label_amp.setColour (TextEditor::backgroundColourId, Colour (0x0));
    
    
    
    
    startButton->addListener(this);
    stopButton->addListener(this);
    
    slider_carrier_freq->setSliderStyle (Slider::LinearHorizontal);
    slider_carrier_freq->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    //slider->setRange(-1.0, 1.0); // for panning
    //slider_carrier_freq->setRange(0.0, 10000.0); // for FM
    slider_carrier_freq->setRange(0, 1000,1);
    slider_carrier_freq->addListener (this);
    
    slider_mod_freq->setSliderStyle (Slider::LinearHorizontal);
    slider_mod_freq->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
//    slider_mod_freq->setRange(0.0, 200.0);
    slider_mod_freq->setRange(0.0, 200.0, 0.1);
    slider_mod_freq->addListener (this);
    
    slider_mod_index->setSliderStyle (Slider::LinearHorizontal);
    slider_mod_index->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
//    slider_mod_index->setRange(0.0, 10.0);
    slider_mod_index->setRange(0.0, 10.0, 0.5);
    slider_mod_index->addListener (this);
    
    slider_amp->setSliderStyle (Slider::LinearHorizontal);
    slider_amp->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider_amp->setRange(0.0, 1.0, 0.05);
    slider_amp->addListener (this);
    
    slider_carrier_freq->setValue(200);
    slider_mod_freq->setValue(10);
    slider_mod_index->setValue(1);
    slider_amp->setValue(0.7);
}

MainContentComponent::~MainContentComponent()
{
    startButton = nullptr;
    stopButton = nullptr;
    slider_carrier_freq = nullptr;
    slider_mod_freq = nullptr;
    slider_mod_index = nullptr;
    slider_amp = nullptr;
    

    Pa_StopStream( stream );
    Pa_CloseStream( stream );
	Pa_Terminate();
    
    cout << "PortAudio terminated!" << endl;
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);
    g.setFont (Font (24.0f));
    g.setColour (Colours::yellowgreen);
    g.drawText ("Simple FM Synth App", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}


void MainContentComponent::buttonClicked (Button* button)
{
    if (button == startButton){
        std::cout<< "startButton clicked" << std::endl;
        
//        Pa_Initialize();
        
        Pa_OpenStream(
                      &stream, // address to output stream
                      NULL, /* no input (stream) */
                      &outputParameters,
                      SAMPLE_RATE,
                      FRAMES_PER_BUFFER,
                      paClipOff,      /* we won't output out of range samples so don't bother clipping them (paClipOff:automatically clipping) */
                      paFMCallback,
                      &data ); // pass the address of sine wave table

        
        Pa_SetStreamFinishedCallback( stream, &StreamFinished );
		Pa_StartStream( stream );
        
    }
    
    else if (button == stopButton){
        std::cout<< "stopButton clicked" << std::endl;
        
        Pa_StopStream( stream );
		Pa_CloseStream( stream );
    }
}

void MainContentComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == slider_carrier_freq) {
        std::cout<< "slider (carrier freq) dragged, value = " << slider_carrier_freq->getValue() << " actual data = " << data.car_freq <<  std::endl;
        data.car_freq = slider_carrier_freq->getValue();

    }

    else if (sliderThatWasMoved == slider_mod_freq) {
        std::cout<< "slider (mod freq) dragged, value = " << slider_mod_freq->getValue() << " actual data = " << data.mod_Freq << std::endl;
        data.mod_Freq = slider_mod_freq->getValue();
    }
    
    else if (sliderThatWasMoved == slider_mod_index) {
        std::cout<< "slider (mod index) dragged, value = " << slider_mod_index->getValue() << " actual data = " << data.mod_Index << std::endl;
        data.mod_Index = slider_mod_index->getValue();
    }
    
    else if (sliderThatWasMoved == slider_amp) {
        std::cout<< "slider (amp) dragged, value = " << slider_amp->getValue() << " actual data = " << data.amp << std::endl;
        data.amp = slider_amp->getValue();
    }
    
}


