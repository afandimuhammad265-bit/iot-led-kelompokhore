from flask import Flask, render_template, request, jsonify
import serial
import threading

app = Flask(__name__)

try:
    arduino = serial.Serial('COM4', 9600, timeout=1)
    arduino_connected = True
except:
    arduino_connected = False
    print("Arduino tidak terdeteksi, mode simulasi aktif")

ldr_value = 0
led_status = {"LED1": False, "LED2": False, "LED3": False, "LED4": False}

def read_serial():
    global ldr_value
    while arduino_connected:
        try:
            line = arduino.readline().decode().strip()
            if line.startswith("LDR:"):
                ldr_value = int(line.split(":")[1])
        except:
            pass

if arduino_connected:
    thread = threading.Thread(target=read_serial, daemon=True)
    thread.start()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/control', methods=['POST'])
def control():
    cmd = request.json.get('cmd', '')
    led = request.json.get('led', '')
    state = request.json.get('state', False)
    if arduino_connected:
        arduino.write((cmd + '\n').encode())
    led_status[led] = state
    return jsonify({"status": "ok", "cmd": cmd})

@app.route('/status')
def status():
    return jsonify({"ldr": ldr_value, "leds": led_status, "arduino": arduino_connected})

if __name__ == '__main__':
    app.run(debug=True)