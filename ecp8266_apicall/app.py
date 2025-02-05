from flask import Flask, request, jsonify, render_template
import json

app = Flask(__name__)

DATA_FILE = "devices.json"

def save_data(data):
    with open(DATA_FILE, "w") as file:
        json.dump(data, file, indent=4)

def load_data():
    try:
        with open(DATA_FILE, "r") as file:
            return json.load(file)
    except FileNotFoundError:
        return {"devices": []}

@app.route("/devices", methods=["POST", "GET"])
def devices():
    if request.method == "POST":
        
        data = request.json

     
        save_data(data)

      
        return jsonify({"status": "success", "message": "Data saved"}), 200
    elif request.method == "GET":
        
        data = load_data()

       
        return jsonify(data), 200

@app.route("/")
def index():
    
    data = load_data()
    return render_template("index.html", devices=data["devices"])

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
