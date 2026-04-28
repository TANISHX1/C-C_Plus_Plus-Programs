# 🔊 C/C++ Audio Processing Project

This repository contains tools and examples for processing audio in C/C++. It explores how digital audio works under the hood and provides practical examples of manipulating audio data.

## What is Audio Processing?

Digital audio processing involves manipulating sound data programmatically. Sound is converted from an analog continuous wave into digital format using **Pulse Code Modulation (PCM)**:

* **Sampling**: Taking snapshots of the sound wave at regular intervals (e.g., 44,100 times a second for standard CD-quality audio).
* **Quantization**: Assigning a numerical value to the volume of each snapshot (e.g., 16-bit audio provides 65,536 possible volume levels).

To allow computers to understand this raw PCM data, it is typically wrapped in a container like a **WAV file**, which contains a 44-byte header providing essential instructions (such as sample rate, number of channels, and bit depth) followed by the raw audio samples.

---

## Project Structure & Programs

The project is divided into two main areas: basic file parsing/modification and hardware interfacing using PortAudio.

### 📁 `basic/` (File Parsing & Audio Modification)
Programs dealing with parsing WAV headers and manipulating raw audio data on disk.

* **[wav_Reader.c](./basic/wav_Reader.c)**: 
  * Parses and validates WAV file headers. 
  * Reads the RIFF container, format chunks, and data chunks to extract metadata (channels, bit depth, sample rate).
* **[vol_control.c](./basic/vol_control.c)**: 
  * A utility that modifies the volume (gain) of a WAV file.
  * Directly manipulates raw PCM sample values.
  * Handles 8-bit, 16-bit mono, and 16-bit stereo formats using precise bitwise clamping to prevent clipping.
* **[decor.h](./decor.h)**: 
  * A utility header providing ANSI escape codes for rich, colored terminal output styling.
* **[vol_control_analysis.md](./basic/vol_control_analysis.md)**: 
  * Detailed documentation explaining the technical logic behind the volume control program (data types, memory management, and channel handling).

### 📁 `port_audio/` (Hardware Interfacing)
Programs dealing with real-time audio playback and hardware communication using the PortAudio library.

* **[audio_devices.c](./port_audio/audio_devices.c)**: 
  * Initializes the PortAudio API.
  * Enumerates and lists all connected audio input/output devices along with their supported channels and latencies.
* **[silence.c](./port_audio/silence.c)**: 
  * Implements a real-time PortAudio playback stream.
  * Uses an audio callback function to push raw PCM data (in this case, zeros for absolute silence) directly to the sound card's hardware buffer.

---

### 💡 Pro-Tip
If you ever encounter "static" or "white noise" while programming stereo audio, check your **Block Align**. If you accidentally read an odd number of bytes in a stereo file, you will "de-sync" the left and right ears, resulting in immediate audio corruption.

---

> *Applications developed by Tanish Shivhare*