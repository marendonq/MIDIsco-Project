import paho.mqtt.client as mqtt
import mido
from mido import Message

# Configuración de MQTT
mqtt_broker = 'mqtt.eclipseprojects.io'
mqtt_port = 1883
mqtt_topic = 'arduino/data'

# Nombre del puerto LoopMIDI (ajusta según sea necesario)
loopmidi_port_name = 'MIDIport 2'

# Listar los puertos MIDI disponibles
print("Puertos MIDI disponibles:")
print(mido.get_output_names())

# Callback que se llama cuando el cliente recibe un mensaje
def on_message(client, userdata, message):
    data = message.payload  # Datos binarios recibidos
    print(f"Datos recibidos: {data}")
    
    try:
        # Suponiendo que data es un bytearray que representa un mensaje MIDI.
        if len(data) >= 3:
            midi_status = data[0]
            midi_note = data[1]
            midi_velocity = data[2]

            if midi_status == 0x90:  # Nota ON
                note_on = Message('note_on', note=midi_note, velocity=midi_velocity)
                outport.send(note_on)
            elif midi_status == 0x80:  # Nota OFF
                note_off = Message('note_off', note=midi_note, velocity=midi_velocity)
                outport.send(note_off)
    except Exception as e:
        print(f"Error al enviar mensaje MIDI: {e}")

# Inicialización del cliente MQTT
client = mqtt.Client()

# Configuración del callback para el evento de mensaje recibido
client.on_message = on_message

# Abre el puerto MIDI de LoopMIDI
try:
    outport = mido.open_output(loopmidi_port_name)
except Exception as e:
    print(f"Error al abrir el puerto MIDI: {e}")
    exit(1)

try:
    # Conexión al broker MQTT
    print(f"Conectando al broker MQTT en {mqtt_broker}:{mqtt_port}...")
    client.connect(mqtt_broker, mqtt_port, 300)
except Exception as e:
    print(f"Error al conectar con el broker MQTT: {e}")
    exit(1)

# Suscripción al tema
client.subscribe(mqtt_topic)

try:
    # Mantén el cliente en funcionamiento para recibir mensajes
    print("Esperando datos del MQTT...")
    client.loop_forever()
except KeyboardInterrupt:
    print("Finalizando...")
finally:
    client.disconnect()
    outport.close()
