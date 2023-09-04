SELECT DISTINCT I.ingredientID, I.ingredientName
FROM Ingredients I, Includes INC
WHERE I.ingredientID = INC.ingredientID
GROUP BY I.ingredientID
HAVING COUNT(*) = (
SELECT MAX(temp.n_use)
FROM (
SELECT COUNT(*) AS n_use
FROM Ingredients I2, Includes INC2
WHERE I2.ingredientID = INC2.ingredientID
GROUP BY I2.ingredientID
) AS temp
)
ORDER BY I.ingredientID ASC;