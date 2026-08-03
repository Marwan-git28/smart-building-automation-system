from flask import Flask, jsonify, render_template
import asyncio
import threading
import BAC0

app = Flask(__name__)
app.config['TEMPLATES_AUTO_RELOAD'] = True

# Variabel Global untuk menampung data sensor terbaru dari ESP32
sensor_data = {
    "temperature": 0.0,
    "humidity": 0.0,
    "smoke": 0.0,
    "status": "Initializing..."
}

# IP ESP32 lu
esp32_ip = "172.25.234.68"

# Fungsi background untuk membaca data BACnet secara terus-menerus
def run_bacnet_loop():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    
    async def fetch_data():
        global sensor_data
        print("Inisialisasi BACnet client untuk REST API & Web...")
        try:
            bacnet = BAC0.lite(ip='172.25.234.149/24')
            print("BAC0 Client berhasil diinisialisasi!")
        except Exception as e:
            print(f"Gagal inisialisasi BAC0: {e}")
            return
        
        while True:
            try:
                print(f"Mencoba membaca data dari ESP32 ({esp32_ip})...")
                temp = await bacnet.read(f"{esp32_ip} analogValue 0 presentValue")
                hum = await bacnet.read(f"{esp32_ip} analogValue 1 presentValue")
                smoke = await bacnet.read(f"{esp32_ip} analogValue 2 presentValue")
                
                sensor_data = {
                    "temperature": round(float(temp), 1),
                    "humidity": round(float(hum), 1),
                    "smoke": round(float(smoke), 0),
                    "status": "ONLINE"
                }
                print("Data berhasil ditarik:", sensor_data)
            except Exception as e:
                print(f"Error saat BACnet read: {e}")
                sensor_data["status"] = f"Error: {str(e)}"
            
            await asyncio.sleep(2)

    loop.run_until_complete(fetch_data())

# Route utama untuk menampilkan Dashboard HTML
@app.route('/')
def index():
    return render_template('dashboard.html')

# Endpoint REST API JSON
@app.route('/api/sensors', methods=['GET'])
def get_sensors():
    return jsonify(sensor_data)

if __name__ == '__main__':
    t = threading.Thread(target=run_bacnet_loop, daemon=True)
    t.start()
    
    app.run(host='0.0.0.0', port=5000, debug=False)