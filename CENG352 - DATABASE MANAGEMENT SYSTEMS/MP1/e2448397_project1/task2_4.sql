SELECT 
    TRIM(day_counts.day_text) AS weekday,
    daily_revenues.total_revenue / day_counts.day_count AS avg_daily_revenue
FROM 
    (
        SELECT 
            day_counts.day_text AS day_text,
            COUNT(*) AS day_count
        FROM (
                SELECT
                    to_char(O.order_time, 'YYYY') AS year,
                    to_char(O.order_time, 'MM') AS month,
                    to_char(O.order_time, 'DD') AS day_numeric,
                    to_char(O.order_time, 'Day') AS day_text
                FROM 
                    orders O
                GROUP BY (year, month, day_numeric, day_text)
                ORDER BY (to_char(O.order_time, 'MM'), to_char(O.order_time, 'DD')) ASC
            ) day_counts
        GROUP BY day_counts.day_text
    ) day_counts,
    (
        SELECT
            to_char(O.order_time, 'Day') AS day_text,
            SUM(P.price * SC.amount) AS total_revenue
        FROM 
            orders O,
            products P,
            shopping_carts SC
        WHERE 
            O.order_id NOT IN (SELECT R.order_id FROM refunds R) AND
            O.status != 'CANCELLED' AND
            SC.order_id = O.order_id AND
            SC.product_id = P.product_id
        GROUP BY (day_text)
        ORDER BY (day_text)
    ) daily_revenues
WHERE
    day_counts.day_text = daily_revenues.day_text
ORDER BY (avg_daily_revenue) DESC 
LIMIT 3;

