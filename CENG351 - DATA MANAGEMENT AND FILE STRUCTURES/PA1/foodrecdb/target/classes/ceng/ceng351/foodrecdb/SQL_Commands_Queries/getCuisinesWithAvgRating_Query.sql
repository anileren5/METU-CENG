SELECT DISTINCT MI.cuisine, AVG(R.rating) AS avg_rating
FROM MenuItems MI
LEFT JOIN Ratings R
ON MI.itemID = R.itemID
GROUP BY MI.cuisine
ORDER BY avg_rating DESC;
