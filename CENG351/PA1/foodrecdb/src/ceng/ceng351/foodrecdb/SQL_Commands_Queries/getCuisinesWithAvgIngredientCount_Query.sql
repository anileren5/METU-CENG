SELECT DISTINCT MI.cuisine, 0 AS averageIngredientCount
FROM MenuItems MI
WHERE NOT EXISTS (
SELECT *
FROM MenuItems MI2, Includes INC
WHERE MI2.itemID = INC.itemID AND MI2.cuisine = MI.cuisine
)
UNION
SELECT temp1.cuisineName1, (ingredientCount/itemCount) AS averageIngredientCount
FROM
(
SELECT DISTINCT MI.cuisine AS cuisineName1, COUNT(*) AS itemCount
FROM MenuItems MI
GROUP BY MI.cuisine
) AS temp1,
(
SELECT DISTINCT MI.cuisine as cuisineName2, COUNT(*) AS ingredientCount
FROM MenuItems MI, Includes INC
WHERE INC.itemID = MI.itemID
GROUP BY MI.cuisine
) AS temp2
WHERE temp1.cuisineName1 = temp2.cuisineName2
ORDER BY averageIngredientCount DESC;

