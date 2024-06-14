const URL = "https://cengden-backend.onrender.com";

async function getUserRolesById(user_id) {
    const response = await fetch(URL + "/users/" + user_id + "/roles") ;
    return await response.json();
}

async function getAllUsers() {
    const response = await fetch(URL + "/users") ;
    return await response.json();
}

async function getUserById(user_id) {
    const response = await fetch(URL + "/users/" + user_id) ;
    return await response.json();
}

async function getUserByIdDB(user_id) {
    const response = await fetch(URL + "/db/users/" + user_id) ;
    return await response.json();
}

async function deleteUserById(user_id) {
    const response = await fetch(URL + "/users/" + user_id, {
        method: 'DELETE'
    });
}

async function addItem(item) {
    const response = await fetch(URL + "/items/", {
        method: 'POST',
        headers: {
            'Content-Type': 'text/plain'
        },
        body: JSON.stringify(item)
    });
    return await response.json();
}

async function getAllItems(){
    const response = await fetch(URL + "/items");
    return await response.json();
}


async function getAllItemsByUserId(user_id){
    const response = await fetch(URL + "/users/items/" + user_id);
    return await response.json();
}

async function activateItem(item_id) {
    try {
        const response = await fetch(URL + "/items/activate/" + item_id, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({})
        });

        if (response.ok) {
            // Handle success
            console.log("Item activated successfully");
        } else {
            // Handle error
            console.error("Failed to activate item:", response.statusText);
        }
    } catch (error) {
        console.error("Error activating item:", error);
    }
}

async function deactivateItem(item_id) {
    try {
        const response = await fetch(URL + "/items/deactivate/" + item_id, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({})
        });

        if (response.ok) {
            console.log("Item activated successfully");
        } else {
            // Handle error
            console.error("Failed to activate item");
        }
    } catch (error) {
        console.error("Error activating item:", error);
    }
}

async function deleteItem(item_id) {
    try {
        const response = await fetch(URL + "/items/" + item_id, {
            method: 'DELETE',
            headers: {
            }
        });
        if (response.ok) {
            console.log("Item deleted successfully");
        } else {
            console.error("Failed to deactivate item:");
        }
    } catch (error) {
        console.error("Error deactivating item:", error);
    }
}

async function getFavorites(user_id) {
    const response = await fetch(URL + "/items/favorites/" + user_id);
    return await response.json();
}

async function addToFavorites(user_id, item_id) {
    const response = await fetch(URL + "/items/favorites/" + user_id + "/" + item_id , {
        method: 'POST',
        headers: {
            'Content-Type': 'text/plain'
        },
        body: ""
    });
    return await response.json();
}

async function removeFromFavorites(user_id, item_id) {
    const response = await fetch(URL + "/items/favorites/" + user_id + "/" + item_id, {
        method: 'DELETE'
    });
    return await response.json();
}

async function updateUserById(user_id, updatedUser) {
    try {
        const response = await fetch(URL + "/db/users/" + user_id, {
            method: 'PUT',
            headers: {
                'Content-Type': 'text/plain'
            },
            body: JSON.stringify(updatedUser)
        });

        if (response.ok) {
            // Handle success
            console.log("User updated successfully");
        } else {
            // Handle error
            console.error("Failed to updateUser", response.message);
        }
    } catch (error) {
        console.error("Error activating item:", error);
    }
}

async function updateItem(item_id, updatedItem) {
    try {
        const response = await fetch(URL + "/items/" + item_id, {
            method: 'PUT',
            headers: {
                'Content-Type': 'text/plain'
            },
            body: JSON.stringify(updatedItem)
        });

        if (response.ok) {
            // Handle success
            console.log("Item updated successfully");
        } else {
            // Handle error
            console.error("Failed to updateItem", response.message);
        }
    } catch (error) {
        console.error("Error updating item:", error);
    }
}



export {
    getUserRolesById,
    getAllUsers,
    deleteUserById,
    addItem,
    getAllItems,
    getAllItemsByUserId,
    activateItem,
    deactivateItem,
    deleteItem,
    getUserById,
    getFavorites,
    getUserByIdDB,
    addToFavorites,
    removeFromFavorites,
    updateUserById,
    updateItem
};
