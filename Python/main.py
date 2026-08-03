import json
import ssl
import threading
from flask import Flask, jsonify, render_template
import paho.mqtt.client as mqtt

# ================= CONFIG MQTT EMQX CLOUD =================
MQTT_BROKER = "bbb5b116.ala.asia-southeast1.emqxsl.com"
MQTT_PORT = 8883
MQTT_USER = "esp32"
MQTT_PASS = "esp32@123"
MQTT_TOPIC = "industrial/modbus"

# Global variable untuk menampung data sensor terbaru
latest_data = {"Tank Level": 0, "Flow Rate": 0, "Pressure": 0}


# ================= MQTT CALLBACK =================
def on_connect(client, userdata, flags, rc):
  if rc == 0:
    print("Berhasil terhubung ke EMQX Cloud Broker!")
    client.subscribe(MQTT_TOPIC)
    print(f"Menunggu data dari topik: {MQTT_TOPIC}...\n")
  else:
    print(f"Gagal konek, kode error: {rc}")


def on_message(client, userdata, msg):
  global latest_data
  try:
    payload_str = msg.payload.decode("utf-8")
    print(f"Pesan Mentah diterima: {payload_str}")

    # Parsing JSON dari ESP32 Master
    data = json.loads(payload_str)

    # Simpan ke variabel global agar bisa diakses REST API Flask
    latest_data["Tank Level"] = data.get("Tank Level", 0)
    latest_data["Flow Rate"] = data.get("Flow Rate", 0)
    latest_data["Pressure"] = data.get("Pressure", 0)

    print("--- DATA SENSOR INDUSTRIAL ---")
    print(f"Tank Level  : {latest_data['Tank Level']}")
    print(f"Flow Rate   : {latest_data['Flow Rate']}")
    print(f"Pressure    : {latest_data['Pressure']}")
    print("------------------------------\n")

  except json.JSONDecodeError:
    print("Gagal parsing JSON dari payload!")


def run_mqtt():
  client = mqtt.Client(
      client_id="Python_Industrial_Dashboard", protocol=mqtt.MQTTv311
  )
  client.username_pw_set(MQTT_USER, MQTT_PASS)
  client.tls_set(cert_reqs=ssl.CERT_NONE)
  client.tls_insecure_set(True)

  client.on_connect = on_connect
  client.on_message = on_message

  client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
  client.loop_forever()


# ================= FLASK WEB & REST API =================
app = Flask(__name__)


@app.route("/")
def index():
  return render_template("dashboard.html")


# REST API Endpoint untuk diambil datanya oleh dashboard.html
@app.route("/api/data")
def get_data():
  return jsonify(
      {
          "tank_level": latest_data.get("Tank Level"),
          "flow_rate": latest_data.get("Flow Rate"),
          "pressure": latest_data.get("Pressure"),
      }
  )


# ================= MAIN PROGRAM =================
if __name__ == "__main__":
  # Jalankan MQTT client di background thread
  mqtt_thread = threading.Thread(target=run_mqtt)
  mqtt_thread.daemon = True
  mqtt_thread.start()

  # Jalankan Flask Web Server
  print("Web Dashboard berjalan di: http://127.0.0.1:5000")
  app.run(host="0.0.0.0", port=5000, debug=True)