# Operating Systems – Assignment 3 (CO3)

**Student:** S. Jeevan  
**Register number:** 1965260079  
**Repository:** https://github.com/sriramjeeva07/CSA0409-1965260079-OS

## Contents

- [Complete report (Markdown)](REPORT.md)
- [Assignment 3 source-code and report package (ZIP)](../OS_Assignment_3_Code_and_Report.zip)

## Task coverage

| Folder / question | Topic | Main work |
|---|---|---|
| Task01 / Q1 | Linking, loading and address binding | Static and dynamic linking, `dlopen()`, `dlsym()`, mappings and update evaluation |
| Task02 / Q2 | Paging | Address translation, page-table sizing, TLB, page sizes and fragmentation |
| Task03 / Q3 | Segmentation | Base-limit translation, protection, sharing and segmented paging |
| Task04 / Q4 | Virtual memory | Demand paging, EAT, copy-on-write, working set and thrashing |
| Task05 / Q5 | Page replacement | FIFO, LRU, Optimal, Clock and Belady's anomaly |
| Task06 / Q6 | Disk management | Disk preparation, boot, swap and RAID comparison |
| Task07 / Q7 | Smart-factory memory plan | Locked real-time working set, ML/logging budgets and evaluation |

## Build and execution notes

The ZIP package already present at the repository root contains the earlier Assignment 3 code/report package. The detailed report in this folder follows the supplied CO3 assignment brief.

The assignment requires actual execution evidence: compiler and Linux versions, command output, screenshots, timing, page-fault counts, generated graphs and memory measurements. Do not treat illustrative calculations or expected values as proof of execution. Replace every marked evidence field with output from your own machine before final submission.

Suggested C compilation pattern (adjust source filename and required libraries for each task):

```bash
gcc -std=c11 -Wall -Wextra source.c -o program
./program
```

For the dynamic-loading task, link with `-ldl` where required. For memory-locking tasks, check the system's `RLIMIT_MEMLOCK` and permissions.

## Important verification checklist

- [ ] Confirm all seven task programs are present in the ZIP package and compile successfully.
- [ ] Complete every TODO in the assignment-provided code.
- [ ] Add actual terminal outputs and screenshots.
- [ ] Add the Linux distribution and GCC version used.
- [ ] Add measured timing, memory and page-fault data; do not invent results.
- [ ] Verify the report's expected values against the executed programs.
- [ ] Ensure the GitHub repository is accessible to course faculty.
