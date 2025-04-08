from flask import Flask, jsonify, render_template
from flask_cors import CORS
import paho.mqtt.client as mqtt

app = Flask(__name__)
CORS(app)

latest_location = {"lat": 0, "lon": 0}

def on_message(client, userdata, msg):
    global latest_location
    try:
        latest_location = eval(msg.payload.decode("utf-8"))
    except Exception as e:
        print(f"Feil ved dekoding av MQTT-melding: {e}")

client = mqtt.Client()
client.on_message = on_message
client.connect("test.mosquitto.org", 1883, 60)

try:
    client.subscribe("devacademy/krit/topic")
    client.loop_start()
except Exception as e:
    print(f"Feil ved tilkobling til MQTT: {e}")

@app.route("/location", methods=["GET"])
def get_location():
    return jsonify(latest_location)

@app.route("/")
def home():
    return render_template("index.html")  # Server index.html når root URL er besøkt

app.config["TEMPLATES_AUTO_RELOAD"] = True

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)

