/*
    I choose MATERIALIZED VIEW. Because there are too many rows in this view,
    it is too costly to calculate this view when a query is received, so it takes
    too much time. As a result of MATERIALIZED VIEW, this view is created once and
    then we regularly update it. Seeing a customer orders is not a task in which
    string consistency is very important, so eventual consistency provided by 
    MATERIALIZED VIEW is enough and a bit delayed data won't be problem.
*/

CREATE MATERIALIZED VIEW customers_view AS
SELECT 
    C.customer_id AS customer_id,
    C.name AS name,
    C.surname AS surname,
    O.order_id AS order_id,
    O.order_time AS order_time,
    O.shipping_time AS shipping_time,
    O.status AS status,
    P.product_id AS product_id,
    P.name AS product_name,
    SC.amount AS amount,
    P.price AS price
FROM 
    customers C,
    orders O,
    products P,
    shopping_carts SC
WHERE 
    O.customer_id = C.customer_id AND
    O.order_id = SC.order_id AND
    P.product_id = SC.product_id;
