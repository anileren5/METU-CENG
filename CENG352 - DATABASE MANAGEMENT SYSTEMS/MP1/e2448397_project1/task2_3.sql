SELECT 
    categories_fraud_counts.name AS name,
    categories_fraud_counts.fraud_count AS fraud_count
FROM 
    (
        SELECT 
            PC.name AS name,
            COUNT(*) AS fraud_count,
            NTILE(4) OVER (ORDER BY COUNT(*) DESC) AS ntile_value
        FROM
            orders O,
            refunds R,
            products P,
            product_category PC,
            shopping_carts SC
        WHERE 
            R.reason = 'FRAUD_SUSPICION' AND
            R.order_id = O.order_id AND
            P.category_id = PC.category_id AND
            P.product_id = SC.product_id AND
            O.order_id = SC.order_id
        GROUP BY (PC.name)
        ORDER BY fraud_count DESC
    ) AS categories_fraud_counts
WHERE
    categories_fraud_counts.ntile_value = 1;