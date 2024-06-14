SELECT
    C.gender,
    to_char(O.order_time, 'Month') AS month,
    SUM(P.price * SC.amount) AS cart_total
FROM
    customers C,
    orders O,
    products P,
    shopping_carts SC
WHERE 
    O.customer_id = C.customer_id AND
    O.order_id = SC.order_id AND
    P.product_id = SC.product_id
GROUP BY CUBE(C.gender, month)
ORDER BY (C.gender, to_char(O.order_time, 'Month'));

SELECT *
FROM crosstab(
    $$
		SELECT
		    C.gender,
		    to_char(O.order_time, 'Month') AS month,
		    SUM(P.price * SC.amount) AS cart_total
		FROM
		    customers C,
		    orders O,
		    products P,
		    shopping_carts SC
		WHERE 
		    O.customer_id = C.customer_id AND
		    O.order_id = SC.order_id AND
		    P.product_id = SC.product_id
		GROUP BY CUBE(C.gender, month)
		ORDER BY (C.gender, to_char(O.order_time, 'Month'))
    $$,
    $$
	    SELECT DISTINCT to_char(O.order_time, 'Month') 
	    AS month FROM orders O 
	    ORDER BY to_char(O.order_time, 'Month')
    $$
) AS ct("gender" text, "February" bigint, "January" bigint, "March" bigint);