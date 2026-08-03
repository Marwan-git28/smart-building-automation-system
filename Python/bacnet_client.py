import asyncio
import BAC0

esp32_ip = "172.25.234.68"

async def main():
    print("Inisialisasi BACnet client...")
    bacnet = BAC0.lite()
    
    print(f"Mencoba memantau 3 Sensor dari ESP32 ({esp32_ip})...\n")
    
    while True:
        try:
            # Membaca 3 Analog Value dari ESP32
            temp  = await bacnet.read(f"{esp32_ip} analogValue 0 presentValue")
            hum   = await bacnet.read(f"{esp32_ip} analogValue 1 presentValue")
            smoke = await bacnet.read(f"{esp32_ip} analogValue 2 presentValue")
            
            # Evaluasi status kondisi sesuai batas aman
            temp_status = "NORMAL (20-35°C)" if 20 <= temp <= 35 else "ABNORMAL (ALERT!)"
            hum_status  = "IDEAL (40-70%)" if 40 <= hum <= 70 else "ABNORMAL (ALERT!)"
            smoke_status = "SAFE (<400 ppm)" if smoke <= 400 else "DANGER (BUZZER ON!)"
            
            # Menampilkan hasil monitoring di terminal VS Code
    
            print(f"1. Temperature : {temp:.1f} °C -> {temp_status}")
            print(f"2. Humidity    : {hum:.1f} %  -> {hum_status}")
            print(f"3. Smoke Level : {smoke:.0f} ppm -> {smoke_status}")
            print("-" * 50)
            
        except Exception as e:
            print(f"Gagal membaca: {e}")
        
        await asyncio.sleep(2)

if __name__ == "__main__":
    asyncio.run(main())