import psycopg2
import uuid
from datetime import datetime

from config import read_config
from messages import *
from seller import Seller

"""
    Splits given command string by spaces and trims each token.
    Returns token list.
"""
def tokenize_command(command):
    tokens = command.split(" ")
    return [t.strip() for t in tokens]

class Mp2Client:
    def __init__(self, config_filename):
        self.db_conn_params = read_config(filename=config_filename, section="postgresql")
        self.conn = None

    """
        Connects to PostgreSQL database and returns connection object.
    """
    def connect(self):
        self.conn = psycopg2.connect(**self.db_conn_params)
        self.conn.autocommit = False

    """
        Disconnects from PostgreSQL database.
    """
    def disconnect(self):
        self.conn.close()

    """
        Prints list of available commands of the software.
    """
    def help(self):
        # prints the choices for commands and parameters
        print("\n*** Please enter one of the following commands ***")
        print("> help")
        print("> sign_up <seller_id> <password> <plan_id>")
        print("> sign_in <seller_id> <password>")
        print("> sign_out")
        print("> show_plans")
        print("> show_subscription")
        print("> change_stock <product_id> <add or remove> <amount>")
        print("> subscribe <plan_id>")
        print("> ship <order_id>")
        print("> show_cart <customer_id>")
        print("> change_cart <customer_id> <product_id> <seller_id> <add or remove> <amount>")
        print("> purchase_cart <customer_id>")
        print("> quit")
    
    """
        Saves seller with given details.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def sign_up(self, seller_id, password, plan_id):
        # TODO: Implement this function
        query = """
        INSERT INTO sellers(seller_id, password, session_count, plan_id)
        VALUES(%s, %s, %s, %s);
        """
        try:
            cursor = self.conn.cursor()
            cursor.execute(query, (seller_id, password, 0, plan_id))
            self.conn.commit()
            return (True, CMD_EXECUTION_SUCCESS)
        except Exception:
            self.conn.rollback()
            return (False, CMD_EXECUTION_FAILED)

    """
        Retrieves seller information if seller_id and password is correct and seller's session_count < max_parallel_sessions.
        - Return type is a tuple, 1st element is a Seller object and 2nd element is the response message from messages.py.
        - If seller_id or password is wrong, return tuple (None, USER_SIGNIN_FAILED).
        - If session_count < max_parallel_sessions, commit changes (increment session_count) and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If session_count >= max_parallel_sessions, return tuple (None, USER_ALL_SESSIONS_ARE_USED).
        - If any exception occurs; rollback, do nothing on the database and return tuple (None, USER_SIGNIN_FAILED).
    """
    def sign_in(self, seller_id, password):
        # TODO: Implement this function
        fetch_seller_count_query = """
        SELECT COUNT(*) 
        FROM sellers s
        WHERE s.seller_id = %s
        AND s.password = %s ;
        """       
        fetch_current_session_count_query = """
        SELECT s.session_count
        FROM sellers s
        WHERE s.seller_id = %s;
        """ 
        fetch_max_session_count_query = """
        SELECT p.max_parallel_sessions
        FROM sellers s, plans p
        WHERE s.seller_id = %s
        AND s.plan_id = p.plan_id; 
        """
        update_current_session_count_query = """
        UPDATE sellers
        SET session_count = session_count + 1
        WHERE seller_id = %s;
        """
        fetch_seller_query = """
        SELECT s.seller_id, s.session_count, s.plan_id
        FROM sellers s
        WHERE s.seller_id = %s;
        """

        try:
            cursor = self.conn.cursor()
            cursor.execute(fetch_seller_count_query, (seller_id, password))
            self.conn.commit()
            n_user = cursor.fetchall()[0][0]
            if n_user != 1:
                # - If seller_id or password is wrong, return tuple (None, USER_SIGNIN_FAILED).
                return None, USER_SIGNIN_FAILED
            cursor.execute(fetch_current_session_count_query, (seller_id,))
            current_session_count = cursor.fetchall()[0][0]
            cursor.execute(fetch_max_session_count_query, (seller_id,))
            max_session_count = cursor.fetchall()[0][0]
            if current_session_count < max_session_count:
                # - If session_count < max_parallel_sessions, commit changes (increment session_count) and return tuple (seller, CMD_EXECUTION_SUCCESS).
                cursor.execute(update_current_session_count_query, (seller_id,))
                cursor.execute(fetch_seller_query, (seller_id,))
                fetched_seller_id, fetched_session_count, fetched_plan_id = cursor.fetchall()[0]
                self.conn.commit()
                seller_object = Seller(fetched_seller_id, fetched_session_count, fetched_plan_id)
                return seller_object, CMD_EXECUTION_SUCCESS
            else:
                # - If session_count >= max_parallel_sessions, return tuple (None, USER_ALL_SESSIONS_ARE_USED).
                return None, USER_ALL_SESSIONS_ARE_USED

        except Exception:
            # - If any exception occurs; rollback, do nothing on the database and return tuple (None, USER_SIGNIN_FAILED).
            self.conn.rollback()
            return None, USER_SIGNIN_FAILED


    """
        Signs out from given seller's account.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - Decrement session_count of the seller in the database.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def sign_out(self, seller):
        # TODO: Implement this function
        update_current_session_count_query = """
        UPDATE sellers
        SET session_count = session_count - 1
        WHERE seller_id = %s;
        """
        fetch_current_session_count_query = """
        SELECT s.session_count
        FROM sellers s
        WHERE s.seller_id = %s;
        """ 
        try:
            cursor = self.conn.cursor()
            cursor.execute(update_current_session_count_query, (seller.seller_id,))
            cursor.execute(fetch_current_session_count_query, (seller.seller_id,))
            current_session_count = cursor.fetchall()[0][0]
            if current_session_count >= 0:
                # - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
                self.conn.commit()
                return True, CMD_EXECUTION_SUCCESS
            else:
                # - If current_session_count is attempted to be less than 0; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
                self.conn.rollback()
                return False, CMD_EXECUTION_FAILED
        except Exception:
            # - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

    """
        Quits from program.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - Remember to sign authenticated user out first.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def quit(self, seller):
        # TODO: Implement this function
        if seller is None:
            return True, CMD_EXECUTION_SUCCESS
        else:
            try:
                boolean_value, _ = self.sign_out(seller)
                if boolean_value:
                    # - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
                    return True, CMD_EXECUTION_SUCCESS
                else:
                    # - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
                    return False, CMD_EXECUTION_FAILED
            except Exception:
                    # - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
                self.conn.rollback()
                return False, CMD_EXECUTION_FAILED

    """
        Retrieves all available plans and prints them.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print available plans and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        #|Name|Max Sessions
        1|Basic|2
        2|Advanced|4
        3|Premium|6
    """
    def show_plans(self):
        # TODO: Implement this function
        fetch_plans_query = """
        SELECT plan_id, name, max_parallel_sessions
        FROM plans;
        """
        try:
            cursor = self.conn.cursor()
            cursor.execute(fetch_plans_query)
            self.conn.commit()
            plans = cursor.fetchall()
            print("#|Name|Max Sessions")
            for plan in plans:
                print(f"{plan[0]}|{plan[1]}|{plan[2]}")
            return True, CMD_EXECUTION_SUCCESS
        except Exception:
            # - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED
    
    """
        Retrieves plan of the authenticated seller.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print the seller's plan and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        #|Name|Max Sessions
        1|Basic|2
    """
    def show_subscription(self, seller):
        # TODO: Implement this function
        fetch_seller_plan_query = """
        SELECT p.plan_id, p.name, p.max_parallel_sessions
        FROM plans p, sellers s
        WHERE s.plan_id = p.plan_id
        AND s.seller_id = %s;
        """
        try:
            # - If the operation is successful; print the seller's plan and return tuple (True, CMD_EXECUTION_SUCCESS).
            cursor = self.conn.cursor()
            cursor.execute(fetch_seller_plan_query, (seller.seller_id,))
            self.conn.commit()
            plan = cursor.fetchone()
            print("#|Name|Max Sessions")
            print(f"{plan[0]}|{plan[1]}|{plan[2]}")
            return True, CMD_EXECUTION_SUCCESS
        except Exception:
            # - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED
    
    """
        Change stock count of a product.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If new stock value is < 0, return tuple (False, CMD_EXECUTION_FAILED).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def change_stock(self, seller, product_id, change_amount):
        # TODO: Implement this function
        update_stock_query = """
        UPDATE stocks
        SET stock_count = stock_count + %s
        WHERE seller_id = %s
        AND product_id = %s;
        """
        fetch_stock_query = """
        SELECT s.stock_count
        FROM stocks s
        WHERE s.seller_id = %s
        AND s.product_id = %s;
        """
        #try:
        cursor = self.conn.cursor()
        cursor.execute(update_stock_query, (change_amount, seller.seller_id, product_id))
        cursor.execute(fetch_stock_query, (seller.seller_id, product_id))
        stock_count_after_update = cursor.fetchone()[0]
        if stock_count_after_update < 0:
                # - If new stock value is < 0, return tuple (False, CMD_EXECUTION_FAILED)
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED
        else:
                # - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
            self.conn.commit()
            return True, CMD_EXECUTION_SUCCESS
        #except Exception:
            # - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
            #self.conn.rollback()
            #return False, CMD_EXECUTION_FAILED
            

    """
        Subscribe authenticated seller to new plan.
        - Return type is a tuple, 1st element is a Seller object and 2nd element is the response message from messages.py.
        - If the new plan's max_parallel_sessions < current plan's max_parallel_sessions, return tuple (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE).
        - If the operation is successful, commit changes and return tuple (seller, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (None, CMD_EXECUTION_FAILED).
    """
    def subscribe(self, seller, plan_id):
        # TODO: Implement this function
        fetch_current_plan_query = """
        SELECT p.plan_id, p.name, p.max_parallel_sessions
        FROM plans p, sellers s
        WHERE s.plan_id = p.plan_id
        AND s.seller_id = %s;
        """
        fetch_new_plan_query = """
        SELECT p.plan_id, p.name, p.max_parallel_sessions
        FROM plans p
        WHERE p.plan_id = %s;
        """
        update_seller_plan_query = """
        UPDATE sellers
        SET plan_id = %s
        WHERE seller_id = %s;
        """
        fetch_seller_query = """
        SELECT s.seller_id, s.session_count, s.plan_id
        FROM sellers s
        WHERE s.seller_id = %s;
        """

        try:
            cursor = self.conn.cursor()
            cursor.execute(fetch_current_plan_query, (seller.seller_id,))
            current_max_parallel_sessions = cursor.fetchone()[2]
            cursor.execute(fetch_new_plan_query, (plan_id,))
            new_max_parallel_sessions = cursor.fetchone()[2]
            if new_max_parallel_sessions < current_max_parallel_sessions:
                # - If the new plan's max_parallel_sessions < current plan's max_parallel_sessions, return tuple (None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE)
                self.conn.rollback()
                return None, SUBSCRIBE_MAX_PARALLEL_SESSIONS_UNAVAILABLE
            else:
                # - If the operation is successful, commit changes and return tuple (seller, CMD_EXECUTION_SUCCESS).
                cursor.execute(update_seller_plan_query, (plan_id, seller.seller_id))
                cursor.execute(fetch_seller_query, (seller.seller_id,))
                updated_seller = cursor.fetchone()
                seller.seller_id = updated_seller[0]
                seller.session_count = updated_seller[1]
                seller.plan_id = updated_seller[2]
                self.conn.commit()
                return seller, CMD_EXECUTION_SUCCESS
        except Exception as e:
            # - If any exception occurs; rollback, do nothing on the database and return tuple (None, CMD_EXECUTION_FAILED).
            self.conn.rollback()
            return None, CMD_EXECUTION_FAILED


    """
        Change stock amounts of sellers of products included in orders.
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - Check shopping cart of the orders, find products and sellers, then update stocks and order status & shipping time. 
        - If everything is OK and the operation is successful, return (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def ship(self, order_ids):
        # TODO: Implement this function
        check_order_exists_query = """
        SELECT COUNT(*)
        FROM orders o
        WHERE o.order_id = %s;
        """
        fetch_product_seller_count_query = """
        SELECT sc.product_id, sc.seller_id, sc.amount
        FROM shopping_carts sc
        WHERE sc.order_id = %s;
        """
        fetch_stock_count_query = """
        SELECT s.stock_count
        FROM stocks s
        WHERE s.product_id = %s
        AND s.seller_id = %s;
        """
        update_stock_query = """
        UPDATE stocks
        SET stock_count = stock_count - %s
        WHERE product_id = %s
        AND seller_id = %s;
        """
        update_order_status_and_shipping_time_query = """
        UPDATE orders
        SET status = %s,
        shipping_time = %s
        WHERE order_id = %s;
        """

        try:
            cursor = self.conn.cursor()
            for order_id in order_ids:
                cursor.execute(check_order_exists_query, (order_id,))
                result = cursor.fetchone()[0]
                if result != 1: # Order does not exist
                    # - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
                    self.conn.rollback()                    
                    return False, CMD_EXECUTION_FAILED
                cursor.execute(fetch_product_seller_count_query, (order_id,))
                records = cursor.fetchall()
                for record in records:
                    product_id = record[0]
                    seller_id = record[1]
                    amount = record[2]
                    cursor.execute(fetch_stock_count_query, (product_id, seller_id))
                    stock_count = cursor.fetchone()[0]
                    if stock_count < amount:
                        # - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
                        self.conn.rollback()                    
                        return False, CMD_EXECUTION_FAILED
                    cursor.execute(update_stock_query, (amount, product_id, seller_id))
                cursor.execute(update_order_status_and_shipping_time_query, ("SHIPPED", datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3], order_id))
            
            # - If everything is OK and the operation is successful, return (True, CMD_EXECUTION_SUCCESS).
            self.conn.commit()
            return True, CMD_EXECUTION_SUCCESS
        
        except Exception:
            # If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

    """
        Retrieves items on the customer's temporary shopping cart (order status = 'CREATED')
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - If the operation is successful; print items on the cart and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Output should be like:
        Order Id|Seller Id|Product Id|Amount
        orderX|sellerX|productX|3
        orderX|sellerX|productY|1
        orderX|sellerY|productZ|4
    """
    def show_cart(self, customer_id):
        # TODO: Implement this function
        fetch_temporary_shopping_cart_query = """
        SELECT o.order_id, sc.seller_id, sc.product_id, sc.amount 
        FROM orders o, shopping_carts sc 
        WHERE o.customer_id = %s
        AND o.status = %s
        AND sc.order_id  = o.order_id ;        
        """
        try:
            # - If the operation is successful; print items on the cart and return tuple (True, CMD_EXECUTION_SUCCESS).
            cursor = self.conn.cursor()
            cursor.execute(fetch_temporary_shopping_cart_query, (customer_id, "CREATED"))
            records = cursor.fetchall()
            print("Order Id|Seller Id|Product Id|Amount")
            for record in records:
                print(f"{record[0]}|{record[1]}|{record[2]}|{record[3]}")
            self.conn.commit()
            return True, CMD_EXECUTION_SUCCESS
        except Exception as e:
            # - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

    """
        Change count of items in temporary shopping cart (order status = 'CREATED')
        - Return type is a tuple, 1st element is boolean and 2nd element is the response message from messages.py.
        - Consider stocks of sellers when you add items to the cart, in case stock is not enough, return (False, STOCK_UNAVAILABLE).
        - Consider weight limit per order, 15 kilograms. return (False, WEIGHT_LIMIT) if it is reached for the whole order.
        - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED).
    """
    def change_cart(self, customer_id, product_id, seller_id, change_amount):
        # TODO: Implement this function
        check_shopping_cart_exist_query = """
        SELECT COUNT(*)
        FROM orders o
        WHERE o.customer_id = %s
        AND o.status = %s;
        """
        create_shopping_cart_query = """
        INSERT INTO orders(order_id, customer_id, order_time, shipping_time, status)
        VALUES (%s, %s, %s, %s, %s);        
        """
        fetch_shopping_cart_order_id_query = """
        SELECT o.order_id
        FROM orders o
        WHERE o.customer_id = %s
        AND o.status = %s;
        """
        check_if_product_is_in_cart = """
        SELECT COUNT(*)
        FROM shopping_carts sc
        WHERE sc.order_id = %s
        AND sc.product_id = %s
        AND sc.seller_id = %s;
        """
        fetch_stock_count_query = """
        SELECT s.stock_count
        FROM stocks s
        WHERE s.product_id = %s
        AND s.seller_id = %s;
        """
        create_new_item_in_the_cart_query = """
        INSERT INTO shopping_carts(order_id, product_id, seller_id, amount)
        VALUES (%s, %s, %s, %s);
        """
        update_item_count_in_the_cart_query = """
        UPDATE shopping_carts
        SET amount = amount + %s
        WHERE order_id = %s
        AND product_id = %s
        AND seller_id = %s;
        """
        fetch_cart_weight_query = """
        SELECT SUM(sc.amount * p.weight)
        FROM shopping_carts sc , products p  
        WHERE sc.order_id  = %s
        AND sc.product_id = p.product_id;
        """
        fetch_current_product_count_in_the_cart = """
        SELECT amount
        FROM shopping_carts 
        WHERE order_id = %s
        AND product_id = %s
        AND seller_id  = %s;
        """
        delete_product_from_the_cart = """
        DELETE FROM shopping_carts
        WHERE order_id = %s
        AND product_id = %s
        AND seller_id = %s;
        """

        try:
            cursor = self.conn.cursor()
            cursor.execute(check_shopping_cart_exist_query, (customer_id, "CREATED"))
            result = cursor.fetchone()[0]
            if result == 0: # An order with CREATED status does not exist for that customer
                cursor.execute(create_shopping_cart_query, (str(uuid.uuid4()), customer_id, None, None, "CREATED"))
            cursor.execute(fetch_shopping_cart_order_id_query, (customer_id, "CREATED"))
            order_id = cursor.fetchone()[0]

            cursor.execute(check_if_product_is_in_cart, (order_id, product_id, seller_id))
            result = cursor.fetchone()[0]
            if result == 0: # Item is not in the cart
                if change_amount > 0: # add
                    cursor.execute(fetch_stock_count_query, (product_id, seller_id))
                    stock_count = cursor.fetchone()[0]
                    if change_amount > stock_count:
                        # - Consider stocks of sellers when you add items to the cart, in case stock is not enough, return (False, STOCK_UNAVAILABLE).
                        self.conn.rollback()
                        return False, STOCK_UNAVAILABLE
                    else:
                        cursor.execute(create_new_item_in_the_cart_query, (order_id, product_id, seller_id, change_amount))
                        cursor.execute(fetch_cart_weight_query, (order_id,))
                        cart_weight = cursor.fetchone()[0]
                        if cart_weight > 15:
                            # - Consider weight limit per order, 15 kilograms. return (False, WEIGHT_LIMIT) if it is reached for the whole order.
                            self.conn.rollback()
                            return False, WEIGHT_LIMIT
                        else:
                            # - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
                            self.conn.commit()
                            return True, CMD_EXECUTION_SUCCESS
                else: # remove
                    # This case is not natural (trying to remove item even if it does not exists)
                    self.conn.rollback()
                    return False, CMD_EXECUTION_FAILED
            else: # Item is in the cart
                if change_amount > 0:
                    cursor.execute(fetch_stock_count_query, (product_id, seller_id))
                    stock_count = cursor.fetchone()[0]
                    cursor.execute(fetch_current_product_count_in_the_cart, (order_id, product_id, seller_id))
                    current_count_in_cart = cursor.fetchone()[0]
                    if change_amount + current_count_in_cart > stock_count:
                        # - Consider stocks of sellers when you add items to the cart, in case stock is not enough, return (False, STOCK_UNAVAILABLE).
                        self.conn.rollback()
                        return False, STOCK_UNAVAILABLE
                    else:
                        cursor.execute(update_item_count_in_the_cart_query, (change_amount, order_id, product_id, seller_id))
                        cursor.execute(fetch_cart_weight_query, (order_id,))
                        cart_weight = cursor.fetchone()[0]
                        if cart_weight > 15:
                            # - Consider weight limit per order, 15 kilograms. return (False, WEIGHT_LIMIT) if it is reached for the whole order.
                            self.conn.rollback()
                            return False, WEIGHT_LIMIT
                        else:
                            # - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
                            self.conn.commit()
                            return True, CMD_EXECUTION_SUCCESS
                else:
                    cursor.execute(fetch_current_product_count_in_the_cart, (order_id, product_id, seller_id))
                    current_count_in_cart = cursor.fetchone()[0]
                    if abs(change_amount) >= current_count_in_cart: # Delete the item from the cart
                        # - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
                        cursor.execute(delete_product_from_the_cart, (order_id, product_id, seller_id))
                        self.conn.commit()
                        return True, CMD_EXECUTION_SUCCESS
                    else: # Just update the current count by removing
                        # - If the operation is successful, commit changes and return tuple (True, CMD_EXECUTION_SUCCESS).
                        cursor.execute(update_item_count_in_the_cart_query, (change_amount, order_id, product_id, seller_id))
                        self.conn.commit()
                        return True, CMD_EXECUTION_SUCCESS
        except Exception:
            # - If any exception occurs; rollback, do nothing on the database and return tuple (False, CMD_EXECUTION_FAILED)
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED

    """
        Purchases items on the cart
        - Return type is a tuple, 1st element is a boolean and 2nd element is the response message from messages.py.
        - When there are no items to purchase, return (False, EMPTY_CART).
        - Consider stocks of sellers when you purchase the cart, in case stock is not enough, return (False, STOCK_UNAVAILABLE).
        - If the operation is successful; return tuple (True, CMD_EXECUTION_SUCCESS).
        - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
        
        Actions:
        - Change stocks on stocks table
        - Update order with status='CREATED' -> status='RECEIVED' and put order_time with current datetime.
    """
    def purchase_cart(self, customer_id):
        # TODO: Implement this function
        check_shopping_cart_exist_query = """
        SELECT COUNT(*)
        FROM orders o
        WHERE o.customer_id = %s
        AND o.status = %s;
        """
        fetch_number_of_entries_in_the_cart_query = """
        SELECT COUNT(*)
        FROM shopping_carts sc, orders o
        WHERE o.customer_id = %s
        AND o.status = %s
        AND sc.order_id = o.order_id;
        """
        fetch_cart_query = """
        SELECT sc.product_id, sc.seller_id, sc.amount
        FROM shopping_carts sc, orders o
        WHERE o.customer_id = %s
        AND o.status = 'CREATED'
        AND sc.order_id = o.order_id;
        """
        fetch_stock_count_query = """
        SELECT s.stock_count
        FROM stocks s
        WHERE s.product_id = %s
        AND s.seller_id = %s;
        """
        update_stock_count_query = """
        UPDATE stocks
        SET stock_count = stock_count - %s
        WHERE product_id = %s
        AND seller_id = %s;
        """
        update_order_status = """
        UPDATE orders
        SET status = %s,
        order_time = %s
        WHERE status = %s
        AND customer_id = %s;
        """

        try:
            cursor = self.conn.cursor()
            cursor.execute(check_shopping_cart_exist_query, (customer_id, "CREATED"))
            result = cursor.fetchone()[0]
            if result == 0: # An order with CREATED status does not exist for that customer
                # - When there are no items to purchase, return (False, EMPTY_CART).
                self.conn.rollback()
                return False, EMPTY_CART
            cursor.execute(fetch_number_of_entries_in_the_cart_query, (customer_id, "CREATED"))
            result = cursor.fetchone()[0]
            if result == 0: # An order with CREATED status exist for that customer but there is no entry in the cart
                # - When there are no items to purchase, return (False, EMPTY_CART).
                self.conn.rollback()
                return False, EMPTY_CART
            cursor.execute(fetch_cart_query, (customer_id,))
            records = cursor.fetchall()
            for record in records:
                product_id = record[0]
                seller_id = record[1]
                amount = record[2]
                cursor.execute(fetch_stock_count_query, (product_id, seller_id))
                stock_count = cursor.fetchone()[0]
                if amount > stock_count:
                    # - Consider stocks of sellers when you purchase the cart, in case stock is not enough, return (False, STOCK_UNAVAILABLE).
                    self.conn.rollback()
                    return False, STOCK_UNAVAILABLE
                else:
                    cursor.execute(update_stock_count_query, (amount, product_id, seller_id))
            cursor.execute(update_order_status, ("RECEIVED", datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3], "CREATED", customer_id))
            self.conn.commit()
            # - If the operation is successful; return tuple (True, CMD_EXECUTION_SUCCESS).
            return True, CMD_EXECUTION_SUCCESS
        except Exception as e:
            # - If any exception occurs; return tuple (False, CMD_EXECUTION_FAILED).
            self.conn.rollback()
            return False, CMD_EXECUTION_FAILED
        