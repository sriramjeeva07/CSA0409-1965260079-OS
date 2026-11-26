#!/usr/bin/env python3
"""Reproducible synthetic batch simulation; every request arrives at t=0."""
import random

random.seed(409)
MAX_CYL = 199


def order(name, head, requests):
    if name == "FCFS":
        return requests[:]
    if name == "SSTF":
        pos, pending, result = head, requests[:], []
        while pending:
            item = min(pending, key=lambda x: (abs(x - pos), x))
            pending.remove(item)
            result.append(item)
            pos = item
        return result

    lower = sorted((x for x in requests if x < head), reverse=True)
    upper = sorted(x for x in requests if x >= head)
    if name == "SCAN":
        return lower + [0] + upper
    if name == "LOOK":
        return lower + upper
    if name == "C-SCAN":
        return upper + [MAX_CYL] + sorted(x for x in requests if x < head)
    if name == "C-LOOK":
        return lower + sorted((x for x in requests if x > head), reverse=True)
    raise ValueError(name)


def metrics(head, sequence, algorithm):
    pos, elapsed, total, waits = head, 0, 0, []
    for cylinder in sequence:
        waits.append(elapsed)
        if algorithm == "C-SCAN" and pos == MAX_CYL and cylinder < MAX_CYL:
            distance = MAX_CYL + cylinder  # travel to 0, then wrap to target
        else:
            distance = abs(cylinder - pos)
        total += distance
        elapsed += distance
        pos = cylinder
    return total, total / len(sequence), max(waits, default=0)


for label, count in [("light", 100), ("medium", 1000), ("heavy", 10000)]:
    requests = [random.randrange(200) for _ in range(count)]
    print(f"{label} load ({count} requests; all arrive at t=0)")
    for algorithm in ["FCFS", "SSTF", "SCAN", "C-SCAN", "LOOK", "C-LOOK"]:
        sequence = order(algorithm, 53, requests)
        total, average, maximum_wait = metrics(53, sequence, algorithm)
        print(f" {algorithm:6s} total={total} avg={average:.2f} "
              f"max_wait={maximum_wait} cylinder-time")
print("Model limitation: batch-only synthetic workload; no arrivals during service, "
      "rotational latency or device cache. Use measured device latency for production.")