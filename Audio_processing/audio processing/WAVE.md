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