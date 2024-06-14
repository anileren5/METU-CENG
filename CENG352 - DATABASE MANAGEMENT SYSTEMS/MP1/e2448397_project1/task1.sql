CREATE TABLE IF NOT EXISTS product_category (
    category_id INT,
    name VARCHAR(255),
    PRIMARY KEY (category_id)
);

CREATE TABLE IF NOT EXISTS products (
    product_id CHAR(36),
    name VARCHAR(255),
    category_id INT,
    weight REAL,
    price REAL,
    PRIMARY KEY (product_id),
    FOREIGN KEY (category_id)
        REFERENCES product_category(category_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
); 

CREATE TABLE IF NOT EXISTS customers (
    customer_id CHAR(36),
    name VARCHAR(255),
    surname VARCHAR(255),
    address VARCHAR(1023),
    state CHAR(2),
    gender CHAR(1),
    PRIMARY KEY (customer_id)
);

CREATE TABLE IF NOT EXISTS orders (
    order_id CHAR(36),
    customer_id CHAR(36),
    order_time TIMESTAMP,
    shipping_time TIMESTAMP,
    status VARCHAR(255),
    PRIMARY KEY (order_id),
    FOREIGN KEY (customer_id)
        REFERENCES customers(customer_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS shopping_carts (
    order_id CHAR(36),
    product_id CHAR(36),
    amount INT,
    PRIMARY KEY (order_id, product_id),
    FOREIGN KEY (order_id) 
        REFERENCES orders(order_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (product_id)
        REFERENCES products(product_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS refunds (
    order_id CHAR(36),
    reason VARCHAR(255),
    PRIMARY KEY (order_id),
    FOREIGN KEY (order_id)
        REFERENCES orders(order_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);