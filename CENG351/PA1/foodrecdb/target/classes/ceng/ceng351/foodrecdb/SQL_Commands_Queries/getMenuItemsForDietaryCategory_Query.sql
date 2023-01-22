SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price
FROM MenuItems MI
WHERE NOT EXISTS (
SELECT I.ingredientID
FROM Ingredients I, Includes INC
WHERE INC.ingredientID = I.ingredientID
AND INC.itemID = MI.itemID
AND I.ingredientID NOT IN (
SELECT DC.ingredientID
FROM DietaryCategories DC
WHERE DC.dietaryCategory = ?
)
) AND
EXISTS (
SELECT INC.ingredientID
FROM Includes INC
WHERE INC.itemID = MI.itemID
)
ORDER BY MI.itemID ASC;