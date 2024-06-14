SELECT
	PC.name AS category,
	P.name AS product_name,
	COUNT(*) refunded_orders,
    rank() OVER (PARTITION BY PC.name ORDER BY COUNT(*) DESC) AS product_rank
FROM 
	products P,
	product_category PC,
	refunds R,
	shopping_carts SC,
	orders O
WHERE 
	P.category_id = PC.category_id AND
	P.product_id = SC.product_id AND
	O.order_id = R.order_id AND
	O.order_id = SC.order_id AND
	R.reason = 'DAMAGED_DELIVERY'
GROUP BY (category, product_name);

