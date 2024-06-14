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

@app.route('/<email>', methods=['GET'])
def get_user_by_email(email):
    document = collection.find_one({'email': email})
    if document:
        document['_id'] = str(document['_id'])
        return jsonify(document)
    else:
        return jsonify({'error': 'User not found'}), 404

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8002, debug=True)
