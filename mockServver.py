from flask import Flask, jsonify
import random

app = Flask(__name__)

def generate_random_sensor_data():
    return {
        "x": random.randint(0, 255),
        "y": random.randint(0, 255),
        "z": random.randint(0, 255),
        "t": random.randint(0, 255)
    }

@app.route('/api/heatmap_data', methods=['GET'])
def get_heatmap_data():
    data = {
        "sensor1": generate_random_sensor_data(),
        "sensor2": generate_random_sensor_data(),
        "sensor3": generate_random_sensor_data(),
        # "sensor4": generate_random_sensor_data(),
        # "sensor5": generate_random_sensor_data()
    }
    return jsonify(data)

if __name__ == '__main__':
    app.run(debug=True)
