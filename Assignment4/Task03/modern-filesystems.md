# Task 3 – Modern File Systems

| Filesystem | Size notes | Journaling/consistency | Snapshots | Checksums | CoW |
|---|---|---|---|---|---|
| FAT32 | 4 GiB − 1 byte file limit | No journal | No | No end-to-end data checksums | No |
| NTFS | Very large; implementation limits apply | Metadata journal | VSS at Windows volume layer | Metadata integrity; optional data integrity streams | Not general CoW |
| ext4 | Common 4 KiB config: 16 TiB file, 1 EiB volume (feature-dependent) | Journal | No native snapshots | Metadata checksums | No |
| XFS | Multi-petabyte scale on modern systems | Metadata journal | Usually via LVM | Metadata checksums | No general CoW |
| Btrfs | Very large theoretical space; practical limits lower | Transactional CoW | Native subvolume snapshots | Data and metadata checksums | Yes |
| ZFS | 128-bit architecture; practical limits very high | Transactional CoW + intent log | Native snapshots/clones | End-to-end checksums | Yes |
| APFS | Large 64-bit-scale limits | CoW metadata | Native snapshots | Metadata checksums | Yes |

Limits depend on OS version, features, block size and implementation.

A crash during write can leave data and metadata inconsistent. Journaling records intended metadata updates before commit and replays/discards incomplete transactions. It does not guarantee all application data reached disk; use fsync/fdatasync. ext4 data=journal journals data and metadata (highest overhead); ordered writes data before related metadata (default balanced mode); writeback may commit metadata before data (weaker crash ordering). CoW writes new blocks and atomically switches metadata roots; snapshots preserve old references and checksums detect corruption, but small random writes can be amplified.

Choices: exFAT for Windows/Linux USB drives; ext4 or XFS for Linux database (benchmark sync I/O); native APFS/NTFS/ext4 for laptop OS; ZFS or Btrfs for snapshot backup server.

Run on Linux, replacing the device carefully:
```sh
stat -f -c 'Type=%T BlockSize=%S Blocks=%b Free=%f' /
df -i /
ls -i . | head
tune2fs -l /dev/sdXN | grep -E 'Block size|Inode count|Free inodes|Free blocks'
```
Do not run tune2fs on a mounted or incorrectly identified device.

Btrfs/ZFS alternative: CoW random updates may write new data/metadata blocks and increase latency; snapshots initially share blocks but grow as blocks diverge. Recovery is transactional and checksums detect latent corruption. Benchmark synchronous writes, record size, compression and cache settings against ext4/XFS.