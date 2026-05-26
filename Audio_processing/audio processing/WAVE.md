### 📝 Note: The RIFF `ChunkSize` Rule

**The Core Concept:**
`ChunkSize` represents the size of the **payload only**. It tells the parser exactly how many bytes come *after* the size variable itself.

**Why subtract 8 instead of 12?**
When calculating the Master RIFF `ChunkSize` from the total file size, you subtract **8 bytes**, not 12. 
* **What is subtracted (8 bytes):** The 4-byte `ChunkID` (e.g., 'RIFF') and the 4-byte `ChunkSize` variable. (A size variable cannot count itself or what came before it).
* **What is kept (The Payload):** The 4-byte Format ID (e.g., 'WAVE') is technically the *first piece of data inside the container*, so its 4 bytes are **included** in the `ChunkSize` count.

**Byte Breakdown (First 12 Bytes):**

| Byte Position | Name                   | Size    | Is it included in the `ChunkSize` count?      |
| :------------ | :--------------------- | :------ | :-------------------------------------------- |
| **0 to 3**    | **ChunkID (`'RIFF'`)** | 4 bytes | ❌ **No.** (Read before the size variable)     |
| **4 to 7**    | **ChunkSize**          | 4 bytes | ❌ **No.** (Cannot count itself)               |
| **8 to 11**   | **Format (`'WAVE'`)**  | 4 bytes | ✅ **Yes!** (First piece of container payload) |

### **WAV Data Types (Choosing the Right Pointer)**

Before doing any math, you must read the data into the correct C data type based on the WAV header's `bits_per_sample`. If you use the wrong one, the audio will sound horribly distorted.

- **8-bit WAV:** Uses **`uint8_t`** (Unsigned). Values range from **0 to 255** (128 is silence).
- **16-bit WAV (Standard):** Uses **`int16_t`** (Signed). Values range from **-32,768 to +32,767** (0 is silence).
- **32-bit WAV:** Uses **`float`**. Values range from **-1.0 to +1.0**.
### **2. How to Normalise 16-bit Audio for PortAudio**

PortAudio (when set to `paFloat32`) requires all audio data to be floating-point numbers between `-1.0` and `+1.0`. Since your 16-bit WAV has massive numbers (up to 32,767), you have to scale them down.

**The Golden Rule:** You do not need to separate positive and negative numbers. You simply divide **every single sample** by `32768.0f`.

- **The Math:** *
 - `-32768 / 32768.0f` = `-1.0`
 - `32767 / 32768.0f` = `0.9999`

- **The C Code:**
```c
   // MUST use .0f to force floating-point math!
   normalized_buffer[i] = (float)raw_wav_data[i] / 32768.0f;
```

_(If you divide by `32768` without the `.0f`, C uses integer division and turns your whole song into zeros!)_


### [Frames and samples](Frames%20and%20samples.md)

This is how audio is stored in memory and how it plays through speakers.

- **A Sample:** A single number for **one speaker** at one exact microsecond.
- **A Frame:** A bundle of samples for **all speakers** at that exact microsecond.    

**How they are arranged in memory (Interleaved):** When you read a Stereo (2-channel) WAV file into your C buffer, the data is just one long, flat list of individual samples alternating left and right:

> `[ L1, R1, L2, R2, L3, R3, L4, R4 ]`

**The Stereo Equation:**

> **1 Frame = (Left 1 + Right 1)**

### **4. Tying it all together for PortAudio**

Because the data in your buffer is just a flat list of alternating samples, **normalizing the samples does not break the frames**. You just process the array from start to finish.

**How the Callback Works:**

1. PortAudio asks for a specific number of **Frames** (e.g., `framesPerCallback = 300`). It is asking for 300 _moments in time_.
    
2. Because you told PortAudio the stream is Stereo (`channels = 2`), it knows it needs **600 Samples** total to fulfill those 300 Frames.
    
3. You loop exactly 600 times, taking the next 600 raw samples, dividing each one by `32768.0f`, and handing them to PortAudio.
    
4. PortAudio automatically grabs them in pairs (L1+R1, L2+R2) and pushes them to your physical speakers.