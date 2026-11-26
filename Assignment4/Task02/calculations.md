# Task 2 – Allocation Calculations

32 camera streams append footage continuously. Reads are mainly sequential, with occasional direct/time-based seeks. Files are deleted after 30 days.

| Method | Sequential | Direct access | Growth | Fragmentation/reliability |
|---|---|---|---|---|
| Contiguous | Excellent | Excellent | Difficult without adjacent space | External fragmentation |
| Linked | Pointer traversal | O(k) traversal | Easy append | No external fragmentation; broken pointer loses chain |
| FAT | FAT-chain traversal | O(k) lookups unless cached | Easy append | Central FAT corruption can affect many files |
| Indexed | Excellent | Index lookup | Easy until index capacity | Index-block overhead and metadata failure point |

4 GiB / 4 KiB = 1,048,576 blocks. Contiguous allocation can calculate the last block location and seek directly (idealized one positioning operation). Linked allocation may traverse 1,048,576 links; FAT may perform the same number of table lookups, though cached FAT entries are RAM hits. Indexed allocation needs one index read (or a few for multi-level) plus the data read if the index can address the block.

An indirect block holds 4096/4 = 1024 addresses. Total inode data blocks = 12 + 1024 + 1024² + 1024³ = 1,074,791,436. Maximum = 4,402,345,721,856 bytes (about 4.004 TiB).

For a 4 TB decimal disk and 4 KiB blocks: 976,562,500 blocks; bitmap size = 976,562,500/8 = 122,070,312.5 bytes ≈ 116.4 MiB. A 4 TiB disk requires 128 MiB.

**Important inconsistency in the supplied question:** after releasing B and D, there are 21 free blocks in two runs: blocks 6–13 (8 blocks) and 19–31 (13 blocks). Therefore, a first-fit contiguous allocation of 10 blocks **does succeed** at block 19. This matches the assignment's own expected output. The statement asking why E cannot be stored contiguously is inconsistent with the given allocation sequence and expected output. If the intended experiment is to demonstrate external fragmentation, change the file sizes/placement so every free run is shorter than 10 while total free space remains at least 10.

The given program's indexed allocation then uses the remaining free blocks: index block 6 and data blocks 7–13, 29–31. Indexed allocation can handle scattered blocks, but in this exact run it is not needed to make the first E allocation succeed.

FAT alternative: O(k) chain/table lookups to block k; a 4-byte FAT entry per disk block requires about 3.64 GiB for this 4 TB disk. Indexed access is O(1) for a single-level index or O(levels) for multi-level indexing.