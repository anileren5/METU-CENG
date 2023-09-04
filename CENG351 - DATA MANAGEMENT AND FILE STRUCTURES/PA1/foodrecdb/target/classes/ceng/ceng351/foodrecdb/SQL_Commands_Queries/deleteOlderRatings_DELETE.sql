SELECT R.ratingID, R.itemID, R.rating, R.ratingDate
FROM Ratings R
WHERE R.ratingDate < ?
ORDER BY R.ratingID ASC;

DELETE FROM Ratings R WHERE R.ratingDate < ?;