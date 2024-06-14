from flask import Flask, jsonify, request
from pymongo import MongoClient
from bson import ObjectId  
from flask_cors import CORS

app = Flask(__name__)
CORS(app, origins=['*'])

MONGO_HOST = 'db'  
MONGO_PORT = 27017
MONGO_DB = 'expense-splitter'
MONGO_COLLECTION = 'expense-groups'

client = MongoClient(MONGO_HOST, MONGO_PORT)
db = client[MONGO_DB]
collection = db[MONGO_COLLECTION]

@app.route('/group', methods=['POST'])
def create_expense_group():
    data = request.json
    collection.insert_one(data)
    return jsonify({"message": "Expense group created successfully!"}), 200

@app.route('/group/<email>', methods=['GET'])
def get_expense_groups_by_email(email):
    documents = collection.find({'user_email': email})
    result = []
    for doc in documents:
        doc['_id'] = str(doc['_id'])
        result.append(doc)
    return jsonify(result)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8003, debug=True)
