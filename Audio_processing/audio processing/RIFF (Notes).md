## What is RIFF?

- **Container Format:** RIFF is a **file container architecture**, not a specific file type. It defines _how_ data should be organized inside a file, not what the data actually contains.
- **Chunk-Based Structure:** It stores information in discrete, self-contained blocks called **"chunks."**
- **FourCC Identifiers:** Each chunk is identified by a 4-character code (FourCC) like `RIFF`, `WAVE`, `fmt` , or `data`.
- **Family of Formats:** It serves as the foundation for several major file types:
    - **WAV** (audio)
    - **AVI** (video)
    - **RMI** (MIDI music)
    - **PAL** (color palettes)
## Why Was RIFF Created ?

- **Standardization Need:** Before RIFF, every software developer stored multimedia data differently. This made files incompatible across different programs and operating systems.

- **Inspired by IFF:** Microsoft and IBM adapted the **IFF (Interchange File Format)** originally developed for Commodore Amiga computers in the 1980s. They modified it to use little-endian byte order (matching Intel x86 processors) instead of big-endian.

- **Multimedia Expansion:** Created in **1991**, it was designed specifically to support the emerging multimedia capabilities of Windows 3.1 and OS/2.

- **Extensibility Goal:** The creators wanted a format that could evolve. New types of data could be added without breaking old software that didn't recognize those new types.

## Why is RIFF Used? (Key Advantages)

- **Platform Independence:** While optimized for little-endian systems, the structure is clear enough to be read on any platform (Windows, macOS, Linux, embedded devices).

- **Simple Parsing:** Programs can read a RIFF file sequentially without complex parsing logic. If a program encounters a chunk type it doesn't understand, it can simply skip it using the size field.

- **Nested Structure:** RIFF supports **LIST chunks**, allowing hierarchical organization of data (e.g., grouping multiple audio streams with their metadata).

- **Backward Compatibility:** New chunk types can be added to existing formats (like WAV) without breaking older software. The old software ignores unknown chunks; new software uses them.

- **Direct Memory Mapping:** The chunk structure aligns well with how C structures work, making it efficient for low-level programming.

## What "Self-Describing" Actually Means

"Self-describing" refers to **every individual chunk** in the file, not just the main RIFF header. Each chunk carries its own "ID card":

>  [Chunk ID: 4 bytes] [Size: 4 bytes] [Data: X bytes]
>      ↑                         ↑
>    "What I am"        "How big I am"

### Example in a WAV file:

|Chunk ID|Meaning|Software Action|
|---|---|---|
|`fmt`|Format info (sample rate, etc.)|Audio player **must** read this|
|`data`|Raw audio samples|Audio player **must** read this|
|`LIST`|Metadata (artist, title)|Player **can** skip if it only wants to play sound|
|`bext`|Broadcast extension (professional metadata)|Basic players **skip**; professional tools **use**|
 
> **Question :** like in that example hexdump output (Read the[[How to Read the Hex Dump(Little-Endian)]] to learn about hex dump  )

| Offset   | Hex Values                                         | ASCII            |
| :------- | :------------------------------------------------- | :--------------- |
| 00000000 | 52 49 46 46 46 a0 fe 01    57 41 56 45 66 6d 74 20 | RIFFF...WAVEfmt  |
| 00000010 | 10 00 00 00 01 00 02 00   44 ac 00 00 10 b1 02 00  | ........D....... |
| 00000020 | 04 00 10 00 4c 49 53 54   1a 00 00 00 49 4e 46 4f  | ....LIST....INFO |
| 00000030 | 49 53 46 54 0e 00 00 00   4c 61 76 66 36 30 2e 31  | ISFT....Lavf60.1 |
| 00000040 | 36 2e 31 30 30 00 64 61   74 61 00 a0 fe 01 00 00  | 6.100.data...... |
| 00000050 | 00 00 00 00 00 00 00 00   00 00 00 00 00 00 00 00  | ................ |
| 00000060 | 00 00 00 00                                        | ....             |


>**Answer :**
>
> 52 49 46 46 = "RIFF"  ← Main container says "I'm a RIFF file"
> 46 a0 fe 01 = 0x01fea046 = 33,425,478 bytes (file size - 8)
> 57 41 56 45 = "WAVE"  ← "I contain WAVE audio"
> 
> 66 6d 74 20 = "fmt "  ← Subchunk: "I'm format info"
> 10 00 00 00 = 16 bytes ← "My data is 16 bytes long"
> [16 bytes of format data follow...]
> 
> 4c 49 53 54 = "LIST"  ← Next subchunk: "I'm a list"
> 1a 00 00 00 = 26 bytes ← "My data is 26 bytes long"
> 49 4e 46 4f = "INFO"  ← Inside LIST: "I contain INFO"
> ...and so on

>## Your "Sub-Container" Insight
>
>You asked: **"Is each chunk like a sub-container with its own size and data?"**

**Exactly!** And here's the cool part—some chunks are **pure data** (like `fmt` , `data`) and some are **nested containers** (like `RIFF`, `LIST`):

> RIFF (container)
> ├── fmt  (data chunk: 16 bytes of format specs)
> ├── LIST (container chunk!)
> │   └── INFO (container!)
> │       └── ISFT (data chunk: "Lavf60.16.100")
> └── data (data chunk: actual audio samples)

## [[WAVE]] (RIFF) Chunk Specification Table

All multi-byte values are **little-endian**. Each chunk consists of a 4-byte ID, a 4-byte Size field, and the Data section.


> [!NOTE] Title
>see the Visualisation of [[WAVE FORMAT VISUALIZATION.html]]file (how containerisation works )


| Chunk ID   | Size (Bytes)    | Data Section Structure                                                                                                                                                                                                                                                                                                                                                 |
| ---------- | --------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`RIFF`** | File Size - 8   | * 4-byte form type **"WAVE"**<br><br>  <br><br>* Followed by all subchunks (ID, Size, Data)                                                                                                                                                                                                                                                                            |
| **`fmt`**  | 16, 18, or 40   | **WAVEFORMATEX structure:**<br><br>* `wFormatTag` (2): Format code<br><br>* `nChannels` (2): Number of channels<br><br>* `nSamplesPerSec` (4): Sample rate<br><br>* `nAvgBytesPerSec` (4): Data rate<br><br>* `nBlockAlign` (2): Data block size (bytes)<br><br>* `wBitsPerSample` (2): Bits per sample<br><br>* `cbSize` (2): Size of extra parameters (if Size > 16) |
| **`data`** | Variable        | * **Raw interleaved audio samples**<br><br>* PCM: Integers<br><br>* Non-PCM: Format-specific bitstream                                                                                                                                                                                                                                                                 |
| **`LIST`** | Variable        | * 4-byte list type (e.g., **"INFO"** or **"adtl"**)<br><br>* Followed by subchunks (ID, Size, Data)<br><br>* INFO tags: `INAM` (Name), `IART` (Artist), `ICMT` (Comment)                                                                                                                                                                                               |
| **`fact`** | Typically 4     | * `dwSampleLength` (4): Number of samples (required for compressed/non-PCM formats)                                                                                                                                                                                                                                                                                    |
| **`cue`**  | 4 + (24 × _n_)  | * `dwCues` (4): Number of cue points<br><br>* **CuePoint Array** (24 bytes each): `dwName`, `dwPosition`, `fccChunk`, `dwChunkStart`, `dwBlockStart`, `dwSampleOffset`                                                                                                                                                                                                 |
| **`smpl`** | 36 + (24 × _n_) | * Manufacturer (4), Product (4), Sample Period (4)<br><br>* MIDI Unity Note (4), MIDI Pitch Fraction (4)<br><br>* SMPTE Format (4), SMPTE Offset (4)<br><br>* `cSampleLoops` (4): Number of loops<br><br>* `cbSamplerData` (4): Size of extra data                                                                                                                     |
| **`plst`** | 4 + (12 × _n_)  | * `dwPoints` (4): Number of playlist segments<br><br>* **Playlist Entry** (12 bytes each): `dwName`, `dwLength`, `dwLoops`                                                                                                                                                                                                                                             |
| **`inst`** | 7               | * `Unigram` (1), `MIDINote` (1), `FineTune` (1)<br><br>* `Volume` (1), `LowNote` (1), `HighNote` (1)<br><br>* `LowVelocity` (1), `HighVelocity` (1)                                                                                                                                                                                                                    |

### Implementation Essentials

- **Word Alignment:** Every chunk must start on an even byte boundary. If a chunk's data size is odd, a single **null padding byte (0x00)** must be added after the data. This pad byte is **not** included in the Chunk Size field.
    
- **The "fmt " Space:** Note that the `fmt` and `cue` IDs are exactly four characters long; they include a trailing space (`0x20`) to meet the 4-byte requirement.
    
- **Mandatory Chunks:** Every valid WAV file must contain the `RIFF` container, the `fmt` chunk, and the `data` chunk.
    
- **Ordering:** The `fmt` chunk **must** appear before the `data` chunk. While other chunks are flexible, `fact` (if present) usually follows `fmt` , and metadata (`LIST`, `cue` ) often appears after `data`.
    

### Calculation Example

For a standard CD-quality PCM file (16-bit, Stereo, 44.1 kHz), the `nBlockAlign` is calculated as:

nBlockAlign=8nChannels×wBitsPerSample​=82×16​=4 bytes

And the data rate:

nAvgBytesPerSec=nSamplesPerSec×nBlockAlign=44100×4=176,400 bytes/s


