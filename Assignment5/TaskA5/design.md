# A5 – Cloud and Orchestration

IaaS provides VMs/networks/disks while university manages OS/runtime; PaaS manages more of the deployment platform; SaaS is a finished application. Use managed Kubernetes or autoscaling VMs behind a cloud load balancer, managed HA database, object storage, CDN and monitoring. Keep database subnets private and encrypt data in transit/at rest.

A Pod is the smallest deployable unit. Deployment maintains replicas and replaces failed Pods. Service provides stable discovery/load balancing. Requests influence scheduling; limits cap CPU/memory. HPA adjusts replicas using CPU/memory/custom metrics. Readiness probes keep unready Pods out of traffic; liveness probes restart unhealthy containers. Use rolling updates, disruption budgets and topology spread. Pre-scale before result day to avoid cold-start delays.

Fixed result-day VMs give predictable capacity but sit idle most of the year and require manual changes. Kubernetes autoscaling adds complexity but adjusts replicas to demand; scheduled pre-scaling should complement reactive HPA.