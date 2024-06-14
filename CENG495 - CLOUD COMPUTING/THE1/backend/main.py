from fastapi import FastAPI, Body 
from fastapi.middleware.cors import CORSMiddleware
from auth0 import *
from mongo import *
import json

app = FastAPI()
db_client = getMongoClient()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
async def health_check():
    return {"status": "Cengden Backend is up and working!"}

@app.get("/users")
async def getAllUsers():
    return getAllUsersInDB(db_client)

@app.get("/users/{user_id}")
async def getUserById(user_id: str):
    return get_user_by_id(user_id)

@app.put("/db/users/{user_id}")
async def updateUserById(user_id: str, request_body: str = Body(...)):
    result = updateUser(db_client, user_id, json.loads(request_body))
    if result:
        return {"message": "Success"}
    else:
        return {"message": "Failed"}

@app.get("/db/users/{user_id}")
async def getUserByIdInDb(user_id: str):
    return getUserByIdDB(db_client, user_id)

@app.get("/users/{user_id}/roles")
async def getUserRolesById(user_id: str):
    return  getUserRoleDB(db_client, user_id)

@app.delete("/users/{user_id}")
async def deleteUserById(user_id: str):
    deleteItemsByUserIdDB(db_client,user_id)
    deleteUserDB(db_client,user_id)
    delete_user_by_id(user_id)

@app.get("/db/users")
async def getAllUsersDB():
    return getAllUsersInDB(db_client)

@app.post("/db/users")
async def registerUserToDb(request_body: str = Body(...)):
    result = addUserIntoDB(db_client, json.loads(request_body))
    if result:
        return {"message": "Success"}
    else:
        return {"message": "Failed"}

@app.post("/items")
async def addItem(request_body: str = Body(...)):
    result = addItemDB(db_client, json.loads(request_body))
    if result:
        return {"message": "Success"}
    else:
        return {"message": "Failed"}

@app.get("/items")
async def getAllItems():
    return getAllItemsInDB(db_client)

@app.get("/users/items/{user_id}")
async def getAllItemsByUserIdDB(user_id: str):
    return getAllItemsByUserIdInDB(db_client, user_id)

@app.put("/items/activate/{item_id}")
async def activate(item_id: str):
    return activateItem(db_client, item_id)

@app.put("/items/deactivate/{item_id}")
async def deactivate(item_id: str):
    return deactivateItem(db_client, item_id)

@app.delete("/items/{item_id}")
async def delete(item_id: str):
    return deleteItem(db_client, item_id)

@app.get("/items/favorites/{user_id}")
async def getUserFavoriteItems(user_id: str):
    return getUserFavoritesDB(db_client, user_id)

@app.post("/items/favorites/{user_id}/{item_id}")
async def addItemToFavorites(user_id: str, item_id: str):
    result = addItemToFavoritesDB(db_client, user_id, item_id)
    if result:
        return {"message": "Success"}
    else:
        return {"message": "Failed"}

@app.delete("/items/favorites/{user_id}/{item_id}")
async def removeItemFromFavorites(user_id: str, item_id: str):
    result = removeItemFromFavoritesDB(db_client, user_id, item_id)
    if result:
        return {"message": "Success"}
    else:
        return {"message": "Failed"}
    
@app.put("/items/{item_id}")
async def updateItem(item_id: str, request_body: str = Body(...)):
    result = updateItemDB(db_client, item_id ,json.loads(request_body))
    if result:
        return {"message": "Success"}
    else:
        return {"message": "Failed"}
    