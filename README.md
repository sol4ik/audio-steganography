# audio-steganography
Final project for Principles of Computer Organisation course by **[Solomiia Leno](https://github.com/sol4ik)** and **[Ihor Titov](https://github.com/lurak)** on **steganographic tranfer of data by means of audio signals**.

## ::table of contents
1. [requirements](#requirements)
2. [algorithms](#algorithms)
   1. [coding](#coding)
   2. [decoding](#decoding)
   3. [model](#model)
3. [hardware](#hardware)
4. [updates](#updates)
---
## requirements
At the start of project development, we got the following requirements for both hardware part and the algorithm realization:
* **two devices** for coding and decoding the message
* stability to **archeiving of the sound**
* ability to transfer the sound by phone-lines
* **real-time** message decoding
* ability to **correct errors in message**, in case of occurance
---

## algorithms
As our project consists of two processes - coding and decoding the message - we've been developing two different algorithms.

### coding
#### :: text coding
First of all, as we get the text message to hide within the audio signal we need to code it.

For **text message coding** we use ASCII table decimal codes converted to binary numbers of 8 digits. 

**example:**

![text coding](https://github.com/sol4ik/audio-steganography/blob/master/pics/text_coding.png)

*P.S. Since ASCII table is quite large, for starters we're going to use the number of letter in alphbet,meaning we'll be able code only 26 small latin letters and a space*.

*P.S. We were also thinking on using **Huffman code** insted in case we need to transfer a long text message, but for now we're focusing on the one descibed above.*

As we also need to detect and correct errors in the transfered message, we apply the **Hamming codes**, as we are guaranted errors detenction with only **log2 n** control bits per **n** information bits. 

#### :: digital signal hiding
For message hiding we've researched different approaches as echo hiding, lower tones insertion and so on but we decided to implement the basic one - **amplitude modulation**.

Since amplitude modulation is very basic, it's pretty easy to be detected even by human himself. That's why we tried one more approach - **phase modulation**.

This one is much mode stable to noises but quite easy to detect.

For now, we have some other ideas for the algorithm including the ones using noise generation and Fourier transform but we stoped on previous two for the hardware implementation.

#### :: sound generation

As for starters, we're generating the sound by ourselves. 

Basically, we generate a single sine wave and modulate it with the information signal.

For the sine generation we're using simple Maclaurin series expansion for half-period of sine.


### decoding
As we're currently working out the amplitude modulation model, the decoding algorithm is **amplitude demodulation**.

To be more precise, we're using **envelope method** based on peeks calculation. Although this method is very unstable for the noises in sound wave and it's quite hard to implement derivatives calculation on the hardware, so we're considering working out **square law detection**.

---
### ::model
In order to test the algorithm we developed a simulation model in Python.

Here are the commands needed if you want to test it on your own.

*P.S. The model works with **.wav** file format*.

    git clone https://github.com/sol4ik/audio-steganography.git
    cd audio-steganography/model
    
    pip install -r requirements.txt
    
The main file has the following options to run:
* **code / decode** mode - you choose whether you want to code some message or decode from existing .wav file
* [optional] **log** - you choose whether you want to see the steps of coding/decoding functions
    
      python main.py code log

---
## hardware
Our first choise for the hardware part of the project was **[fm4-176l-s6e2cc-eth-Arm® Cortex®-M4 MCU Starter Kit with Ethernet and USB Host](https://www.cypress.com/documentation/development-kitsboards/sk-fm4-176l-s6e2cc-fm4-family-quick-start-guide)**. 

**IDE** choice for the development was **[Keil µVision® IDE 5](http://www2.keil.com/mdk5/uvision/)**.

Since finding decent data sheet and examples for this device turned out to be too hard, we decided to use other device.

Our next choice was **[PSoC 4 BLE Pioneer Kit](https://www.cypress.com/products/psoc-4-ble-bluetooth-smart)**.

The IDE choise is naturally **PSoC creator**.

Since DAC is not implemented in PSoC we decedid to generate the needed sound wave using **PWM**.

Here's the **Top Design** for the modulating part.

![top design](https://github.com/sol4ik/audio-steganography/blob/master/pics/top_design_am.jpg)

---
## ::updates
### ::until Dec 24
* meetings with mentor
* algorithms reseraches
* built coding algorithm
* developed model for coding algorithm
* process on decoding algorithm
* read all the datasheets for FM4
* researes of examples for sound generation

**current problems**
* no working examples for the FM4 (only the ones for LED blinking)

### ::until Jan 20 [demo]
* change devoce for hardware implementation
* implement amplitude modulation algorithm
* imlement Python mode for almplitude modulation and demodulation

**current problems**
* the only way to reproduce the sound wave is to see it using analyzer and LA-viewer

---
