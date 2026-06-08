from flask import Flask, render_template, request, jsonify
import serial
import time

app = Flask(__name__)

arduino_connected = False
arduino = None

try:
    # Membuka port serial. Flask yang mengunci COM3, bukan browser!
    arduino = serial.Serial(
        port='COM3',
        baudrate=9600,
        timeout=1
    )
    time.sleep(2)  # Memberi jeda waktu reset otomatis Arduino
    arduino_connected = True
    print("=== ARDUINO BERHASIL TERHUBUNG PADA COM3 ===")

except Exception as e:
    print("=== ARDUINO GAGAL TERHUBUNG ===")
    print(e)


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/control', methods=['POST'])
def control():
    global arduino_connected
    data = request.get_json()
    cmd = data.get('cmd', '')

    serial_cmd = None

    # Pemetaan Command sesuai dengan isi kode Arduino kamu
    if cmd == "MATI_SEMUA":
        serial_cmd = "M"
    elif cmd == "NYALA_SEMUA":
        serial_cmd = "N"
    elif cmd == "DISCO":
        serial_cmd = "D"
    elif cmd == "TRAFFIC":
        serial_cmd = "T"
    elif cmd in ["SIGNAL_0", "SIGNAL_1", "SIGNAL_2", "SIGNAL_3"]:
        # Mengambil angka terakhir saja ('0', '1', '2', atau '3')
        serial_cmd = cmd.split("_")[1]

    # Mengirimkan karakter ke Arduino via Python
    if arduino_connected and serial_cmd:
        try:
            arduino.write(serial_cmd.encode())
            print(f"[WEB API] Command: {cmd} -> Dikirim ke Serial: {serial_cmd}")
            return jsonify({"status": "success", "cmd": cmd, "serial": serial_cmd})
        except Exception as e:
            print("Gagal mengirim data serial:", e)
            arduino_connected = False
            return jsonify({"status": "error", "message": "Koneksi serial terputus"}), 500
    else:
        return jsonify({"status": "failed", "message": "Arduino tidak terdeteksi/command salah"}), 400


@app.route('/status')
def status():
    return jsonify({
        "arduino": arduino_connected
    })


if __name__ == '__main__':
    app.run(
        host='0.0.0.0',
        port=5000,
        debug=False,
        use_reloader=False
    )
