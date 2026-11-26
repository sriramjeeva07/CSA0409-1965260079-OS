# B2 – Containerise the Portal

Build and run:
```sh
docker build -t result-portal .
docker run --rm -p 8000:8000 --cpus=0.5 --memory=512m result-portal
```
Check `http://localhost:8000`, `docker ps`, `docker images`, `docker history result-portal`, `docker exec <container> whoami` (expected user: portal) and `docker stats`.

For Compose, create a local .env file containing DB_PASSWORD=<strong-local-secret>, then run `docker compose up --build -d`. The named pgdata volume persists database files across container recreation. Never commit the .env file.

The slim base reduces image size and packages compared with a full Ubuntu base. Dependencies are installed before copying app source to preserve Docker layer cache. Docker is not available in the authoring execution environment; record actual image size, stats and HTTP test output on the target system.