# B1 – Linux Administration Runbook

Run the sample:
```sh
chmod +x log_summary.sh disk_alert.sh
./log_summary.sh
./disk_alert.sh
```
Executed output is saved in [execution_output.txt](execution_output.txt). Expected log summary:
```text
Requests per status code:
200 4
302 1
403 1
404 1
Busiest client:
10.0.0.5 4
Total bytes sent: 43200
```
Cron hourly: `0 * * * * /absolute/path/log_summary.sh`.

Observe the host using:
```sh
ps aux --sort=-%cpu | head
top
free -h
vmstat 1 5
iostat -xz 1 5
df -h
df -i
ss -tulpn
```
Install the systemd unit and Nginx config only after creating the portal service account, virtual environment, paths and TLS configuration. Firewall should allow HTTP/HTTPS to Nginx and SSH only from an admin VPN/bastion; do not expose the database. Capture actual host outputs; they are environment-specific.