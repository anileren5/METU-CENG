CREATE OR REPLACE FUNCTION check_shopping_cart_amount()
RETURNS TRIGGER AS
$func$
BEGIN
    IF NEW.amount <= 0 THEN
        RAISE EXCEPTION 'Amount in shopping cart must be greater than 0';
    END IF;
    
    RETURN NULL;
END
$func$ LANGUAGE plpgsql;

CREATE TRIGGER check_shopping_cart_amount_trigger
BEFORE INSERT ON shopping_carts
FOR EACH ROW
EXECUTE PROCEDURE check_shopping_cart_amount();
