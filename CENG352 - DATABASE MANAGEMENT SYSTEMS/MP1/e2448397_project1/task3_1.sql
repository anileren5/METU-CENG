CREATE TABLE IF NOT EXISTS category_avg_prices (
    category_id INT,
    avg_price REAL,
    PRIMARY KEY (category_id),
    FOREIGN KEY (category_id)
        REFERENCES product_category(category_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

INSERT INTO category_avg_prices
SELECT
    P.category_id AS category_id,
    AVG(P.price) AS avg_price
FROM
    products P
GROUP BY (P.category_id)
ORDER BY (P.category_id) ASC;


CREATE OR REPLACE FUNCTION update_avg_price()
RETURNS TRIGGER AS
$func$
BEGIN
    UPDATE category_avg_prices CAP
    SET avg_price = (
        SELECT AVG(P.price) AS avg_price 
        FROM products P
        WHERE P.category_id = NEW.category_id
        GROUP BY P.category_id
    )
    WHERE CAP.category_id = NEW.category_id;
    RETURN NEW; 
END
$func$ LANGUAGE plpgsql;


CREATE TRIGGER avg_price_insert_trigger
AFTER INSERT ON products
FOR EACH ROW
EXECUTE PROCEDURE update_avg_price();
