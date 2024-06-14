from flask import Flask, jsonify, request
from pymongo import MongoClient
from bson import ObjectId  
from flask_cors import CORS
from datetime import datetime

app = Flask(__name__)
CORS(app, origins=['*'])

MONGO_HOST = 'db'  
MONGO_PORT = 27017
MONGO_DB = 'expense-splitter'
MONGO_COLLECTION = 'expense-groups'

client = MongoClient(MONGO_HOST, MONGO_PORT)
db = client[MONGO_DB]
collection = db[MONGO_COLLECTION]

@app.route('/details/<id>', methods=['GET'])
def get_expense_groups_by_id(id):
    document = collection.find_one({'_id': ObjectId(id)})
    if document:
        document['_id'] = str(document['_id'])
        return jsonify(document)
    else:
        return jsonify({'error': 'Group not found'}), 404
    
@app.route('/details/<id>', methods=['POST'])
def add_expense(id):
    expense_data = request.get_json()
    if not expense_data:
        return jsonify({'error': 'No data provided for the expense'}), 400
    
    document = collection.find_one({'_id': ObjectId(id)})
    if not document:
        return jsonify({'error': 'Group not found'}), 404
    
    expense_name = expense_data.get('name')
    expense_amount = expense_data.get('amount')
    expense_payer = expense_data.get('payer')
    expense_date = expense_data.get('date')    

    
    collection.update_one(
        {'_id': ObjectId(id)},
        {'$push': {'expenses': {'name': expense_name, 'amount': expense_amount, 'payer': expense_payer, 'date': expense_date}}}
    )
    
    return jsonify({'message': 'Expense added successfully'}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8004, debug=True)
