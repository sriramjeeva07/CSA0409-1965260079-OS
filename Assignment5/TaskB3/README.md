# B3 – Deployment Comparison and Kubernetes

Build and load a local image into kind/minikube, or replace the manifest image with an accessible registry image. The sample image name is illustrative and must exist before apply.

```sh
docker build -t result-portal:1.0 ../TaskB2
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

For kind, load image using `kind load docker-image result-portal:1.0` and update the Deployment image to that tag with imagePullPolicy: IfNotPresent. For minikube, use its image build environment or load command.

Compare native Linux, VM and container with the same application/hardware. Record startup via /usr/bin/time, memory via free/ps/docker stats, image/disk size via du/docker images, throughput via wrk -t4 -c100 -d60s. Docker/kubectl/cluster are not available in the execution environment; do not report fabricated pod or benchmark results.