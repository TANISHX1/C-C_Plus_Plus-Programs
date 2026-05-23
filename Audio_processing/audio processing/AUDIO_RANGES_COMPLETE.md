# 🎵 Audio Range & Normalization - Complete Guide

---

## Part 1: Understanding Audio Ranges

### What is a Sample?

A **sample** is a single number representing the volume at one moment in time.

```
Sample: 16543 (a number stored in file)
```

Different formats store samples in different ranges.

---

## Part 2: Storage Ranges (File Format)

### Why Large Ranges?

Audio files use **large integer ranges** to save storage space.

| Bit Depth | Range | Bytes/Sample | Storage |
|-----------|-------|--------------|---------|
| **8-bit** | -128 to +127 | 1 byte | Smallest |
| **16-bit** | -32,768 to +32,767 | 2 bytes | ⭐ Most Common |
| **24-bit** | -8,388,608 to +8,388,607 | 3 bytes | Medium |
| **32-bit** | -2,147,483,648 to +2,147,483,647 | 4 bytes | Largest |

**Example - Storage Size:**
```
1 second of audio at 44,100 Hz, Mono, 16-bit:
  44,100 samples × 2 bytes = 88,200 bytes ≈ 88 KB per second

Same with 32-bit float:
  44,100 samples × 4 bytes = 176,400 bytes ≈ 176 KB per second
  (2x larger!)
```

**That's why files use small integers!** ✓

---

## Part 3: Processing Range (Standard)

### Why -1.0 to +1.0?

All audio **processing and playback** uses the **normalized float range**:

```
Range: -1.0 to +1.0

 +1.0 ← Maximum positive (speaker cone in)
  0.0 ← Silence (no movement)
 -1.0 ← Maximum negative (speaker cone out)
```

### Why This Range?

```
Universal Standard:
  - Works for all bit depths
  - Hardware independent
  - Easy to do math (volume control, filtering)
  - Speaker can interpret directly
```

### Speaker Interpretation:

```
+1.0  → Electrical signal pushes speaker cone IN (max)
+0.5  → Half strength push
 0.0  → No signal, cone rests
-0.5  → Half strength pull
-1.0  → Pulls speaker cone OUT (max)

Rapid changes = Sound waves! 🌊
```

---

## Part 4: The Conversion Pipeline

### Step-by-Step Process:

```
STEP 1: READ FROM FILE (Storage Range)
  16-bit WAV file contains: -16000 (int16_t data type)
  
STEP 2: NORMALIZE (Convert to Processing Range)
  raw_value / max_range
  -16000 / 32767 = -0.488

STEP 3: PROCESS (Processing Range)
  normalized = -0.488 (ready for speaker)
  Apply effects, volume control, etc.
  (stays in -1.0 to +1.0 range)

STEP 4: SEND TO SPEAKER (Still Processing Range)
  PortAudio receives: -0.488
  Speaker plays: -0.488 (pulls cone partially out)

STEP 5: (IF SAVING) DENORMALIZE (Back to Storage Range)
  -0.488 × 32767 = -15,989 (int16_t)
  Save to WAV file
```

### Visual Flow:

```
File (Large Range)
    ↓
Read as int16: -16000
    ↓
Normalize ÷32767: -0.488
    ↓
Process: -0.488
    ↓
PortAudio/Speaker: -0.488
    ↓
(Optional) Denormalize ×32767: -15989
    ↓
File (Large Range)
```

---

## Part 5: Normalization Formulas

### Reading (File → Processing):

```c
// 16-bit WAV file
int16_t raw = (read from file);
float normalized = raw / 32767.0f;
// Range: -1.0 to +1.0 ✓
```

### Writing (Processing → File):

```c
// Processing range to 16-bit
float sample = 0.488;  // (-1.0 to +1.0)
int16_t encoded = (int16_t)(sample * 32767);
// Range: -32767 to +32767 ✓
```

### Different Bit Depths:

```c
// 8-bit
int8_t raw_8 = (read);
float norm_8 = raw_8 / 127.0f;

// 16-bit
int16_t raw_16 = (read);
float norm_16 = raw_16 / 32767.0f;

// 24-bit
int32_t raw_24 = (read);
float norm_24 = raw_24 / 8388607.0f;

// 32-bit
int32_t raw_32 = (read);
float norm_32 = raw_32 / 2147483647.0f;
```

---

## Part 6: PortAudio Specific

### PortAudio Always Uses -1.0 to +1.0:

```c
// No matter what format you use:
PaStreamParameters params;
params.sampleFormat = paFloat32;  // -1.0 to +1.0
// OR
params.sampleFormat = paInt16;     // PortAudio converts internally
```

### In Callback:

```c
// With paFloat32 (what we use)
float *out = (float *)output;
out[i] = 0.5f;  // Valid: -1.0 to +1.0 range ✓

// Output must ALWAYS be in -1.0 to +1.0
out[i] = sinf(...);       // OK ✓
out[i] = 5.0f;            // ERROR: Outside range ❌
```

---

## Part 7: Industry Standard

### All Audio Software Uses -1.0 to +1.0:

```
PortAudio     → -1.0 to +1.0 ✓
FFMPEG        → -1.0 to +1.0 ✓
Audacity      → -1.0 to +1.0 ✓
Python/scipy  → -1.0 to +1.0 ✓
WebAudio API  → -1.0 to +1.0 ✓
JUCE Library  → -1.0 to +1.0 ✓
```

**This is the UNIVERSAL STANDARD for audio processing!**

---

## Part 8: Clipping (What NOT to Do)

### Clipping Occurs:

```
If you output: out[i] = 2.0 (exceeds +1.0)
PortAudio clips to: +1.0
Result: Distorted, harsh sound ❌

If you output: out[i] = sinf(...) × 100 (way too loud)
Clipped to: +1.0 repeatedly
Result: Loud distortion ❌
```

### Prevention:

```c
// WRONG ❌
out[i] = sinf(2.0f * M_PI * phase) * 100;

// CORRECT ✓
out[i] = sinf(2.0f * M_PI * phase) * 0.5;  // 50% volume
out[i] = sinf(2.0f * M_PI * phase);         // Full volume (0 dB)
```

---

## Part 9: Practical Examples

### Example 1: Read WAV and Play

```c
// Reading 16-bit WAV
int16_t sample_from_file = -16000;
float for_playback = sample_from_file / 32767.0f;  // = -0.488

// Send to speaker
out[i] = for_playback;  // Correct ✓
```

### Example 2: Generate Tone

```c
// Generate sine wave (already in -1.0 to +1.0)
out[i] = sinf(2.0f * M_PI * phase);

// Generate quieter sine wave (reduce volume)
out[i] = sinf(2.0f * M_PI * phase) * 0.3;  // 30% volume

// Generate loud sine wave
out[i] = sinf(2.0f * M_PI * phase) * 0.9;  // 90% volume
```

### Example 3: Volume Control

```c
// Original sample
float original = 0.5f;  // (-1.0 to +1.0)

// Apply gain (0.0 = silence, 1.0 = normal, 2.0 = loud)
float gain = 0.5f;  // Reduce volume by half
float with_gain = original * gain;  // = 0.25

// Clamp to prevent clipping
if (with_gain > 1.0f) with_gain = 1.0f;
if (with_gain < -1.0f) with_gain = -1.0f;

out[i] = with_gain;  // Safe ✓
```

---

## Part 10: Summary Table

| Aspect | Storage | Processing | Conversion |
|--------|---------|-----------|------------|
| **Bit Depth** | 8/16/24/32 bit | 32-bit float | Formula |
| **Range** | Large integers | -1.0 to +1.0 | divide by max |
| **Size** | 1-4 bytes | 4 bytes | Runtime |
| **Where** | File on disk | RAM/Processing | When reading/writing |
| **Standard** | Various | Universal | -1.0 to +1.0 |

---

## Part 11: Quick Reference

### Ranges by Format:

```
8-bit:  Divide by 127,         Multiply by 127
16-bit: Divide by 32767,       Multiply by 32767  ⭐ Common
24-bit: Divide by 8388607,     Multiply by 8388607
32-bit: Divide by 2147483647,  Multiply by 2147483647
```

### Processing Rules:

```
✓ Always output -1.0 to +1.0 from callback
✓ Use -1.0 to +1.0 for all processing
✓ Normalize when reading files
✓ Denormalize when writing files
✓ Clamp to prevent clipping
```

### The Golden Rule:

```
Everything in PROCESSING = -1.0 to +1.0 ✓
Everything in FILES = Large integer range ✓
Convert between them using division/multiplication ✓
```

---

## Part 12: Key Takeaways

1. **Files use large ranges** to save storage space
2. **Processing uses -1.0 to +1.0** (universal standard)
3. **Conversion is simple math**: divide by max when reading, multiply by max when writing
4. **PortAudio always expects -1.0 to +1.0** in callback
5. **All audio software** (Audacity, FFMPEG, etc.) uses -1.0 to +1.0 for processing
6. **Clipping occurs** if you exceed the range
7. **Prevention**: Always keep output in -1.0 to +1.0 range

---

