# Task 6 – Video-Streaming Storage Design

Uploads are write-once; reads are many-to-one and highly skewed to the first seven days. Students seek to arbitrary timestamps. Targets: cached campus first frame under 2 seconds; p99 seek under 500 ms within a region; no loss of acknowledged uploads; peak bitrate capacity with 30% headroom.

Store canonical media on object storage or HDD-backed storage, transcode into segmented HLS/DASH objects, and cache hot segments on SSDs and a CDN. Use XFS/ext4 on local cache nodes, object storage for durable originals/segments, indexed/extents allocation and a separate metadata database. Replicate metadata and keep versioned backups.

Illustrative sizing: 200,000 users, 10% concurrent = 20,000; average bitrate 2 Mbps gives 40 Gbps egress; with 30% headroom plan ~52 Gbps across CDN/edge. 5,000 videos × 1 GB average = 5 TB raw; three copies = 15 TB before transcoded variants. With 4-second segments at 2 Mbps each segment is about 1 MB. Seek to minute m maps to floor(60m/4), typically 1–2 object reads to start playback.

Compare HDD origin + SSD cache + CDN against HDD origin without cache/CDN using the same trace and fio/wrk; record p50/p95/p99, throughput, cache hit ratio and CPU. No physical benchmark is claimed. Cloud object storage + CDN improves geographic delivery but costs depend on GB-month, requests, egress and retrieval. Hybrid origin + CDN is recommended for a small team.