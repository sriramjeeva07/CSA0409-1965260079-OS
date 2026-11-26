# Operating Systems – Assignment 3 (CO3)

OPERATING SYSTEMS

ASSIGNMENT 3 – CO3

Memory Management, Virtual Memory
and Disk Management

Individual Assignment Report

## Declaration and Report Scope

This report follows the seven questions in the supplied Operating Systems Assignment 3 – CO3 brief. The submitted code, screenshots, measurements and outputs must be the student's own executed work.

### Course Outcome

CO3: Evaluate memory-management mechanisms such as linking, paging and segmentation to determine their effectiveness for different system requirements. Topics include linking and loading, paging, segmentation, virtual memory, demand paging, page-replacement techniques and disk management.

## Contents

Q1 – Linking, Loading and Address Binding

Q2 – Paging

Q3 – Segmentation

Q4 – Virtual Memory and Demand Paging

Q5 – Page-Replacement Techniques

Q6 – Disk Management

Q7 – Memory-Management Plan for a Smart Factory

Overall Conclusion and Evidence Checklist

## Q1 – Linking, Loading and Address Binding

### 1.1 Problem overview

The banking application contains multiple modules and libraries. When its encryption library receives a security fix, distributing a new large executable to every branch is costly. Static linking, dynamic linking and run-time loading offer different ways to manage this dependency.

### 1.2 Address binding and system roles

### 1.3 Static and dynamic linking

### 1.4 Run-time loading with dlopen() and dlsym()

dlopen() loads a shared object when a feature is needed. dlsym() retrieves the address of a symbol. The program should check dlerror(), handle missing libraries or symbols safely, and call dlclose() when the handle is no longer needed.

Illustrative pattern (adapt the library and symbol names to your actual program):

void *handle = dlopen("./libcrypto_demo.so", RTLD_LAZY);
if (!handle) { fprintf(stderr, "%s\\n", dlerror()); return 1; }
dlerror();
void (*fn)(void) = (void (*)(void)) dlsym(handle, "encrypt_demo");
const char *err = dlerror();
if (err) { fprintf(stderr, "%s\\n", err); dlclose(handle); return 1; }
fn();
dlclose(handle);

### 1.5 Experimental evidence

### 1.6 Evaluation and recommendation

Dynamic linking is generally suitable for the bank because a compatible encryption-library security update can be deployed without rebuilding every application executable. Run-time loading is useful if encryption is optional or rarely used, since the library is mapped only when required. The bank must manage library compatibility, trusted paths and failure handling. Static linking remains useful when a self-contained deployment is essential. The final choice should be supported by the measured executable sizes, start-up time and memory use.

### 1.7 Alternative concept and complexity

Using dlopen() only when encryption is requested can reduce start-up work and memory use when encryption is not used. The first encryption request may take longer, and the application must handle a missing library or symbol. This changes when the loading work occurs; it does not improve the encryption algorithm's complexity.

## Q2 – Paging

### 2.1 External fragmentation

External fragmentation occurs when free memory is split into separated holes. Total free memory may be sufficient, but no individual hole is large enough for a process. Paging divides virtual memory into fixed-size pages and physical memory into equal-sized frames. Any free frame can hold a page, so a process need not occupy one contiguous physical region. Paging removes external fragmentation from physical allocation, but internal fragmentation can remain in a partly used final page.

### 2.2 Logical-to-physical translation

For page size P and logical address A: page number p = floor(A/P), offset d = A mod P. The page table maps p to frame f. If valid, physical address = f × P + d. A nonresident page causes a page fault; a page number outside the table is invalid.

These are the assignment's expected translations for page size 1024 bytes and page table {5, 2, -1, 7, 0, -1, 3, -1}.

### 2.3 Page-table size

These are flat single-level page-table sizes for a complete address space. Real systems use multi-level, hashed or inverted structures to avoid allocating such a large table for every process.

### 2.4 TLB and effective access time

A TLB caches recent translations. Let T be TLB lookup time, M be memory access time and h be the hit ratio. Under a simplified model where a TLB miss requires one page-table memory access and one data access: EAT = h(T + M) + (1 − h)(T + 2M). This excludes page faults.

### 2.5 Page-size evaluation

### 2.6 Page-table structures, protection and sharing

Hierarchical tables split the virtual address into indexes and allocate lower-level tables only for populated regions.

Hashed tables use a hash of the virtual page number to locate mappings; useful for sparse, large address spaces.

Inverted tables keep one entry per physical frame, reducing storage in systems with large virtual address spaces.

Protection bits mark pages read-only, writable, executable or user/supervisor accessible.

Shared pages let processes map the same physical frame, as with shared libraries or shared-memory regions.

### 2.7 Internal-fragmentation experiment

For process size S and page size P: allocated bytes = ceil(S/P) × P; internal fragmentation = allocated bytes − S. Sum this value over the same 100 generated process sizes for 1 KB, 4 KB and 2 MB pages.

### 2.8 Linux verification and completed code

Run getconf PAGESIZE to identify the system page size. Run the test program using /usr/bin/time -v to inspect major and minor page faults. The expected 1 KB program output is shown in Section 2.2. When PAGE_SIZE is changed to 2048, recompute page = address / 2048 and offset = address % 2048, then include your actual new output.

### 2.9 Alternative: inverted page table

An inverted page table has one entry per physical frame, so storage is proportional to physical memory rather than all virtual pages of all processes. This can greatly reduce table size. Translation is more complex and commonly uses a hash lookup on process identity and virtual page number. A multi-level table instead performs a structured walk through levels.

## Q3 – Segmentation

### 3.1 Address translation

Segmentation divides a program into logical regions such as code, data, heap and stack. A logical address contains a segment number and offset. Each segment-table entry stores a base and limit. If offset < limit, physical address = base + offset; otherwise the access exceeds the segment boundary and causes a protection fault.

The numbers above are illustrative examples; use your program's actual segment-table values if they differ.

### 3.2 Protection, sharing and fragmentation

Segmentation matches the logical program view by separating code, data, heap and stack.

Different permissions can be assigned to each segment; code can be read-only and executable.

Read-only code segments can be shared among processes running the same program.

Pure segmentation can suffer external fragmentation because variable-sized segments need contiguous physical memory.

### 3.3 Compare memory schemes

### 3.4 Segmentation with paging

The segment selector identifies a segment-table entry. That entry points to the segment's page table. The offset is divided into page number and in-page offset. The page table maps the page to a physical frame, and the in-page offset is added to the frame base.

### 3.5 Experimental investigation

Translate eight (segment, offset) addresses by hand and verify them with a small program. Include valid and invalid offsets. Show /proc/<pid>/maps for a running process. A deliberate invalid memory access can raise SIGSEGV; the kernel reports the protection violation and normally terminates the process.

For the 20-segment random experiment, record allocations and removals, free holes, total free space and largest contiguous free block. State the fragmentation metric used.

### 3.6 Alternative: pure paging

Pure paging represents code, data and stack as pages rather than variable-sized segments. It removes external fragmentation, but can have internal fragmentation in the last page of a region. Multi-level tables reduce unused page-table allocation. A TLB accelerates translation. Code sharing and protection remain possible through read-only mappings and shared physical frames, though logical grouping is less explicit than segmentation.

## Q4 – Virtual Memory and Demand Paging

### 4.1 Demand-paging context

The radiology application needs 24 GB of virtual memory while the laptop has 16 GB RAM. Demand paging keeps currently needed pages in RAM and brings other pages from backing storage when referenced. A program can therefore have a virtual address-space requirement larger than RAM, provided its active working set and backing-store capacity are manageable.

### 4.2 Page-fault handling

CPU references a page marked not present; the MMU raises a page-fault exception.

The OS checks whether the address and requested access are valid.

If invalid or prohibited, the OS reports an error to the process.

Otherwise, the OS finds the page on storage or creates a zero-filled page.

The OS obtains a free frame or selects a victim; a dirty victim may need writing back.

The required page is loaded and page-table/TLB state is updated.

The interrupted instruction is restarted.

### 4.3 Effective access time

Given memory access = 100 ns and page-fault service = 8 ms = 8,000,000 ns, use EAT = (1 − p) × 100 + p × 8,000,000 ns.

Even a small page-fault rate can dominate normal memory access because servicing a fault takes milliseconds.

### 4.4 Maximum fault rate for 10% slowdown

A 10% slowdown permits EAT = 110 ns. Solving 110 = (1 − p)100 + p(8,000,000) gives p = 10 / 7,999,900 ≈ 0.00000125, or roughly one fault per 800,000 references under this simplified model.

### 4.5 COW, thrashing and working set

Copy-on-write lets parent and child processes initially share pages after fork(); a page is copied only when one process writes to it.

COW avoids immediately copying all memory during fork().

Thrashing occurs when processes spend excessive time paging because their active working sets do not fit in available frames.

The working-set model estimates recently used pages; the OS can reduce multiprogramming or allocate more frames when combined working sets exceed RAM.

### 4.6 Frame allocation and OS response

Equal allocation gives processes the same number of frames; proportional allocation assigns frames according to process size. Priority-based allocation can reserve more frames for critical processes. High page-fault rates, low CPU utilisation and sustained paging activity can indicate thrashing. The OS can suspend or swap out processes to restore stable working sets.

### 4.7 Experimental evidence

### 4.8 Engineering decision

Improving locality is often the first software-level step because it reduces repeated faults without purchasing hardware. If the measured working set consistently exceeds RAM, adding RAM can provide a substantial improvement. Compare measured runtime and fault rates with the cost of a memory upgrade. A 32 GB system may hold the 24 GB workload more comfortably, but OS and other process memory must also be counted.

### 4.9 Alternative: 32 GB RAM

If the 24 GB application and supporting processes fit in 32 GB, storage-backed faults may be greatly reduced and EAT approaches normal memory access time. Substitute the measured fault probability into the formula in Section 4.3 for a fair comparison. Both systems should be tested with the same workload.

## Q5 – Page-Replacement Techniques

### 5.1 Algorithms

### 5.2 Assignment reference string

For the supplied reference string {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1}, the assignment brief gives the following expected output:

Confirm these expected values using your own completed executable and attach the terminal output.

### 5.3 Belady's anomaly

For the supplied Belady string {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5}, FIFO gives 9 faults with 3 frames and 10 faults with 4 frames, according to the assignment brief. The queue order changes when the frame count changes, so adding a frame does not guarantee that the pages retained with fewer frames are preserved.

LRU does not suffer from Belady's anomaly because it is a stack algorithm: the pages resident with n frames form a subset of those resident with n+1 frames for the same reference prefix.

### 5.4 Optimal, complexity and workload

Optimal is a theoretical lower bound because it uses future references, which a real OS cannot know exactly. In a simple simulator with N references and F frames, linear frame lookup is O(NF). FIFO replacement can be O(1) after lookup using a circular pointer. LRU timestamps need O(F) storage; choosing the oldest victim by scanning costs O(F) on replacement. The simple Optimal algorithm scans future references and can take O(N²F) in the worst case.

Strong temporal locality tends to favour LRU. Random access weakens recency benefits. For a general-purpose web server, Clock is a practical recommendation because it approximates recency with low overhead; measurements should decide whether exact LRU's extra cost is justified.

### 5.5 Experimental evidence

## Q6 – Disk Management

### 6.1 Disk preparation and boot process

Low-level formatting prepares physical sectors; modern drives are generally formatted by the manufacturer.

Partitioning divides a disk into logical regions using GPT or MBR.

Logical formatting creates a filesystem such as ext4 or XFS in a partition or logical volume.

Firmware initialises hardware and selects a boot device; the bootloader loads the kernel and initial RAM filesystem.

Bad blocks may be remapped by drive firmware; filesystems report errors. Monitor and replace failing drives rather than relying on remapping permanently.

### 6.2 RAID comparison for six 4 TB disks

Capacities use decimal TB. RAID 1 depends on layout; the 4 TB figure assumes a six-way mirror.

### 6.3 Hospital records server recommendation

RAID 10 is a strong choice when predictable performance and resilience are priorities, especially for write-intensive database workloads. Six 4 TB drives provide about 12 TB usable capacity. RAID 6 is an alternative when higher capacity and survival of any two disk failures are more important; it provides 16 TB. RAID is not a backup: maintain tested offline or immutable backups, monitoring and restoration procedures.

### 6.4 Swap

Swap supports less-active anonymous pages and can absorb some memory pressure. Size depends on RAM, peak workload, hibernation needs and policy. A dedicated device can provide predictable placement; a swap file is easier to resize. Swap is not a substitute for sufficient RAM in a latency-sensitive server.

### 6.5 Linux inspection evidence

Use lsblk for devices, partitions, filesystems and mount points; sudo fdisk -l for partition tables; swapon --show or cat /proc/swaps for active swap.

### 6.6 RAID 6 alternative and reliability

RAID 6 usable capacity = (6 − 2) × 4 TB = 16 TB. It can survive any two disk failures because it stores two independent parity values. Writes have extra parity calculation and update overhead. A numerical probability of data loss needs a specified disk failure probability, time interval, rebuild duration and independence assumptions; the assignment brief does not provide these inputs, so do not invent a probability.

## Q7 – Memory-Management Plan for a Smart Factory Edge Device

### 7.1 Requirements

### 7.2 Mechanism comparison

### 7.3 Proposed design

Reserve and pre-fault the control task's code, stack and data before real-time operation.

Use mlock() or mlockall() where permitted; check return values and memory-lock limits.

Touch every required page during initialisation so first-touch faults occur before the control loop.

Avoid dynamic allocation, file I/O, blocking locks and unbounded system calls in the critical section.

Set explicit memory budgets for ML and logging, leaving room for kernel, drivers and transient allocations.

Use bounded queues and log rotation so logging cannot consume all memory.

Monitor pressure and faults; defer additional workloads if the safety margin would be exceeded.

The arithmetic is restrictive: 1.5 GB for ML leaves at most about 0.5 GB of nominal 2 GB for the control task, logger, kernel, drivers and all other allocations. Actual application capacity is lower after system overhead. Peak memory must be measured.

### 7.4 Verification measurements

Run the time-critical loop with and without memory locking, pre-fault the same working set, and measure maximum observed loop delay over a documented number of iterations. Record CPU, kernel, compiler options, system load and sample count. An observed maximum is not a formal hard real-time guarantee.

### 7.5 Fourth task of 500 MB

Adding another 500 MB task would exceed the remaining budget in many realistic configurations: the 1.5 GB ML task plus 500 MB already uses the nominal 2 GB before the control task, logger, kernel and drivers. Do not overcommit. Reduce the ML working set, move work elsewhere, upgrade RAM or reject/defer the new task through memory admission control.

### 7.6 Limitation and RTOS alternative

Linux memory locking reduces paging risk but does not by itself guarantee hard real-time behaviour. Interrupts, scheduler latency, drivers, firmware and resource contention can still delay execution. An RTOS without virtual memory can use fixed preallocated physical regions and avoid demand-paging faults, improving determinism. However, the ML task and future tasks must fit fixed RAM; adding tasks requires careful static memory planning and validation.

## Overall Conclusion

The assignment compares memory-management and storage mechanisms against practical requirements. Dynamic linking improves update flexibility, while static linking favours self-contained deployment. Paging removes external fragmentation in physical allocation; segmentation reflects logical program regions and supports natural protection and sharing. Demand paging allows virtual address spaces larger than RAM, but page faults are expensive and excessive paging causes thrashing. FIFO, LRU, Optimal and Clock differ in fault behaviour and implementation overhead. RAID choices balance capacity, performance and fault tolerance. For the smart-factory device, the critical control task should have a reserved, pre-faulted and locked working set, while non-critical tasks operate within explicit memory budgets.

The final report must be supported by evidence collected on the student's own Linux system. Expected outputs and illustrative examples in this report must be checked against the submitted code and actual execution.

## Execution Evidence Checklist

Q1: static/dynamic builds, sizes, ldd, dlopen/dlsym, /proc/self/maps, timing, memory and missing-library test.

Q2: completed address-translation program for 1 KB and 2 KB pages, 100-process fragmentation calculation, TLB calculations, getconf PAGESIZE and time -v output.

Q3: eight address translations, segment-limit test, /proc/<pid>/maps, SIGSEGV evidence and fragmentation calculation.

Q4: sequential/random memory test, page-fault counts, memory-growth test, fork() COW evidence and faults-versus-frames graph.

Q5: completed FIFO/LRU/Optimal code, hand calculation, 3–10 frame experiments, graph, 100,000-reference timing and Clock comparison.

Q6: lsblk, fdisk -l, swapon --show, RAID capacity/reliability calculations and SSD/HDD read-speed evidence if available.

Q7: memory-locking source, with/without-locking measurements, page-replacement simulation and memory budget.

README.md contains student details, Linux/compiler versions, compile commands, run instructions and a result for every task.

Working GitHub repository URL appears on the cover page and contains source, outputs and screenshots.

## References

Operating Systems Assignment 3 – CO3, supplied assignment brief (10 pages).

Linux manual pages relevant to implementation: dlopen(3), dlsym(3), proc(5), getconf(1), time(1), mlock(2), fork(2), lsblk(8), fdisk(8), swapon(8).


### Table 1

| Student information | Details |
| --- | --- |
| Name | S. Jeevan |
| Register number | 1965260079 |
| Course | Operating Systems |
| Course Outcome | CO3 – Evaluate memory-management mechanisms |
| GitHub repository | https://github.com/sriramjeeva07/CSA0409-1965260079-OS |
| Linux distribution / version | FILL IN YOUR ACTUAL SYSTEM |
| Compiler / version | FILL IN ACTUAL VERSION |
| Submission date | FILL IN |


### Table 2

| Important: This report is prepared from the assignment brief, not from your source-code repository. It does not invent machine-specific measurements. Fill every marked evidence field with your actual program output before submitting. |
| --- |


### Table 3

| Concept | Explanation |
| --- | --- |
| Compile-time binding | Addresses are fixed during compilation; this requires the program's memory location to be known. |
| Load-time binding | The program is compiled as relocatable code. The loader chooses its memory location and adjusts address references. |
| Execution-time binding | Address translation continues while the program runs, usually with MMU support; a process can move without rewriting logical addresses. |
| Linker | Combines object files, resolves external symbols and produces an executable or shared object. |
| Loader | Maps the executable and shared libraries into memory, performs required relocations and starts the program. |
| Position-independent code | Code that can execute correctly regardless of its mapped address; commonly used for shared libraries. |


### Table 4

| Factor | Static linking | Dynamic linking |
| --- | --- | --- |
| Executable size | Usually larger; library code is copied into executable. | Usually smaller; library code remains in separate shared objects. |
| Memory | Separate executables can each contain library code. | Read-only shared-library pages can be shared between processes. |
| Start-up | No shared-library lookup for statically linked components; larger file may need loading. | Loader locates and maps required shared libraries. |
| Updating | Usually rebuild and redistribute executable. | Compatible shared library can be updated independently. |
| Deployment | Self-contained, but larger and less flexible to update. | Smaller application, but depends on compatible libraries. |


### Table 5

| Required experiment | Insert your actual result |
| --- | --- |
| Static and dynamic executable sizes | ADD SIZE OUTPUT / SCREENSHOT |
| ldd output | PASTE OUTPUT |
| dlopen()/dlsym() run | PASTE TERMINAL OUTPUT |
| /proc/self/maps | ADD RELEVANT LINES / SCREENSHOT |
| Start-up and memory comparison | ADD COMMAND, VALUES AND UNITS |
| Two process shared-page evidence | ADD smaps / pmap OBSERVATION |
| Missing-library test | ADD ERROR OUTPUT AND EXPLANATION |


### Table 6

| Logical address | Page | Offset | Frame / status | Physical address |
| --- | --- | --- | --- | --- |
| 0 | 0 | 0 | 5 | 5120 |
| 1500 | 1 | 476 | 2 | 2524 |
| 3100 | 3 | 28 | 7 | 7196 |
| 6200 | 6 | 56 | 3 | 3128 |
| 9000 | 8 | 808 | Outside table | Invalid address |
| 2049 | 2 | 1 | Not resident | Page fault |


### Table 7

| Address space | Page size | Entries | At 4 bytes per entry |
| --- | --- | --- | --- |
| 32-bit | 4 KB (2^12) | 2^20 = 1,048,576 | 4 MiB |
| 48-bit | 4 KB (2^12) | 2^36 = 68,719,476,736 | 256 GiB |


### Table 8

| TLB hit ratio | EAT expression |
| --- | --- |
| 80% | 0.80(T+M) + 0.20(T+2M) |
| 95% | 0.95(T+M) + 0.05(T+2M) |
| 99% | 0.99(T+M) + 0.01(T+2M) |


### Table 9

| Page size | Advantages | Disadvantages |
| --- | --- | --- |
| 1 KB | Low average internal waste; fine-grained allocation. | More pages, larger page tables and more TLB pressure. |
| 4 KB | Practical balance and widely used. | Still many entries for very large address spaces. |
| 2 MB | Fewer entries and larger TLB reach for large contiguous working sets. | Greater internal waste; harder to allocate large contiguous frames. |


### Table 10

| Insert your generated process sizes, random seed, totals and graph. Do not report simulated values unless you ran the program. |
| --- |


### Table 11

| Segment | Base | Limit | Example offset | Result |
| --- | --- | --- | --- | --- |
| Code | 10000 | 4000 | 120 | Valid: 10120 |
| Data | 20000 | 3000 | 2999 | Valid: 22999 |
| Stack | 30000 | 2000 | 2000 | Invalid: at limit |


### Table 12

| Scheme | Allocation | Fragmentation | Key evaluation |
| --- | --- | --- | --- |
| Pure paging | Fixed-size pages/frames | No external; some internal | Flexible physical allocation; page-table overhead. |
| Pure segmentation | Variable-size logical segments | External | Natural logical protection/sharing; contiguous allocation and compaction are concerns. |
| Segmented paging | Each segment divided into pages | No external physical fragmentation; some page internal waste | Combines logical segments with paging; translation is more complex. |


### Table 13

| Fault rate | p | Effective access time |
| --- | --- | --- |
| 1 in 1,000 | 0.001 | 8,099.9 ns |
| 1 in 100,000 | 0.00001 | 179.999 ns |
| 1 in 1,000,000 | 0.000001 | 107.9999 ns |


### Table 14

| Experiment | Evidence to insert |
| --- | --- |
| Sequential vs random memory access | Source, input size, major/minor faults and runtime |
| Memory use beyond free RAM | VM or memory-limit setup, runtime and fault counts |
| fork() copy-on-write | Parent/child memory before and after child writes |
| Faults vs frame count | Reference string, algorithm, graph and interpretation |


### Table 15

| Algorithm | Rule | Evaluation |
| --- | --- | --- |
| FIFO | Evict the page loaded earliest. | Simple queue; may show Belady's anomaly. |
| LRU | Evict the page least recently used. | Uses locality; exact implementation may need timestamps or list updates. |
| Optimal | Evict the page whose next use is farthest in the future. | Minimum possible faults for known reference string; future is unknown in practice. |
| Clock / second chance | Circular list with reference bits; set bits are cleared and get another chance. | Low-overhead approximation to LRU. |


### Table 16

| Frames | FIFO faults | LRU faults | Optimal faults |
| --- | --- | --- | --- |
| 3 | 15 | 12 | 9 |
| 4 | 10 | 8 | 8 |


### Table 17

| Experiment | Evidence to insert |
| --- | --- |
| Locality and random reference strings | Generation method, seed and samples |
| Faults for 3–10 frames | Table and graph for FIFO, LRU and Optimal |
| 100,000-reference runtime | Machine details, compiler command and timings |
| Hand calculation | Step-by-step frame states and fault count |
| Clock versus LRU | Clock source, fault count and comparison |


### Table 18

| RAID | Usable capacity | Fault tolerance | Characteristics |
| --- | --- | --- | --- |
| RAID 0 | 24 TB | None | Striping; fast, but any disk failure loses the array. |
| RAID 1 | 4 TB if all six are one mirror | Up to five failures if six independent copies | Strong redundancy, low usable capacity. Three mirrored pairs instead give 12 TB. |
| RAID 5 | 20 TB | One disk failure | Distributed parity; good capacity, parity write overhead. |
| RAID 10 | 12 TB | At least one; possibly multiple in different mirror pairs | Striped mirrors; strong performance and redundancy. |
| RAID 6 | 16 TB | Any two disk failures | Dual parity; lower capacity than RAID 5, more write overhead. |


### Table 19

| Experiment | Insert actual output |
| --- | --- |
| lsblk | PASTE OUTPUT / SCREENSHOT |
| fdisk -l | PASTE RELEVANT OUTPUT |
| swapon --show | PASTE OUTPUT |
| SSD/HDD read speed | DEVICE, TOOL, FILE SIZE, CACHE STATE AND RESULT |
| RAID calculations | Show formula and RAID 1 layout assumption |


### Table 20

| Task | Memory and timing need | Fault tolerance |
| --- | --- | --- |
| Real-time control | Small, bounded, preallocated working set; strict deadline. | Must not page fault during critical loop. |
| Machine learning | Approximately 1.5 GB working set; throughput-oriented. | May tolerate some delay, but no swap and limited RAM constrain it. |
| Logging | Bounded buffers and background I/O. | Can tolerate occasional delay and lower priority. |


### Table 21

| Mechanism | Control task | ML task | Logging |
| --- | --- | --- | --- |
| Contiguous allocation | Predictable if reserved, but inflexible. | Large contiguous reservation may be difficult. | Useful for fixed buffers. |
| Paging | Only if critical pages are resident and locked. | Flexible allocation and protection. | Suitable for ordinary process memory. |
| Segmentation | Separates code, stack and data logically. | Logical protection; pure form can fragment. | Useful logical organisation. |
| Demand paging | Avoid in critical loop due to unpredictable faults. | May load pages on demand, but no swap limits it. | Acceptable for non-critical portions. |


### Table 22

| Measurement | Without locking | With locking |
| --- | --- | --- |
| Maximum loop delay | FILL FROM RUN | FILL FROM RUN |
| Median / average delay | FILL FROM RUN | FILL FROM RUN |
| Minor / major page faults | FILL FROM RUN | FILL FROM RUN |
| Memory locked | FILL FROM RUN | FILL FROM RUN |