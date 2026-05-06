
## What is a Hex Dump?

A hex dump shows **raw binary data** as readable hexadecimal values, typically with:

- **Offset/Address**: Where in the file this data lives
- **Hex bytes**: The actual data (16 bytes per row usually)
- **ASCII**: Printable character representation

### Reading Offsets
<div style="display: flex; align-items: flex-start; gap: 25px;">
  <!-- Left Side: The Heading -->
  <div style="padding-top: 10px;">
    <h4 style="margin: 0; white-space: nowrap;"><br><br><br>Converting B/w Offsets</h4>
  </div>

  <!-- Right Side: The Table -->
  <table border="1" style="border-collapse: collapse; width: 100%;">
    <thead>
      <tr >
        <th style="padding: 8px; text-align: left;">Offset Notation</th>
        <th style="padding: 8px; text-align: left;">Format</th>
        <th style="padding: 8px; text-align: left;">Example</th>
        <th style="padding: 8px; text-align: left;">Meaning</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td style="padding: 8px;">Hex</td>
        <td style="padding: 8px;"><code>0x00000010</code></td>
        <td style="padding: 8px;">16 bytes from start</td>
        <td style="padding: 8px;">16 bytes from start</td>
      </tr>
      <tr>
        <td style="padding: 8px;">Hex shorthand</td>
        <td style="padding: 8px;"><code>0x10</code></td>
        <td style="padding: 8px;">Same as above</td>
        <td style="padding: 8px;">Same as above</td>
      </tr>
      <tr>
        <td style="padding: 8px;">Decimal</td>
        <td style="padding: 8px;"><code>16</code></td>
        <td style="padding: 8px;">16 bytes from start</td>
        <td style="padding: 8px;">16 bytes from start</td>
      </tr>
    </tbody>
  </table>
</div>



<div style="display: flex; align-items: flex-start;">
  <div style="padding-right: 20px;">
    <h4>Converting B/w Offsets</h4>
  </div>
  
  <table border="1">
    <thead>
      <tr>
        <th>Hex → Decimal</th>
        <th>Decimal → Hex</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>0x0C = 12</td>
        <td>512 = 0x200</td>
      </tr>
      <tr>
        <td>0xFF = 255</td>
        <td>1024 = 0x400</td>
      </tr>
      <tr>
        <td>0x100 = 256</td>
        <td>44100 = 0xAC44</td>
      </tr>
      <tr>
        <td>0x1000 = 4096</td>
        <td>10 = 0xA</td>
      </tr>
    </tbody>
  </table>
</div>

### File Position Calculation

If you need to **jump to a specific offset**:

> Physical position in file = offset value (in bytes)
> 
> Example: To read at offset 0x24:
> - Skip first 0x24 (36) bytes
> - Start reading from byte 37

## Analyzing Structure: Step-by-Step Method

### Step 1: Identify the Base Address

Most hex dumps show **relative offsets** from file start (0x00000000).

> 00000000:  <- Base = 0x00, file start
> 00000010:  <- 16 bytes from start (0x10)
> 00000020:  <- 32 bytes from start (0x20)

### Step 2: Calculate Field Positions

Given a structure at offset `0x0C`:

> Structure starts at: 0x0C (12 decimal)
> 
> Field layout (example: WAV fmt chunk):
> +-----------+--------+--------------------------------+
> | Offset  | Size  | Field                               |
> +-----------+--------+--------------------------------+
> | 0x0C    | 2      | wFormatTag (audio format)      |
> | 0x0E    | 2      | nChannels                      |
> | 0x10    | 4      | nSamplesPerSec                 |
> | 0x14    | 4      | nAvgBytesPerSec                |
> | 0x18    | 2      | nBlockAlign                    |
> | 0x1A    | 2      | wBitsPerSample                 |
> +--------+--------+-------------------------------------+

### Step 3: Extract Values from Hex Dump

**Example analysis:**

> | Offset    |  0  1  2   3   4   5   6   7      8   9   A   B   C   D   E   F   |
> 00000000: 52 49 46 46 24 08 00 00   57 41 56 45 66 6D 74 20
> 00000010: 10 00 00 00 01 00 01 00   44 AC 00 00 88 58 01 00

**Task: Read `nSamplesPerSec` at offset 0x10**

> 1. Find row containing 0x10: Row starting at 00000010
> 2. Calculate position in row: 0x10 - 0x10 = 0 (first byte of row)
> 3. Read 4 bytes: 44 AC 00 00
> 4. Apply little-endian: 00 00 AC 44 → 0x0000AC4

%%
## Practical Decoding Examples

### Example 1: Decode a 32-bit Integer

> Raw bytes at offset 0x08:  24 08 00 00
> 
> Little-endian decode:
>   Byte 0: 0x24 = 36
>   Byte 1: 0x08 = 8
>   Byte 2: 0x00 = 0
>   Byte 3: 0x00 = 0
> 
>   Value = 36 + (8 × 256) + 0 + 0
>         = 36 + 2048
>         = 2084 (0x0824)
> 
> Wait! Actually: 0x00 00 08 24 = 0x00000824 = 2084

### Example 2: Decode a 16-bit Integer

> Raw bytes:  01 00
> 
> Little-endian:  0x0001 = 1
> Big-endian:     0x0100 = 256

### Example 3: Decode ASCII/String

> Raw bytes:  57 41 56 45
> 
> Convert each:  0x57='W'  0x41='A'  0x56='V'  0x45='E'
> Result: "WAVE"

%%

 
### Little-Endian Rule (RIFF/WAV uses this!)

**The first byte is the LEAST significant:**

> Value = Byte0 + (Byte1 × 256) + (Byte2 × 65536) + (Byte3 × 16777216)
> 
> 0c 00 00 00 = 0x0C + 0 + 0 + 0 = 0x0000000C = 12 (decimal)
> (the '0x0000000C' is representation on  32 bit system with little endian )

**NOT** `0x000000C0` or `0xC0000000`! Those would be wrong.

### Visual Example

|Hex Bytes|Little-Endian Value|Big-Endian Value|
|---|---|---|
|`0c 00 00 00`|`0x0000000C` = **12**|`0x0C000000` = 201326592|
|`44 ac 00 00`|`0x0000AC44` = **44100**|`0x44AC0000` = 1150783488|
|`10 00`|`0x0010` = **16**|`0x1000` = 4096|

**RIFF/WAV is always little-endian** (Intel/x86 format).

---

## 2. Where to Get Official Chunk Format Information

Here are the **authoritative sources**:

|Source|What You Get|Link/Location|
|---|---|---|
|**Microsoft Official Spec**|Original RIFF/WAVE specification|Search: "Multimedia Programming Interface and Data Specifications 1.0"|
|**Microsoft Learn**|Modern documentation|[learn.microsoft.com - WAVEFORMATEX](https://learn.microsoft.com/en-us/windows/win32/api/mmreg/ns-mmreg-waveformatex)|
|**Windows SDK Headers**|C structure definitions|`mmreg.h` and `mmsystem.h` in Windows SDK|
|**RFC 2361**|WAVE format registration|IETF official standard|
|**Library Source Code**|Working implementations|[libsndfile](https://github.com/libsndfile/libsndfile), [FFmpeg](https://ffmpeg.org/), [WavPack](https://www.wavpack.com/)|

### Key Documents to Search For:

> "Resource Interchange File Format (RIFF)"
> "RIFF File Format Reference"
> "WAVE file format specification"
> "mmreg.h wave format structures"

### Quick Reference: Common Chunk Structures

|Chunk|Spec Source|Key Structure|
|---|---|---|
|`RIFF`|Microsoft RIFF spec|`RIFFChunk { char id[4]; uint32_t size; char format[4]; }`|
|`fmt`|`mmreg.h`|`WAVEFORMATEX` or `PCMWAVEFORMAT`|
|`data`|RIFF spec|Raw bytes, no header|
|`fact`|RIFF spec|`struct { uint32_t dwSampleLength; }`|
|`cue`|RIFF spec|`CueChunk` with point lis|
