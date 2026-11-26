# A2 – System Administration

Create non-login service accounts portal and dbadmin; human accounts use individual SSH keys and MFA. Groups: portal-ops, db-readonly, sysadmins. Apply least privilege to app, logs and secrets. Run app as systemd service with User=portal, Restart=on-failure, NoNewPrivileges=true, PrivateTmp=true and resource limits. Persist journald logs and forward centrally; alert on 5xx, auth failures, disk pressure and restarts.

Use private RFC1918 addresses for app/database. DNS points to load balancer, not database. Nginx terminates TLS and proxies privately. Firewall default-deny inbound; allow 80/443 to proxy, 22 only from admin VPN/bastion, database port only from app subnet. Disable SSH password/root login; require keys.

Automate consistent backups, checksum verification, restore tests, log rotation and disk/inode alerts with cron/systemd timers. Example hourly log summary: 15 * * * * /opt/ops/log_summary.sh. For one server shell scripts may be quickest; for 20 servers Ansible inventory/roles/idempotency reduce configuration drift and parallelize work, with initial learning/inventory/secret-management overhead.