from flask import Flask, jsonify, request
from pymongo import MongoClient
from bson import ObjectId  
from flask_cors import CORS

app = Flask(__name__)
CORS(app, origins=['*'])

MONGO_HOST = 'db'  
MONGO_PORT = 27017
MONGO_DB = 'expense-splitter'
MONGO_COLLECTION = 'users'

client = MongoClient(MONGO_HOST, MONGO_PORT)
db = client[MONGO_DB]
collection = db[MONGO_COLLECTION]

@app.route('/sign-up', methods=['POST'])
def sign_up():
    data = request.json
    n = collection.insert_one(data)
    return jsonify({"message": "User created successfully!"}), 200

@app.route('/login', methods=['POST'])
def login():
    data = request.json
    user = collection.find_one({"email":data["email"]})
    if user:
        return jsonify({"message": "User found successfully!"}), 200
    else:
        return jsonify({"message": "User not found!"}), 404

@app.route('/')
def get_users():
    documents = collection.find({})
    result = []
    for doc in documents:
        doc['_id'] = str(doc['_id'])
        result.append(doc)
    return jsonify(result)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8001, debug=True)
