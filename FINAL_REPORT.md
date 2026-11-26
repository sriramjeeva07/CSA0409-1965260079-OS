# OPERATING SYSTEMS – ASSIGNMENTS 4 & 5
## CO4: Optimising File Storage and Disk Access
## CO5: Linux, Containers and Virtualisation for Efficient Operations

**Student Name:** S. Jeevan  
**Register Number:** 1965260079  
**GitHub Repository:** https://github.com/sriramjeeva07/CSA0409-1965260079-OS  
**Compiler used for C programs:** GCC 14.2.0  
**Test environment:** Debian GNU/Linux 13  
**Scope:** This report covers Assignments 4 and 5 only. Existing Assignments 1, 2 and 3 are preserved.

---

# ASSIGNMENT 4 – CO4

## Task 1 – File-System Design Review

### 1.1 Weaknesses in the proposed design
1. A single flat directory causes filename collisions because students may choose identical names.
2. Searching one namespace containing 12,000 students' files increases lookup/listing overhead and makes administration difficult.
3. Contiguous allocation suffers external fragmentation. A growing file may not have adjacent free blocks and may need relocation.
4. No access control exposes coursework and personal data to other students.
5. One directory complicates quotas, retention, backup selection and per-student recovery.
6. Group-project sharing lacks a controlled namespace and least-privilege permissions.

### 1.2 Improved design
Use a hierarchical namespace: `/srv/students/<register-number>/{home,coursework,uploads}` and `/srv/projects/<course>/<project-id>/`. Use register numbers as stable identifiers, safe filename rules and path-traversal protection. Store owner, group, size, timestamps, type, checksum, ACL and quota metadata. Student folders use owner-only permissions (0700); staff access uses course groups/ACLs; project folders use dedicated groups and 2770 setgid directories. Link shared folders into workspaces while retaining target permissions. Use extents/indexed allocation, snapshots, encrypted backups and tested restores.

### 1.3 Design defence
Each student owns a private directory protected by owner-only permissions and server-side ACL checks. Faculty receive access through course groups, while project members receive access only to their project's shared directory. Separate project groups and setgid directories preserve group ownership for new files. Audit logs record access and changes. This provides private-by-default storage without preventing deliberate collaboration, and backups/snapshots support recovery without granting students access to one another's data.

### Cloud-drive alternative
A managed drive offers indexed metadata search, synchronization and access from campus/home. Search/open time depends on provider indexing, network latency, file size and local cache. Versions, recycle bins and provider replicas may consume additional storage. The college retains policy control through SSO, MFA, retention, DLP, audit logs and sharing restrictions, but delegates physical infrastructure and some operational control to the provider.

## Task 2 – File Allocation and Free-Space Management

32 camera streams append footage continuously. Reads are mainly sequential, with occasional time-based direct seeks. Files are deleted after 30 days.

| Method | Sequential access | Direct access | File growth | Fragmentation / reliability |
|---|---|---|---|---|
| Contiguous | Excellent | Excellent | Difficult without adjacent space | External fragmentation |
| Linked | Pointer traversal | O(k) traversal | Easy append | No external fragmentation; broken pointer loses chain |
| FAT | FAT-chain traversal | O(k) lookups unless cached | Easy append | Central FAT corruption can affect many files |
| Indexed | Excellent | Index lookup | Easy until index capacity | Index-block overhead and metadata failure point |

For a 4 GiB file with 4 KiB blocks: 4,294,967,296 / 4,096 = **1,048,576 blocks**. In an idealized model, contiguous allocation calculates the final block address directly. Linked allocation may traverse 1,048,576 links. FAT may perform the same number of table lookups (often RAM hits when cached). Indexed allocation requires an index read (or a few for multilevel indexing) plus the data-block read.

An indirect block holds 4096/4 = 1024 addresses. An inode with 12 direct, one single-, one double- and one triple-indirect pointer addresses:
12 + 1024 + 1024² + 1024³ = **1,074,791,436 data blocks**. Maximum bytes = 1,074,791,436 × 4096 = **4,402,345,721,856 bytes**, about **4.004 TiB**.

For a 4 TB decimal disk with 4 KiB blocks: 976,562,500 blocks. Bitmap size = 976,562,500 / 8 = 122,070,312.5 bytes, approximately **116.4 MiB**. For a 4 TiB disk, it is **128 MiB**. A linked free list has low per-entry overhead but slow traversal; grouping stores batches of free addresses; counting stores start/length pairs for contiguous runs.

### Completed allocation program – execution
```text
AAAAAA........CCCCC.............
E (10 blocks) contiguous: start 19
E (10 blocks) indexed: index block 6, data blocks 7 8 9 10 11 12 13 29 30 31
AAAAAAeEEEEEEECCCCCEEEEEEEEEEEEE
```

**Important inconsistency in the assignment prompt:** after releasing B and D, free blocks are 6–13 (8 blocks) and 19–31 (13 blocks). Therefore, the first-fit contiguous allocation of 10 blocks **succeeds at block 19**. This matches the assignment's own expected output. The question asking why E cannot be stored contiguously conflicts with the provided allocation sequence and expected output. To demonstrate external fragmentation, the input allocation sizes/placement must be changed so every free run is shorter than 10 while total free space remains at least 10. In the supplied run, indexed allocation then uses the remaining free blocks (index block 6 and data blocks 7–13, 29–31); indexed allocation works with scattered blocks but is not needed to make the first E allocation succeed.

FAT alternative: access to block k requires O(k) chain/table lookups. With 4-byte entries and 976,562,500 blocks, a FAT requires about 3.64 GiB of memory. Indexed access is O(1) for a single-level index or O(levels) for multilevel indexing. A damaged FAT can affect many files; duplicate FAT copies reduce but do not eliminate risk.

## Task 3 – Modern File Systems

| File system | Maximum-size notes | Journaling / consistency | Snapshots | Checksums | Copy-on-write |
|---|---|---|---|---|---|
| FAT32 | File limit 4 GiB − 1 byte | No journal | No | No end-to-end data checksums | No |
| NTFS | Very large; implementation limits apply | Metadata journal | VSS at Windows volume layer | Metadata integrity; optional data integrity streams | Not general CoW |
| ext4 | Common 4 KiB configuration: up to 16 TiB file and 1 EiB volume (feature-dependent) | Journal | No native snapshots | Metadata checksums | No |
| XFS | Multi-petabyte scale on modern systems | Metadata journal | Usually through LVM | Metadata checksums | No general CoW |
| Btrfs | Very large theoretical address space; practical limits lower | Transactional CoW | Native subvolume snapshots | Data and metadata checksums | Yes |
| ZFS | 128-bit architecture; practical limits very high | Transactional CoW and intent log | Native snapshots/clones | End-to-end checksums | Yes |
| APFS | Large 64-bit-scale limits | CoW metadata | Native snapshots | Metadata checksums | Yes |

Limits depend on OS version, features, block size and implementation.

A crash during a write can leave data and metadata inconsistent. Journaling records intended metadata updates before commit and replays/discards incomplete transactions. It does not guarantee every application data byte reached disk; applications still need fsync/fdatasync and correct ordering. ext4 data=journal journals data and metadata (strongest ordering, highest overhead); ordered mode writes data before related metadata (default balanced mode); writeback may commit metadata before data (weaker crash ordering). CoW writes new blocks and atomically switches metadata roots; snapshots preserve old references and checksums detect corruption, but small random writes can be amplified.

Recommendations: exFAT for a Windows/Linux USB drive; ext4 or XFS for a Linux database server after synchronous-I/O benchmarking; APFS/NTFS/ext4 according to laptop OS; ZFS or Btrfs for a snapshot backup server.

Inspect a real Linux filesystem using:
```sh
stat -f -c 'Type=%T BlockSize=%S Blocks=%b Free=%f' /
df -i /
ls -i . | head
tune2fs -l /dev/sdXN | grep -E 'Block size|Inode count|Free inodes|Free blocks'
```
Replace the device carefully; do not run tune2fs on a mounted or incorrectly identified device. These host-specific command outputs must be captured on the student's Linux machine.

A Btrfs/ZFS database may write new data and metadata blocks for a small random update, increasing write amplification and latency. Snapshots initially share blocks but consume more space as data diverges. Transactional recovery is generally fast and checksums help detect latent corruption. Benchmark synchronous writes, record size, compression and cache settings against ext4/XFS.

## Task 4 – Disk Scheduling Algorithms

Head = 53; cylinders 0–199; queue = 98, 183, 37, 122, 14, 124, 65, 67. SCAN/LOOK initially move toward cylinder 0. C-SCAN/C-LOOK are calculated with an upward sweep for C-SCAN and downward-first C-LOOK.

| Algorithm | Service order | Head movement |
|---|---|---:|
| FCFS | 98, 183, 37, 122, 14, 124, 65, 67 | 640 |
| SSTF | 65, 67, 37, 14, 98, 122, 124, 183 | 236 |
| SCAN | 37, 14, 0, 65, 67, 98, 122, 124, 183 | 236 |
| C-SCAN | 65, 67, 98, 122, 124, 183, 199, wrap to 0, 14, 37 | 382 including the 199-cylinder wrap |
| LOOK | 37, 14, 65, 67, 98, 122, 124, 183 | 208 |
| C-LOOK | 37, 14, jump to 183, 124, 122, 98, 67, 65 | 326 |

The C-SCAN assignment's expected output separates the 183-cylinder movement from the additional 199-cylinder wrap: 183 + 199 = **382 total**.

### Simulation
The included Python program runs reproducible synthetic batches of 100 (light), 1,000 (medium) and 10,000 (heavy) requests. All requests arrive at time zero; cylinder travel is used as service time. This is a simplified comparison, not a physical HDD benchmark.

| Load | Algorithm | Total movement | Average/request | Maximum wait (cylinder-time) |
|---|---|---:|---:|---:|
| Light (100) | FCFS | 6,506 | 65.06 | 6,493 |
|  | SSTF | 249 | 2.49 | 243 |
|  | SCAN | 251 | 2.49 | 245 |
|  | C-SCAN | 395 | 3.91 | 395 |
|  | LOOK | 249 | 2.49 | 243 |
|  | C-LOOK | 391 | 3.99 | 389 |
| Medium (1,000) | FCFS | 66,318 | 66.32 | 66,174 |
|  | SSTF | 252 | 0.25 | 252 |
|  | SCAN | 252 | 0.25 | 252 |
|  | C-SCAN | 397 | 0.40 | 397 |
|  | LOOK | 252 | 0.25 | 252 |
|  | C-LOOK | 397 | 0.40 | 396 |
| Heavy (10,000) | FCFS | 665,382 | 66.54 | 665,330 |
|  | SSTF | 252 | 0.03 | 252 |
|  | SCAN | 252 | 0.03 | 252 |
|  | C-SCAN | 397 | 0.04 | 397 |
|  | LOOK | 252 | 0.03 | 252 |
|  | C-LOOK | 397 | 0.04 | 397 |

The batch model's wait metric is cumulative head travel before service, not wall-clock time. It does not model requests arriving during service, rotational latency or device cache. SSTF can starve a far request if closer requests continuously arrive; a stream near cylinder 60 can repeatedly beat a request at cylinder 183. FCFS is arrival-fair but can move the head excessively. SCAN bounds waiting through sweeps; C-SCAN's one-direction service produces more uniform waiting. LOOK/C-LOOK avoid travel to unused disk endpoints.

SSDs have no mechanical seek/rotational delay, so minimizing head movement has little value. Linux may use the none scheduler to minimize overhead or mq-deadline to bound latency and prevent starvation. Inspect with `lsblk` then `cat /sys/block/<device>/queue/scheduler`.

LOOK requires finding the last pending request (O(n) scan or efficient sorted queue). A sorted structure supports O(log n) insertion/removal and efficient extreme selection.

## Task 5 – Distributed and Cloud-Based Storage

| Criterion | NFS | HDFS | CephFS/Ceph | Cloud object storage |
|---|---|---|---|---|
| Access | Remote POSIX-like filesystem | Distributed blocks, large sequential data | Distributed POSIX/object/block | HTTP object API |
| Consistency | Server-mediated close-to-open semantics; options matter | Metadata and replicated block pipeline | Strong consistency in normal Ceph operations | Strong read-after-write in major providers |
| Availability | Server/network/HA dependent | Replication and NameNode HA | Replication/erasure coding and quorum | Provider-managed multi-device durability |
| Performance | Low LAN latency; central bottleneck | High throughput; less suited to tiny random files | Scales across nodes with network/recovery overhead | Good for large objects; CDN improves global delivery |
| Cost | Hardware and administration | Multiple nodes and expertise | More nodes/network and expertise | Storage, requests, retrieval and transfer |

Recommend managed cloud object storage for general student documents with SSO/MFA, lifecycle rules and CDN for public course material. NFS suits legacy POSIX applications on a controlled LAN. Ceph suits private cloud where skilled operators are available. HDFS is primarily suited to analytics.

Three replicas can preserve availability after failure if placement/quorum permits; place copies across hosts, racks and power domains. Replication improves availability/read throughput but uses about 3× raw capacity. It is not a backup because deletion/ransomware can replicate; retain immutable/versioned backups. Strong consistency simplifies official grades and examination submissions but may require coordination. Eventual consistency allows temporarily stale reads and requires retries/version checks/conflict handling; it may be acceptable for public media caches.

Device management: drivers translate kernel I/O to device commands and handle interrupts/DMA; buffering absorbs speed differences; caching retains hot blocks; spooling queues serialized work; the block-device interface exposes addressable fixed-size blocks to filesystems and the page cache.

One large NFS server is simple at low concurrency but becomes a CPU/RAM/network/disk bottleneck and a single failure domain. Three-replica distributed storage uses more capacity and coordination but can serve reads across nodes and survive node failure when quorum permits.

## Task 6 – Video-Streaming Storage Design

Uploads are write-once; reads are many-to-one and highly skewed to the first seven days. Students seek to arbitrary timestamps. Proposed targets: cached campus first frame under 2 seconds; p99 seek under 500 ms within a region; no loss of acknowledged uploads; peak bitrate capacity with 30% headroom.

Store canonical media on object storage or HDD-backed storage, transcode into segmented HLS/DASH objects, and cache hot segments on SSDs and a CDN. Use XFS/ext4 on local cache nodes, object storage for durable originals/segments, indexed/extents allocation and a separate metadata database. Replicate metadata and keep versioned backups.

Illustrative sizing: 200,000 users, 10% concurrent = 20,000 users. At an average bitrate of 2 Mbps, aggregate egress = 40 Gbps; with 30% headroom plan about 52 Gbps across CDN/edge. If 5,000 videos average 1 GB, raw media is 5 TB; three durable copies require 15 TB before transcoded variants. With 4-second segments at 2 Mbps, each segment is about 1 MB. Seeking to minute m maps to segment index floor(60m/4), typically 1–2 object reads to start playback.

Compare HDD origin + SSD cache + CDN against HDD origin without cache/CDN using the same trace and fio/wrk; record p50/p95/p99, throughput, cache hit ratio and CPU. No physical benchmark is claimed. Cloud object storage + CDN improves geographic delivery but costs depend on GB-month, requests, egress and retrieval. A hybrid origin + CDN is recommended for a small team.

---

# ASSIGNMENT 5 – CO5

## Part A – Design

### A1 – Linux System Components
The Linux kernel is the privileged core managing processes, memory, filesystems, devices and networking through system calls. System libraries such as glibc wrap system calls; the shell interprets commands/scripts; utilities such as ps, systemctl, ip and df administer the system. CFS (with newer kernels moving toward EEVDF scheduling) shares CPU among runnable tasks; it is fair but not hard real-time. The page cache speeds repeated reads and buffers writes; reclaim and swap handle pressure, but excessive swapping increases latency. /proc exposes process/kernel runtime data; /sys exposes structured device/kernel attributes.

The FHS separates /etc configuration, /var variable data/logs, /home user data, /usr software and /dev device nodes. VFS provides a common interface across filesystem types. Permissions apply read/write/execute to owner/group/others; ACLs add per-user rules. Setuid runs with the file owner's effective UID and must be tightly controlled. sudo grants approved privileged commands with policy/logging.

Windows Server alternative: Server Core reduces footprint compared with a GUI installation. Linux shell/systemd/Ansible and Windows PowerShell/DSC both automate administration; effort depends on team expertise. Windows licensing is generally per-core with CAL considerations; exact cost depends on edition, core count and access model and must be quoted. Linux is a natural fit for a Python/Linux-native portal.

### A2 – System Administration
Create non-login service accounts portal and dbadmin; human accounts use individual SSH keys and MFA. Groups include portal-ops, db-readonly and sysadmins. Apply least privilege to app, logs and secrets. Run the app as a systemd service with User=portal, Restart=on-failure, NoNewPrivileges=true, PrivateTmp=true and resource limits. Persist journald logs and forward centrally; alert on 5xx rate, authentication failures, disk pressure and service restarts.

Use private RFC1918 addresses for application/database nodes. DNS points to a load balancer, not the database. Nginx terminates TLS and proxies to the app privately. Firewall default-deny inbound; allow 80/443 to proxy and 22 only from admin VPN/bastion; database port only from app subnet. Disable SSH password/root login and require keys.

Automate consistent database backups, checksum verification, restore tests, log rotation, disk/inode alerts and retention with cron/systemd timers. For one server, shell scripts may be quickest; for 20 servers Ansible inventory/roles/idempotency reduce configuration drift and parallelize work, with initial learning/inventory/secret-management overhead.

### A3 – Virtualisation
Type-1 hypervisors run directly on hardware (KVM, ESXi, Xen, Hyper-V); type-2 hypervisors run as host applications (VirtualBox, VMware Workstation). Full virtualization presents virtual hardware; paravirtualization uses guest-aware drivers. Intel VT-x and AMD-V provide hardware CPU virtualization; IOMMU supports device assignment.

VMs offer kernel isolation, consolidation, snapshots and migration, but consume guest OS RAM/disk and add CPU/I/O overhead and licence/patching work. Use type-1 VMs for production isolation between tiers/tenants; type-2 VirtualBox is suitable for development/labs. CPU-bound overhead can be modest with hardware support; I/O-bound workloads may incur more overhead.

### A4 – Containerisation
Namespaces isolate process IDs, mounts, networks, IPC, hostnames and user IDs. Cgroups account for and limit CPU, memory, I/O and process use. Image layers are immutable base/runtime, dependency and application layers; caching reuses unchanged layers.

| Dimension | Containers | Virtual machines |
|---|---|---|
| Start-up | Usually seconds or less | Tens of seconds to minutes |
| Memory | Shared host kernel | Each guest has kernel and OS services |
| Isolation | Namespaces/cgroups; shared kernel | Hardware-virtualized guest kernel boundary |
| Security | Needs seccomp, least capabilities and rootless mode | Stronger kernel boundary, but guest/hypervisor still need patching |

Use stateless portal replicas in containers for fast scale-out; use a managed database or isolated database VM/service with persistent storage. Separate VMs are appropriate when distinct kernels, stricter tenant isolation or legacy OS requirements are needed.

### A5 – Cloud Set-up and Kubernetes
IaaS provides VMs/networks/disks while the university manages OS/runtime; PaaS manages more of the deployment platform; SaaS is a finished application. Use managed Kubernetes or autoscaling VMs behind a cloud load balancer, managed HA database, object storage, CDN and monitoring. Keep database subnets private and encrypt data in transit/at rest.

A Pod is the smallest deployable unit. A Deployment maintains replicas and replaces failed Pods. A Service provides stable discovery/load balancing. Resource requests influence scheduling; limits cap CPU/memory. HPA adjusts replicas using CPU/memory/custom metrics. Readiness probes keep unready Pods out of traffic; liveness probes restart unhealthy containers. Use rolling updates, disruption budgets and topology spread. Pre-scale before result day to avoid cold-start delays.

Fixed result-day VMs give predictable capacity but sit idle most of the year and require manual changes. Kubernetes autoscaling adds complexity but adjusts replicas to demand; scheduled pre-scaling should complement reactive HPA.

## Part B – Implementation and Experimental Investigation

### B1 – Linux Administration
The completed Bash log-analysis script uses awk, sort and uniq to group status codes and client IPs. Executed output:
```text
Requests per status code:
200 4
302 1
403 1
404 1
Busiest client:
10.0.0.5 4
Total bytes sent: 43200
```

Cron example: `0 * * * * /absolute/path/log_summary.sh`. The repository includes a disk-space alert script, sample systemd unit and Nginx reverse-proxy configuration. Observe a real host using `ps aux --sort=-%cpu`, `top`, `free -h`, `vmstat 1 5`, `iostat -xz 1 5`, `df -h`, `df -i` and `ss -tulpn`. These are host-specific; actual output must be captured on the student's Linux system.

### B2 – Containerise the Portal
The repository includes a Dockerfile, Flask app, pinned requirements and Compose configuration. Build/run:
```sh
docker build -t result-portal .
docker run --rm -p 8000:8000 --cpus=0.5 --memory=512m result-portal
```
Check `http://localhost:8000`, `docker ps`, `docker images`, `docker history result-portal`, `docker exec <container> whoami` (expected user: portal) and `docker stats`. For Compose, set DB_PASSWORD in a local .env file and run `docker compose up --build -d`. The named pgdata volume persists database data. Never commit .env secrets.

The slim Python base reduces image size and package attack surface compared with a full Ubuntu base. Dependencies are installed before copying source to preserve layer cache. Docker was unavailable in the authoring execution environment; image size, docker stats, container identity and HTTP response are not claimed as executed.

### B3 – Deployment Comparison and Kubernetes
Kubernetes Deployment/Service and HPA manifests are included. Replace the illustrative image name with an accessible registry image or build/load the local image before applying. Use:
```sh
kubectl apply -f deployment.yaml
kubectl get pods -w
kubectl delete pod <one-result-portal-pod>
kubectl get pods -w
kubectl scale deployment/result-portal --replicas=6
kubectl get pods
kubectl describe pod <pod-name>
kubectl apply -f hpa.yaml
kubectl get hpa -w
```
Compare native Linux, VM and container using the same application/hardware. Measure startup with /usr/bin/time, memory with free/ps/docker stats, image/disk size with du/docker images and throughput with wrk. Docker, kubectl and a cluster were unavailable here; pod recovery and benchmark results are not fabricated.

Compose restarts containers on one host but cannot move them to another host if that server fails. Kubernetes needs a control plane but can reschedule Pods across nodes, provide stable Services and support autoscaling. Compose suits a single-host demo; Kubernetes suits multi-node availability and automated rollout.

### B4 – Final Engineering Decision
A Compose extension defines a second learning-management service with independent resource limits: portal 0.50 CPU/512 MiB and LMS sample 0.25 CPU/256 MiB. Start with `docker compose up -d` after confirming the Docker daemon and build context.

150,000 logins in two hours = 150,000/7,200 = **20.83 logins/s average**. If each login triggers eight HTTP requests, average demand is about **167 requests/s**. With a fivefold peak and 30% headroom, plan approximately **1,086 requests/s**. If one container sustains measured R requests/s at target latency, replicas = `ceil(1086 / (0.70 × R))`. This is a planning estimate; database contention, session storage and network limits can prevent linear scaling.

Recommended decision: hybrid. Retain private infrastructure for routine workloads and sensitive internal services; use a cloud load balancer/autoscaling application tier and managed HA database or warm standby for burst. Pre-scale before result day, cache static assets, rate-limit abusive clients, tune database pooling and load-test. Annual cloud cost must include baseline hours, burst hours, database, storage, monitoring, backup and egress; compare with hardware, power, cooling, licensing and staff. Cloud-only offers elasticity but increases provider dependency and data-governance considerations.

---

# Conclusion and Evidence Checklist

Assignments 4 and 5 now contain the written evaluations, calculations, completed C/Bash code, Docker/Compose/Kubernetes examples and reproducible run instructions.

**Executed in Debian GNU/Linux 13 with GCC 14.2.0:**
- Assignment 4 contiguous/indexed allocation C program compiled with `gcc -std=c11 -Wall -Wextra` and output matched the assignment's expected output.
- Assignment 4 disk scheduling C program compiled and produced FCFS, SSTF, SCAN, C-SCAN, LOOK and C-LOOK movement values.
- Assignment 4 synthetic Python simulation ran for 100, 1,000 and 10,000 requests.
- Assignment 5 Bash log-summary script ran and matched the expected output.

**Still requires execution on the student's own machine:** filesystem inspection commands and disk scheduler check; Docker build/run, image size, history, docker stats and container whoami; Docker Compose with database volume; native/VM/container load benchmarks; Kubernetes pod recreation and six-replica scaling; screenshots and command outputs. Docker, kubectl and a Kubernetes cluster were unavailable in the authoring environment, so these results are intentionally not invented.

Before final submission, capture the remaining evidence, confirm the Docker/Kubernetes image is available, and explain the design decisions during the assignment viva.