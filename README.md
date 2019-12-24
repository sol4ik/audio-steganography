# audio-steganography
Final project for Principles of Computer Organisation course by **[Solomiia Leno](https://github.com/sol4ik)** and **[Ihor Titov](https://github.com/lurak)** on **steganographic tranfer of data by means of audio signals**.

## ::table of contents
1. [requirements](#requirements)
2. [algorithms](#algorithms)
   1. [coding](#coding)
   2. [decoding](#decoding)
   3. [model](#model)
3. [hardware](#hardware)
---
## requirements
At the start of project development, we got the following requirements for both hardware part and the algorithm realization:
* **two devices** for coding and decoding the message
 * **i/o for coding device**
  * *(not decided yet)*
  * FM4 CN5 Jack for headphone
 * **i/o for decoding device**
  * UART *(not approved yet)*
  * LCD screen
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

As we also need to detect and correct errors in the transfered message, we apply the **Hamming codes**, as we are guaranted errors detenction with only **log2 n** control bits per **n** information bits. 

#### :: digital signal hiding
For message hiding we've researched different approaches as echo hiding, lower tones insertion and so on but we decided to implement the basic one - **amplitude modulation**.

Since amplitude modulation is very basic, it's pretty easy to be detected even by human himself.

#### :: sound generation

### decoding
*(info yet to be written)*

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
For the hardware part of the project we have **[fm4-176l-s6e2cc-eth-Arm® Cortex®-M4 MCU Starter Kit with Ethernet and USB Host](https://www.cypress.com/documentation/development-kitsboards/sk-fm4-176l-s6e2cc-fm4-family-quick-start-guide)**. 

**IDE** choice for the development is **[Keil µVision® IDE 5](http://www2.keil.com/mdk5/uvision/)**.

Since we have all the functionality need already implemented in the FM4, there was no need for any additinal work with hardware.

### ::schemes
The basic process here is the sound processing, so here's the **sound generation and processing** scheme.

![sound generation scheme](https://github.com/sol4ik/audio-steganography/blob/master/pics/sound_generation.jpg)

The sound is generated as the **sine wave**, as we mentioned in the previous section, at the **processing unit**. The output signals with hidden message then are passed to the **DAC** and the analog signal is then played at **the headphones jack**

Other imporant part is also **i/o** of the data.

*(the schemes are yet to be developed)*

---
## latest updates


---
