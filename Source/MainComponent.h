/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"




//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainContentComponent   :  public Component,
public ButtonListener,
public SliderListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();
    
    void paint (Graphics&);
    void resized();
    
    void buttonClicked (Button* button);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
    ScopedPointer<Button> startButton, stopButton;
    ScopedPointer<Slider> slider_carrier_freq, slider_mod_freq, slider_mod_index, slider_amp ;
    
    Label label_carrier_freq, label_mod_freq, label_mod_index, label_amp;

};


#endif  // MAINCOMPONENT_H_INCLUDED
