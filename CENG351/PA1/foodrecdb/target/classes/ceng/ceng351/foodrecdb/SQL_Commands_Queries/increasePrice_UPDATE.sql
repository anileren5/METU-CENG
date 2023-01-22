UPDATE MenuItems MI
SET MI.price = MI.price + ?
WHERE EXISTS (
SELECT *
FROM Includes INC, Ingredients I
WHERE INC.ingredientID = I.ingredientID AND INC.itemID = MI.itemID AND I.ingredientName = ?
);