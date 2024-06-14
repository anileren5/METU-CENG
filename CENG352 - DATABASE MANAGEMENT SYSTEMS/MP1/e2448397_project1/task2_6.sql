SELECT
    product_order_counts.name AS name,
    product_order_counts.product_id AS product_id,
    product_order_counts.total_ordered AS total_ordered,
    (total_ordered - LAG(total_ordered, 1) OVER (ORDER BY total_ordered DESC)) as diff_with_previous
FROM 
    (
        SELECT 
            PC.name AS name,
            P.product_id AS product_id,
            SUM(SC.amount) AS total_ordered
        FROM
            product_category PC,
            products P,
            shopping_carts SC
        WHERE
            P.category_id = PC.category_id AND
            SC.product_id = P.product_id
        GROUP BY (P.product_id, PC.category_id)
        ORDER BY total_ordered DESC        
    ) AS product_order_counts
WHERE product_order_counts.total_ordered >= ALL (
                                                SELECT 
                                                    SUM(SC2.amount) AS total_ordered
                                                FROM
                                                    product_category PC2,
                                                    products P2,
                                                    shopping_carts SC2
                                                WHERE
                                                    P2.category_id = PC2.category_id AND
                                                    SC2.product_id = P2.product_id AND
                                                    product_order_counts.name = PC2.name
                                                GROUP BY (P2.product_id, PC2.category_id)
                                            )
ORDER BY (total_ordered) DESC;