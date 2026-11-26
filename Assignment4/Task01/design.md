# Task 1 – File-System Design Review

## 1.1 Weaknesses
1. Flat namespace causes filename collisions.
2. Searching one directory containing files for 12,000 students increases lookup/listing overhead.
3. Contiguous allocation suffers external fragmentation; growth may require relocation.
4. No access control exposes coursework and personal data.
5. One directory complicates quotas, retention, backup and per-student recovery.
6. Group sharing lacks controlled namespaces and least-privilege permissions.

## 1.2 Improved design
Use /srv/students/<register-number>/{home,coursework,uploads} and /srv/projects/<course>/<project-id>/. Use register numbers as stable IDs, safe filename rules and path-traversal protection. Store owner, group, size, timestamps, type, checksum, ACL and quota metadata. Student folders use 0700; staff access uses course groups/ACLs; project folders use dedicated groups and 2770 setgid directories. Link shared folders into workspaces while retaining target permissions. Use extents/indexed allocation, snapshots, encrypted backups and tested restores.

## 1.3 Design defence (under 100 words)
Each student owns a private directory protected by owner-only permissions and server-side ACL checks. Faculty receive access through course groups, while project members receive access only to their project's shared directory. Separate project groups and setgid directories preserve group ownership for new files. Audit logs record access and changes. This provides private-by-default storage without preventing deliberate collaboration, and backups/snapshots support recovery without granting students access to one another's data.

## Cloud alternative
Managed drive search/open time depends on indexing, network latency, file size and local cache. Versions/recycle bins/replicas may consume extra storage. The college retains policy control through SSO, MFA, retention, DLP, audit logs and sharing restrictions, but delegates physical infrastructure to the provider.