from flask import Flask, jsonify
app = Flask(__name__)

@app.get("/")
def index():
    return jsonify(service="University Result Portal", status="ok")

@app.get("/health")
def health():
    return jsonify(healthy=True), 200