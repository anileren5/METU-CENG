SELECT DISTINCT MI.itemID, MI.itemName, AVG(R.rating) AS avg_rating
FROM MenuItems MI
LEFT JOIN Ratings R
ON MI.itemID = R.itemID
GROUP BY MI.itemID
ORDER BY avg_rating DESC;