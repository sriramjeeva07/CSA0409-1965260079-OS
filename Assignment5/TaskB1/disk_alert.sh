#!/usr/bin/env bash
set -euo pipefail
THRESHOLD=80
df -P -x tmpfs -x devtmpfs | awk -v limit="$THRESHOLD" 'NR>1 {gsub(/%/,"",$5); if ($5+0 >= limit) printf "WARNING: %s is %s%% full (mounted on %s)\n",$1,$5,$6}'