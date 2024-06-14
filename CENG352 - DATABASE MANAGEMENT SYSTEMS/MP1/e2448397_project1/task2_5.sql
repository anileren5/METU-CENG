SELECT 
	order_category_1.name AS category1,
	order_category_2.name AS category2,
	COUNT(*) AS total_value
FROM 
	(
		SELECT 
			SC.order_id,
			PC.name
		FROM
			shopping_carts SC,
			products p,
			product_category PC
		WHERE
			SC.product_id = P.product_id AND
			PC.category_id = P.category_id
	) AS order_category_1,
	(
		SELECT 
			SC.order_id,
			PC.name
		FROM
			shopping_carts SC,
			products p,
			product_category PC
		WHERE
			SC.product_id = P.product_id AND
			PC.category_id = P.category_id
	) AS order_category_2,
	orders O
WHERE 
	order_category_1.order_id = order_category_2.order_id AND
	order_category_1.name != order_category_2.name AND
	order_category_1.order_id NOT IN (SELECT R.order_id FROM refunds R) AND
	O.order_id = order_category_1.order_id AND
	O.status != 'CANCELLED' AND
	order_category_1.name < order_category_2.name
GROUP BY (category1, category2)
ORDER BY (total_value) DESC
LIMIT 10;