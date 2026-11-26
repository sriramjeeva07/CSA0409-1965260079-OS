# Task 4 – Disk Scheduling Evaluation

Head=53; cylinders 0–199; queue 98,183,37,122,14,124,65,67. SCAN/LOOK initially move toward 0; C-SCAN/C-LOOK move upward unless noted.

| Algorithm | Service order | Movement |
|---|---|---:|
| FCFS | 98,183,37,122,14,124,65,67 | 640 |
| SSTF | 65,67,37,14,98,122,124,183 | 236 |
| SCAN | 37,14,0,65,67,98,122,124,183 | 236 |
| C-SCAN | 65,67,98,122,124,183,199, wrap 0,14,37 | 382 incl. 199-cylinder wrap (183 excluding wrap) |
| LOOK | 37,14,65,67,98,122,124,183 | 208 |
| C-LOOK | 37,14,183,124,122,98,67,65 | 326 |

SSTF can starve a far request if close requests continuously arrive; a stream near cylinder 60 can repeatedly beat cylinder 183. FCFS is arrival-fair but may cause large movement. SCAN gives bounded sweep service; C-SCAN's one-direction service makes waiting more uniform. LOOK/C-LOOK avoid travel to unused endpoints.

The included simulation uses reproducible random batches (100/1,000/10,000 requests) with all requests arriving at t=0 and seek distance as service time. It is a simplified model, not a physical disk benchmark.

SSDs have no mechanical seek/rotational delay. Linux may use none to minimize scheduler overhead or mq-deadline to bound latency/prevent starvation. Inspect cat /sys/block/<device>/queue/scheduler after identifying the correct device with lsblk.

LOOK movement is 208 vs SCAN 236. Finding an extreme by scanning pending requests is O(n); a balanced sorted structure gives O(log n) insertion/removal and efficient extreme selection.