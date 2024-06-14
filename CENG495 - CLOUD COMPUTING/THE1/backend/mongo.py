import certifi
from pymongo.mongo_client import MongoClient
from pymongo.server_api import ServerApi
from bson import ObjectId  # Import ObjectId
from send_grid_mail import *

ca = certifi.where()
uri = "mongodb+srv://anilerengocer495:1234567890@cengden.xpzuvl5.mongodb.net/?retryWrites=true&w=majority&appName=cengden"

def getMongoClient():
    # Create a new client and connect to the server
    client = MongoClient(uri, server_api=ServerApi('1'), tlsCAFile=ca)
    return client

def getAllUsersInDB(client):
    db = client.get_database("cengden")
    collection = db.users   
    all_documents = collection.find({})
    # Convert ObjectId to string for each document
    users = []
    for doc in all_documents:
        doc['_id'] = str(doc['_id'])
        temp = []
        try:
            for itemId in doc["favoriteItems"]:
                temp.append(str(itemId))
            doc["favoriteItems"] = temp
        except:
            pass
        users.append(doc)
    return users

def getUserByIdDB(client,user_id):
    db = client.get_database("cengden")
    collection = db.users
    all_documents = collection.find({"user_id": user_id})
    users = []
    for doc in all_documents:
        doc['_id'] = str(doc['_id'])
        temp = []
        for itemId in doc["favoriteItems"]:
            temp.append(str(itemId))
        doc["favoriteItems"] = temp
        users.append(doc)
    return users[0]

def addUserIntoDB(client, user):
    db = client.get_database("cengden")
    collection = db.users   
    result = collection.insert_one(user)
    return True if result else False

def addItemDB(client, item):
    
    # Filter the empty values and do not save them into MongoDB
    keysWithEmptyValues = []
    for key in item:
        if item[key] == "":
            keysWithEmptyValues.append(key)
    for key in keysWithEmptyValues:
        del item[key]
    

    # Filter the empty values in flatFields and do not save them into MongoDB
    keysWithoutEmptyValuesInFlatFields = []
    for flatField in item["flatFields"]:
        if flatField["fieldValue"] != "":
            keysWithoutEmptyValuesInFlatFields.append(flatField)
    item["flatFields"] = keysWithoutEmptyValuesInFlatFields
        
    # Remove if the nested field name is empty
    nestedFieldsWithoutEmptyName = []
    for nestedField in item["nestedFields"]:
        if nestedField["name"] != "":
            nestedFieldsWithoutEmptyName.append(nestedField)
    item["nestedFields"] = nestedFieldsWithoutEmptyName

    # For each fields in a nestedField, remove the fields with an empty fieldValue
    nestedFieldsWithoutEmptyValueUnderIt = []
    for nestedField in item["nestedFields"]:
        temp = []
        for field in nestedField["fields"]:
            if field["fieldValue"] != "" and field["fieldName"] != "":
                temp.append(field)
        nestedFieldsWithoutEmptyValueUnderIt.append({"name": nestedField["name"], "fields": temp})
    item["nestedFields"] = nestedFieldsWithoutEmptyValueUnderIt

    # Filter the listFields i.e. remove the listField if it has empty name
    listFieldsWithoutEmptyName = []
    for listField in item["listFields"]:
        if len(listField["name"]) != 0:
            listFieldsWithoutEmptyName.append(listField)
    item["listFields"] = listFieldsWithoutEmptyName

    # Filter the empty listItems in listFields
    listFieldsWithoutEmptyListItem = []
    for listField in item["listFields"]:
        nonEmptyItems = []
        for listItem in listField["list"]:
            if len(listItem["value"]) != 0:
                nonEmptyItems.append(listItem)
        listField["list"] = nonEmptyItems
        listFieldsWithoutEmptyListItem.append(listField)
    item["listFields"] = listFieldsWithoutEmptyListItem

    db = client.get_database("cengden")
    collection = db.items 
    result = collection.insert_one(item)
    return True if result else False

def getAllItemsInDB(client):
    db = client.get_database("cengden")
    collection = db.items 
    all_documents = collection.find({}).sort({ "creation_date": -1 }).limit(100)
    # Convert ObjectId to string for each document
    items = []
    for doc in all_documents:
        doc['_id'] = str(doc['_id'])
        items.append(doc)
    return items

def getAllItemsByUserIdInDB(client, user_id):
    db = client.get_database("cengden")
    collection = db.items 
    all_documents = collection.find({"user_id": user_id}).sort({ "creation_date": -1 }).limit(100)
    # Convert ObjectId to string for each document
    items = []
    for doc in all_documents:
        doc['_id'] = str(doc['_id'])
        items.append(doc)
    return items

def activateItem(client, item_id):
    db = client.get_database("cengden")
    collection = db.items 
    result = collection.update_one(
        {"_id": ObjectId(item_id)},
        {"$set": {"isActive": True}}
    )

def deactivateItem(client, item_id):
    db = client.get_database("cengden")
    collection = db.items 
    result = collection.update_one(
        {"_id": ObjectId(item_id)},
        {"$set": {"isActive": False}}
    )

def deleteItem(client, item_id):
    db = client.get_database("cengden")
    collection = db.items 
    result = collection.delete_one({"_id": ObjectId(item_id)})

def deleteItemsByUserIdDB(client, user_id):
    db = client.get_database("cengden")
    collection = db.items 
    result = collection.delete_many({"user_id": user_id})

def deleteUserDB(client, user_id):
    db = client.get_database("cengden")
    collection = db.users
    result = collection.delete_one({"user_id": user_id})

def updateUser(client, user_id, updated_user):
    del updated_user["_id"]
    temp = []
    for itemId in updated_user["favoriteItems"]:
        temp.append(ObjectId(itemId))
    updated_user["favoriteItems"] = temp
    db = client.get_database("cengden")
    collection = db.users
    result = collection.replace_one({"user_id": user_id}, updated_user)
    return result.modified_count == 1

def getUserRoleDB(client, user_id):
    db = client.get_database("cengden")
    collection = db.users
    all_documents = collection.find({"user_id": user_id})
    return {
            "name": all_documents[0]["role"]
        }

def getUserFavoritesDB(client, user_id):
    db = client.get_database("cengden")
    collection = db.users
    result = collection.find({"user_id": user_id}).sort({ "creation_date": -1 }).limit(100)
    users = []
    for doc in result:
        doc['_id'] = str(doc['_id'])
        users.append(doc)

    favoriteItemIds = users[0]["favoriteItems"]
    favoriteItems = []
    for item_id in favoriteItemIds:
        item_collection = db.items
        result = item_collection.find({"_id": ObjectId(item_id)}).sort({ "creation_date": -1 })
        for doc in result:
            doc['_id'] = str(doc['_id'])
            favoriteItems.append(doc)

    favoriteItems = sorted(favoriteItems, key=lambda x: x['creation_date'], reverse=True)
  
    return favoriteItems

def addItemToFavoritesDB(client, user_id, item_id):
    db = client.get_database("cengden")
    collection = db.users
    
    # Update the user document to add the new item to the favoriteItems array
    result = collection.update_one(
        {"user_id": user_id},
        {"$push": {"favoriteItems": ObjectId(item_id)}}
    )

    return result.modified_count > 0

def removeItemFromFavoritesDB(client, user_id, item_id):
    db = client.get_database("cengden")
    collection = db.users
    
    # Update the user document to remove the item from the favoriteItems array
    result = collection.update_one(
        {"user_id": user_id},
        {"$pull": {"favoriteItems": ObjectId(item_id)}}
    )
    
    return result.modified_count > 0

def updateItemDB(client, item_id, updateRequest):
    try:
        db = client.get_database("cengden")
        itemsCollection = db.items 
        updateRequest["updatedItem"]["_id"] = ObjectId(updateRequest["updatedItem"]["_id"])
        itemsCollection.replace_one({'_id': updateRequest["updatedItem"]["_id"]}, updateRequest["updatedItem"])
        
        if updateRequest["updatedItem"]["priceInTermsOfTL"] < updateRequest["old_price"]:
            usersCollection = db.users
            query = {'favoriteItems': {'$in': [ObjectId(updateRequest["updatedItem"]["_id"])]}}
            users = usersCollection.find(query)
            emails = []
            for user in users:
                emails.append(user["email"])    

            sendMail(updateRequest["updatedItem"]["title"], updateRequest["oldPriceInOriginalCurrency"] + " " + updateRequest["oldCurrency"], updateRequest["updatedItem"]["price"] + " " + updateRequest["updatedItem"]["currency"], emails)
            print("Email sent!")

        return True
    except:
        print("Error occured during item update!")
        return False