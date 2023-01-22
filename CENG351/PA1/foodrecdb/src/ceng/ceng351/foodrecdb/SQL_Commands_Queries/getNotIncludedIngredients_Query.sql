SELECT DISTINCT I.ingredientID, I.ingredientName
FROM Ingredients I
WHERE I.ingredientID NOT IN
(
SELECT DISTINCT I.ingredientID
FROM Ingredients I, Includes INC
WHERE I.ingredientID = INC.ingredientID
)
ORDER BY I.ingredientID ASC;