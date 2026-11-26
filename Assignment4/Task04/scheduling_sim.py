#!/usr/bin/env python3
"""Reproducible batch model: all requests in a batch arrive at time zero."""
import random
random.seed(409)
MAX=199
def order(name,head,q):
    if name=="FCFS": return q[:]
    if name=="SSTF":
        p=head; pending=q[:]; out=[]
        while pending:
            x=min(pending,key=lambda v:(abs(v-p),v)); pending.remove(x); out.append(x); p=x
        return out
    lo=sorted((x for x in q if x<head),reverse=True); hi=sorted(x for x in q if x>=head)
    if name=="SCAN": return lo+[0]+hi
    if name=="LOOK": return lo+hi
    if name=="C-SCAN": return hi+[MAX]+sorted(x for x in q if x<head)
    if name=="C-LOOK": return lo+sorted((x for x in q if x>head),reverse=True)
    raise ValueError(name)
def measure(head,seq,name):
    p=head; total=0; elapsed=0; waits=[]
    for x in seq:
        d=abs(x-p); total+=d; waits.append(elapsed); elapsed+=d; p=x
    if name=="C-SCAN" and MAX in seq:
        i=seq.index(MAX)
        if i+1<len(seq): total+=MAX; elapsed+=MAX
    return total,total/len(seq),max(waits,default=0)
for label,n in [("light",100),("medium",1000),("heavy",10000)]:
    q=[random.randrange(200) for _ in range(n)]
    print(f"{label} load ({n} requests; all arrive at t=0)")
    for alg in ["FCFS","SSTF","SCAN","C-SCAN","LOOK","C-LOOK"]:
        seq=order(alg,53,q); total,avg,mw=measure(53,seq,alg)
        print(f" {alg:6s} total={total} avg={avg:.2f} max_wait={mw} cylinder-time")
print("Model limitation: batch-only synthetic workload; use arrival timestamps and measured device latency for production.")