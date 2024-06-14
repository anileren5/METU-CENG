SELECT DISTINCT
    C.customer_id,
    C.gender,
    SUM(P.price * SC.amount) OVER (PARTITION BY C.customer_id)
FROM 
    customers C,
    orders O,
    products P,
    shopping_carts SC,
    refunds R
WHERE 
    O.status = 'COMPLETED' AND
    R.order_id = O.order_id AND
    SC.order_id = O.order_id AND
    SC.product_id = P.product_id AND
    O.customer_id = C.customer_id
ORDER BY C.customer_id ASC;

    