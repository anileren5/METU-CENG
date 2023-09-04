SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price
FROM MenuItems MI
WHERE MI.itemID NOT IN
(
SELECT DISTINCT MI.itemID
FROM MenuItems MI, Includes INC
WHERE MI.itemID = INC.itemID
)
ORDER BY MI.itemID ASC;
