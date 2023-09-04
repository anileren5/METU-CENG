SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price
FROM MenuItems MI, Ingredients I, Includes INC
WHERE INC.itemID = MI.itemID
AND INC.ingredientID = I.ingredientID
AND I.ingredientName = ?
ORDER BY MI.itemID ASC;