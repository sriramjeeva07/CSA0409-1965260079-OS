# A3 – Virtualisation

Type-1 hypervisors run directly on hardware (KVM, ESXi, Xen, Hyper-V); type-2 run as host applications (VirtualBox, VMware Workstation). Full virtualization presents virtual hardware; paravirtualization uses guest-aware drivers. Intel VT-x and AMD-V provide hardware CPU virtualization; IOMMU supports device assignment.

VMs offer kernel isolation, consolidation, snapshots and migration, but consume guest OS RAM/disk and add CPU/I/O overhead, licensing and patching. Use type-1 VMs for production isolation between tiers/tenants; type-2 VirtualBox is suitable for labs/development. CPU-bound overhead can be modest with hardware support; I/O-bound work may incur more overhead.