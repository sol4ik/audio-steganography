# audio-steganography
Course project for Principles of Computer Organisation by **[Solomiia Leno](https://github.com/sol4ik)** and **[Ihor Titov](https://github.com/lurak)** on **steganographic tranfer of data by means of audio signals**.

---
## requirements
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

## algoritms
As our project consists of two processes - coding and decoding the message - we've been developing two different algorithms.

### coding
For message hiding we've researched different approaches as echo hiding, lower tones insertion and so on but we decided to implement the basic one - **amplitude modulation**.

## model

---
## hardware
For the hardware part of the project we have **[fm4-176l-s6e2cc-eth-Arm® Cortex®-M4 MCU Starter Kit with Ethernet and USB Host](https://www.cypress.com/documentation/development-kitsboards/sk-fm4-176l-s6e2cc-fm4-family-quick-start-guide)**. 

The **IDE** for development is **[Keil µVision® IDE 5](http://www2.keil.com/mdk5/uvision/)**.

As we have all the functionality need already implemented in the FM4, there was no need for any additinal work with hardware.


The basic process here is the sound processing, so here's the **sound generation and processing** scheme.
![sound generation scheme](https://github.com/sol4ik/audio-steganography/blob/master/pics/sound_generation.jpg)
The sound is generated as the **sine wave**, as we mentioned in the previous section, at the **processing unit**. The output signals with hidden message then are passed to the **DAC** and the analog signal is then played at **the headphones jack**

Other imporant part is also **i/o** of the data.

*(the schemes are yet to be developed)*

---
## latest updates


---
