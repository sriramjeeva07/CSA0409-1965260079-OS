# A4 – Containerisation

Linux namespaces isolate process IDs, mounts, networks, IPC, hostnames and user IDs. Cgroups account for and limit CPU, memory, I/O and process use. Image layers are immutable base/runtime, dependency and application layers; caching reuses unchanged layers.

| Dimension | Containers | VMs |
|---|---|---|
| Start-up | Usually seconds or less | Tens of seconds to minutes |
| Memory | Shared host kernel | Each guest has kernel/OS services |
| Isolation | Namespaces/cgroups; shared kernel | Hardware-virtualized guest kernel boundary |
| Security | Needs seccomp, least capabilities, rootless mode | Stronger kernel boundary, but guest/hypervisor still need patching |

Use stateless portal replicas in containers for fast scale-out; use managed DB or isolated DB VM/service with persistent storage. Separate VMs fit distinct kernels, strict tenant isolation or legacy OS needs.