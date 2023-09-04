CREATE TABLE IF NOT EXISTS MenuItems(
    itemID INT,
    itemName VARCHAR(40),
    cuisine VARCHAR(20),
    price INT,
    PRIMARY KEY (itemID)
);

CREATE TABLE IF NOT EXISTS Ingredients(
    ingredientID INT,
    ingredientName VARCHAR(40),
    PRIMARY KEY (ingredientID)
);

CREATE TABLE IF NOT EXISTS Includes(
    itemID INT,
    ingredientID INT,
    PRIMARY KEY (itemID, ingredientID),
    FOREIGN KEY (itemID) REFERENCES MenuItems(itemID) ON DELETE CASCADE,
    FOREIGN KEY (ingredientID) REFERENCES Ingredients(ingredientID) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS Ratings(
    ratingID INT,
    itemID INT,
    rating INT,
    ratingDate DATE,
    PRIMARY KEY (ratingID),
    FOREIGN KEY (itemID) REFERENCES MenuItems(itemID) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS DietaryCategories(
    ingredientID INT,
    dietaryCategory VARCHAR(20),
    PRIMARY KEY (ingredientID, dietaryCategory),
    FOREIGN KEY (ingredientID) REFERENCES Ingredients(ingredientID) ON DELETE CASCADE
);