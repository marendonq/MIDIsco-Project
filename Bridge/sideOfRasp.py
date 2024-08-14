import serial
import paho.mqtt.client as mqtt
import time

# Configuración del puerto serial (ajusta el puerto y la velocidad según sea necesario)
serial_port = '/dev/ttyACM0'
baud_rate = 115200

# Configuración de MQTT
mqtt_broker = 'mqtt.eclipseprojects.io'  
mqtt_port = 1883
mqtt_topic = 'arduino/data'

# Inicialización del puerto serial
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Inicialización del cliente MQTT
client = mqtt.Client()

# Conexión al broker MQTT
client.connect(mqtt_broker, mqtt_port, 60)

def send_data_to_mqtt(data):
    client.publish(mqtt_topic, data)

try:
    print("Esperando datos del Arduino...")
    while True:
        # Lee datos binarios desde el Arduino
        if ser.in_waiting > 0:
            data = ser.read(ser.in_waiting)
            if data:
                print(f"Datos recibidos del Arduino: {data}")
                # Envía los datos recibidos a través de MQTT
                send_data_to_mqtt(data)
        # Añade un pequeño retraso para evitar una alta carga de CPU
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Finalizando...")
finally:
    # Cierra la conexión serial y el cliente MQTT
    ser.close()
    client.disconnect()