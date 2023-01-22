SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price
FROM MenuItems MI, Includes INC
WHERE MI.itemID = INC.itemID
GROUP BY MI.itemID
HAVING COUNT(*) = (
SELECT MAX(temp.n_ingredients)
FROM (
SELECT COUNT(*) AS n_ingredients
FROM MenuItems MI2, Includes INC2
WHERE MI2.itemID = INC2.itemID
GROUP BY MI2.itemID
) AS temp
)
ORDER BY MI.itemID ASC;