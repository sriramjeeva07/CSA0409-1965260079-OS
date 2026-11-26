FROM python:3.12-slim
WORKDIR /app
RUN useradd --create-home --shell /usr/sbin/nologin portal
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY --chown=portal:portal app.py .
USER portal
EXPOSE 8000
CMD ["gunicorn", "--bind", "0.0.0.0:8000", "app:app"]