SELECT temp.cuisine, AVG(temp.ingredientCount) AS averageCount
FROM
(
SELECT DISTINCT MI.itemID AS itemID, MI.cuisine AS cuisine, CASE COUNT(*) WHEN 1 THEN 0 ELSE COUNT(*) END AS ingredientCount
FROM MenuItems MI
LEFT JOIN Includes INC
ON MI.itemID = INC.itemID
GROUP BY MI.itemID
ORDER BY ingredientCount
) AS temp
GROUP BY temp.cuisine
ORDER BY averageCount DESC;
