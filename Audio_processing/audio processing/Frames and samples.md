
### **1. What is a Sample?**

A **Sample** is a single, solitary number. It represents the sound wave at one exact microsecond in time, for **one single speaker (or one ear)**.

If you are looking at your `bits_per_sample` variable (which is usually 16-bit or 24-bit), that is the size of _one_ sample.

### **2. What is a Frame?**

A **Frame** is a bundle of samples that happen at the exact same time across **all speakers**.

Your computer cannot play the left ear and the right ear at different times. It has to feed the data to both speakers simultaneously. Therefore, it bundles the samples together into a "Frame."

### **Real-World Examples**

Let’s look at how this changes depending on how many `audio_channels` your WAV file has.

**Scenario A: Mono Audio (1 Channel)**
- You have one speaker.
- At Time = 0.01s, the computer needs 1 number.
- **1 Frame = 1 Sample.**
- _(This is why the terms get mixed up. In Mono audio, a frame and a sample are the exact same thing!)_

**Scenario B: Stereo Audio (2 Channels)**
- You have a Left speaker and a Right speaker.
- At Time = 0.01s, the computer needs a number for the Left and a number for the Right.
- **1 Frame = 2 Samples** (Left Sample + Right Sample).

**Scenario C: 5.1 Surround Sound (6 Channels)**
- You have 6 speakers in the room.    
- At Time = 0.01s, the computer needs 6 numbers.
- **1Frame = 6 Samples.**

### **The "Sample Rate" Naming Trap**

This is where the math goes wrong for almost every beginner.

Your WAV header has a variable called `sample_rate` (usually 44100 Hz or 48000 Hz). Based on the name, you would think this means "44,100 individual samples per second."

**It does not.** `sample_rate` actually means **Frames per second**.

- If you have a **Stereo** WAV file at 44100 Hz, your computer reads 44,100 _Frames_ every second.
    
- Because it is stereo (2 samples per frame), the computer is actually processing **88,200 individual Samples** every second.


> [!NOTE] Common mistake
> **The Common Mistake (Stuttering/Slow Audio):**  
Audio APIs (like PortAudio) request audio by `frameCount`.  
If you loop `for(i = 0; i < frameCount)` and write one sample per iteration, you only fill the buffer for 1 channel (resulting in a half-empty buffer of zeroes/garbage, causing severe lag and stutter).

**The Golden Rule:**  
When filling the output buffer in a callback, you must loop exactly:  
**`frameCount * channels`**
```c
// Correct Callback Loop for Output Buffer
float* out = (float*)output;
unsigned long total_samples_to_write = framecount * num_channels;

for (unsigned long i = 0; i < total_samples_to_write; i++) {
    out[i] = my_audio_data[current_sample_index++];
}
```
**Key Takeaway:** APIs ask for _Frames_, but arrays hold _Samples_. Always multiply frames by channels when iterating through your 1D audio buffer.

