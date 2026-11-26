# Task 5 – Distributed and Cloud-Based Storage

| Criterion | NFS | HDFS | CephFS/Ceph | Cloud object storage |
|---|---|---|---|---|
| Access | Remote POSIX-like filesystem | Distributed blocks, large sequential data | Distributed POSIX/object/block | HTTP object API |
| Consistency | Server-mediated close-to-open semantics; options matter | Metadata and replicated block pipeline | Strong consistency in normal Ceph operations | Strong read-after-write in major providers |
| Availability | Server/network/HA dependent | Replication and NameNode HA | Replication/erasure coding and quorum | Provider-managed multi-device durability |
| Performance | Low LAN latency; central bottleneck | High throughput; less suited to tiny random files | Scales across nodes with network/recovery overhead | Good for large objects; CDN improves global delivery |
| Cost | Hardware/admin | Multiple nodes and expertise | More nodes/network and expertise | Storage, requests, retrieval and transfer |

Recommend managed object storage for student documents with SSO/MFA, lifecycle rules and CDN for public course material. NFS suits legacy POSIX applications on a controlled LAN. Ceph suits private cloud when skilled operators are available; HDFS is mainly for analytics.

Three replicas can preserve availability after failure if placement/quorum permits; place copies across hosts/racks/power domains. Replication improves availability/read throughput but uses about 3× raw capacity. It is not a backup: deletion/ransomware can replicate, so retain immutable/versioned backups.

Strong consistency simplifies official grades and exam submissions but may require coordination. Eventual consistency allows temporarily stale reads and requires retries/version checks/conflict handling; it may be acceptable for public media caches.

Device management: drivers translate kernel I/O to device commands and handle interrupts/DMA; buffering absorbs speed differences; caching retains hot blocks; spooling queues serialized work; block-device interface exposes addressable fixed-size blocks to filesystems/page cache.

One large NFS server is simple at low concurrency but becomes a CPU/RAM/network/disk bottleneck and a single failure domain. Three-replica distributed storage uses more capacity and coordination but can serve reads across nodes and survive node failure when quorum permits.