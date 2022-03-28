# 5-Band-Equalizer
Equalization meaning is to adjust the balance between frequency components of an audio signal, this is 
done with an equalizer: a device that boost or reduce the various frequency ranges.
This function is particularly useful in audio production contests or in live events where there is the need to 
give the audio signal specific characteristics.
Normally the range of audible frequencies for human is from 20 to 20˙000 Hz so we divided that range in 5 
bands.
We also included a VuMeter for each band to simply visualize the effects of the equalization process in the 
audio signal.
The hardware is realized with 5 second order band pass filters (multiple gain).
To realize the VuMeter we needed a led matrix driver. We ended up using a microcontroller to reduce the 
PCB’s dimension and cost compared to five single dot led bar driver (one for each channel), and allows to 
implement some useful algorithms.
We used an AtMega32 because it has 8 channels whit 10-bit ADCs and opted for the THT version to 
improve repairability simplifying replacement process.
The firmware periodically controls the ADCs input and, after compensating the error caused by the average 
value shifting, it calculates the average value of the peaks reached by the signal and drive the led matrix 
with the appropriate values.
We used port-A for the ADCs, port-C to drive the led matrix’s rows and port-D to drive the led matrix’s 
columns.
