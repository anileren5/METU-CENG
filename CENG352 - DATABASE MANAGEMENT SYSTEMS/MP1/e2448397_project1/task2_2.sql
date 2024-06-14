SELECT DISTINCT
    order_prices.customer_id AS customer_id,
    order_prices.year AS year,
    order_prices.month AS month,
    order_prices.order_price AS cart_total
FROM 
    (
        SELECT DISTINCT
            CAST(order_prices.year AS INTEGER) AS year,
            CAST(order_prices.month AS INTEGER) AS month,
            MAX(order_prices.order_price) OVER (PARTITION BY order_prices.year, order_prices.month) AS max_price
        FROM
            (
                SELECT 
                    O2.customer_id AS customer_id,
                    CAST(to_char(O2.order_time, 'YYYY') AS INTEGER) AS year,
                    CAST(to_char(O2.order_time, 'MM') AS INTEGER) AS month,
                    SUM(P2.price * SC2.amount) OVER (PARTITION BY SC2.order_id) AS order_price
                FROM
                    orders O2,
                    shopping_carts SC2,
                    products P2
                WHERE 
                    O2.order_id = SC2.order_id AND
                    P2.product_id = SC2.product_id
            ) AS order_prices
    ) AS max_order_prices,
    (
        SELECT 
            O3.customer_id AS customer_id,
            CAST(to_char(O3.order_time, 'YYYY') AS INTEGER) AS year,
            CAST(to_char(O3.order_time, 'MM') AS INTEGER) AS month,
            SUM(P3.price * SC3.amount) OVER (PARTITION BY SC3.order_id) AS order_price
        FROM
            orders O3,
            shopping_carts SC3,
            products P3
        WHERE 
            O3.order_id = SC3.order_id AND
            P3.product_id = SC3.product_id
    ) AS order_prices
WHERE
    max_order_prices.year = order_prices.year AND
    max_order_prices.month = order_prices.month AND
    max_order_prices.max_price = order_prices.order_price
ORDER BY month ASC;
