from flask import Flask, jsonify, render_template
from flask_cors import CORS
import paho.mqtt.client as mqtt
import json

#Initialiserer Flask-applikasjonen
app = Flask(__name__) 

# Aktiverer CORS for å tillate forespørsel fra andre domener
CORS(app)  

#Initialiserer en global variabel for å lagre den siste posisjonen
latest_location = {"lat": 0, "lon": 0}
#Flag for å kun sende én gang
has_sent = False  



def on_message(client, userdata, msg):
    global latest_location, has_sent
    try:
        #Dekoder meldingen som en UTF-8 string
        payload_str = msg.payload.decode("utf-8")
        print(f"[DEBUG] Mottatt melding fra topic '{msg.topic}': {payload_str}")

        #Prøver å laste dataene som JSON
        data = json.loads(payload_str)

        if isinstance(data, dict) and "lat" in data and "lon" in data:
            #Oppdaterer vi posisjonen hvis dataene er et JSON-objekt med lat og lon
            latest_location = data
            print(f"[DEBUG] Oppdatert posisjon: {latest_location}")

            if not has_sent:
                #Hvis flagget has_sent er False, sender vi en melding til gruppe10/elsysprosjekt/sendersub
                print("[DEBUG] Sender 'LED1ON' til gruppe10/elsysprosjekt/sendersub")
                client.publish("gruppe10/elsysprosjekt/sendersub", payload="LED1ON", qos=1, retain=False)
                has_sent = True
        else:
            print("[DEBUG] JSON er ikke en gyldig posisjon.")
    except json.JSONDecodeError:
        #Hvis JSON-dekodingen feiler, skriver vi en feilmelding og sender "LED1OFF"
        print(f"[DEBUG] Ikke gyldig JSON: {payload_str}")
        print("[DEBUG] Sender 'LED1OFF' til gruppe10/elsysprosjekt/sendersub")
        client.publish("gruppe10/elsysprosjekt/sendersub", payload="LED1OFF", qos=1, retain=False)
        has_sent = False
    except Exception as e:
        #Håndterer generelle unntak og logger feil
        print(f"[ERROR] Feil i on_message: {e}")


#Initialiserer en MQTT-klient og kaller på on_message for når meldinger mottas
client = mqtt.Client()
client.on_message = on_message
#Forsøker å koble til MQTT-brokeren test.mosquitto.org på port 1883
client.connect("test.mosquitto.org", 1883, 60)


try:
    #Abonnerer på topicen gruppe10/elsysprosjekt/hovedtopic for å motta meldinger
    client.subscribe("gruppe10/elsysprosjekt/hovedtopic")
    #Starter MQTT-klientens hovedloop for å motta meldinger
    client.loop_start()
except Exception as e:
    #Logger eventuelle feil ved tilkobling til MQTT
    print(f"Feil ved tilkobling til MQTT: {e}")

#Flask-rute som returnerer den siste mottatte posisjonen som JSON
@app.route("/location", methods=["GET"])
def get_location():
    return jsonify(latest_location)


#Flask-rute som returnerer en HTML-side når brukeren besøker URL-en
@app.route("/")
def home():
    return render_template("index.html")

#Slår på automatisk omlasting av maler under utvikling
app.config["TEMPLATES_AUTO_RELOAD"] = True

if __name__ == "__main__":
    #Starter Flask-applikasjonen og gjør den tilgjengelig på alle IP-adresser på port 5000
    app.run(host="0.0.0.0", port=5000, debug=True)
