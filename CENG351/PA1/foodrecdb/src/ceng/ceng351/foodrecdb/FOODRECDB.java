package ceng.ceng351.foodrecdb;

import java.sql.*;
import java.util.ArrayList;

public class FOODRECDB implements IFOODRECDB{

    // Credentials for database connection
    private static String USER = "e2448397";
    private static String PASSWORD = "rfSFxYy5r-IlM529";
    private static String HOST = "momcorp.ceng.metu.edu.tr";
    private static String DATABASE = "db2448397";
    private static int PORT = 8080;

    private static Connection CONNECTION = null;


    @Override
    public void initialize(){
        String ENDPOINT = "jdbc:mysql://" + HOST + ":" + PORT + "/" + DATABASE + "?useSSL=false";

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            CONNECTION =  DriverManager.getConnection(ENDPOINT, USER, PASSWORD);
            dropTables();
        }
        catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int createTables(){
        int numberOfCreatedTables = 0;

        String CREATE_MENUITEMS = "CREATE TABLE IF NOT EXISTS MenuItems(\n" +
                "    itemID INT,\n" +
                "    itemName VARCHAR(40),\n" +
                "    cuisine VARCHAR(20),\n" +
                "    price INT,\n" +
                "    PRIMARY KEY (itemID)\n" +
                ");";

        String CREATE_INGREDIENTS = "CREATE TABLE IF NOT EXISTS Ingredients(\n" +
                "    ingredientID INT,\n" +
                "    ingredientName VARCHAR(40),\n" +
                "    PRIMARY KEY (ingredientID)\n" +
                ");";

        String CREATE_INCLUDES = "CREATE TABLE IF NOT EXISTS Includes(\n" +
                "    itemID INT,\n" +
                "    ingredientID INT,\n" +
                "    PRIMARY KEY (itemID, ingredientID),\n" +
                "    FOREIGN KEY (itemID) REFERENCES MenuItems(itemID) ON DELETE CASCADE,\n" +
                "    FOREIGN KEY (ingredientID) REFERENCES Ingredients(ingredientID) ON DELETE CASCADE\n" +
                ");";

        String CREATE_RATINGS = "CREATE TABLE IF NOT EXISTS Ratings(\n" +
                "    ratingID INT,\n" +
                "    itemID INT,\n" +
                "    rating INT,\n" +
                "    ratingDate DATE,\n" +
                "    PRIMARY KEY (ratingID),\n" +
                "    FOREIGN KEY (itemID) REFERENCES MenuItems(itemID) ON DELETE CASCADE\n" +
                ");";

        String CREATE_DIETARY_CATEGORIES = "CREATE TABLE IF NOT EXISTS DietaryCategories(\n" +
                "    ingredientID INT,\n" +
                "    dietaryCategory VARCHAR(20),\n" +
                "    PRIMARY KEY (ingredientID, dietaryCategory),\n" +
                "    FOREIGN KEY (ingredientID) REFERENCES Ingredients(ingredientID) ON DELETE CASCADE\n" +
                ");";

        try{
            CONNECTION.prepareStatement(CREATE_MENUITEMS).execute();
            numberOfCreatedTables++;
            CONNECTION.prepareStatement(CREATE_INGREDIENTS).execute();
            numberOfCreatedTables++;
            CONNECTION.prepareStatement(CREATE_INCLUDES).execute();
            numberOfCreatedTables++;
            CONNECTION.prepareStatement(CREATE_RATINGS).execute();
            numberOfCreatedTables++;
            CONNECTION.prepareStatement(CREATE_DIETARY_CATEGORIES).execute();
            numberOfCreatedTables++;
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return numberOfCreatedTables;
    }

    @Override
    public int dropTables(){
        int numberOfDroppedTables = 0;

        try{
            String DROP_DIETARY_CATEGORIES = "DROP TABLE IF EXISTS DietaryCategories;";
            CONNECTION.prepareStatement(DROP_DIETARY_CATEGORIES).execute();
            numberOfDroppedTables++;

            String DROP_RATINGS = "DROP TABLE IF EXISTS Ratings;";
            CONNECTION.prepareStatement(DROP_RATINGS).execute();
            numberOfDroppedTables++;

            String DROP_INCLUDES = "DROP TABLE IF EXISTS Includes;";
            CONNECTION.prepareStatement(DROP_INCLUDES).execute();
            numberOfDroppedTables++;

            String DROP_MENUITEMS = "DROP TABLE IF EXISTS MenuItems;";
            CONNECTION.prepareStatement(DROP_MENUITEMS).execute();
            numberOfDroppedTables++;

            String DROP_INGREDIENTS = "DROP TABLE IF EXISTS Ingredients;";
            CONNECTION.prepareStatement(DROP_INGREDIENTS).execute();
            numberOfDroppedTables++;
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return numberOfDroppedTables;
    }

    @Override
    public int insertMenuItems(MenuItem[] items){
        int numberOfInsertedRows =  0;
        for (MenuItem item : items){
            try{
                String INSERT_MENUITEMS = "INSERT INTO MenuItems (itemID, itemName, cuisine, price) VALUES (?,?,?,?);";
                PreparedStatement statement = CONNECTION.prepareStatement(INSERT_MENUITEMS);

                // PreparedStatement indices start at 1
                statement.setInt(1, item.getItemID());
                statement.setString(2,item.getItemName());
                statement.setString(3, item.getCuisine());
                statement.setInt(4,item.getPrice());

                statement.execute();
                numberOfInsertedRows++;
            }
            catch (SQLException e){
                e.printStackTrace();
            }
        }
        return numberOfInsertedRows;
    }

    @Override
    public int insertIngredients(Ingredient[] ingredients){
        int numberOfInsertedRows = 0;
        for (Ingredient ingredient : ingredients){
            try{
                String INSERT_INGREDIENTS = "INSERT INTO Ingredients (ingredientID, ingredientName) VALUES (?,?);";
                PreparedStatement statement = CONNECTION.prepareStatement(INSERT_INGREDIENTS);

                statement.setInt(1, ingredient.getIngredientID());
                statement.setString(2, ingredient.getIngredientName());

                statement.execute();
                numberOfInsertedRows++;
            }
            catch (SQLException e){
                e.printStackTrace();
            }
        }
        return numberOfInsertedRows;
    }

    @Override
    public int insertIncludes(Includes[] includes){
        int numberOfInsertedRows = 0;
        for (Includes include : includes){
            try {
                String INSERT_INCLUDES = "INSERT INTO Includes (itemID, ingredientID) VALUES (?,?);";
                PreparedStatement statement = CONNECTION.prepareStatement(INSERT_INCLUDES);

                statement.setInt(1, include.getItemID());
                statement.setInt(2, include.getIngredientID());

                statement.execute();
                numberOfInsertedRows++;
            }
            catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberOfInsertedRows;
    }

    @Override
    public int insertDietaryCategories(DietaryCategory[] categories) {
        int numberOfInsertedRows = 0;
        for (DietaryCategory category : categories){
            try{
                String INSERT_DIETARY_CATEGORIES = "INSERT INTO DietaryCategories (ingredientID, dietaryCategory) VALUES (?,?);";
                PreparedStatement statement = CONNECTION.prepareStatement(INSERT_DIETARY_CATEGORIES);

                statement.setInt(1,category.getIngredientID());
                statement.setString(2, category.getDietaryCategory());

                statement.execute();
                numberOfInsertedRows++;
            }
            catch (SQLException e){
                e.printStackTrace();
            }
        }
        return numberOfInsertedRows;
    }

    @Override
    public int insertRatings(Rating[] ratings) {
        int numberOfInsertedRows = 0;
        for (Rating rating: ratings){
            try{
                String INSERT_RATINGS = "INSERT INTO Ratings (ratingID, itemID, rating, ratingDate) VALUES (?,?,?,?);";
                PreparedStatement statement = CONNECTION.prepareStatement(INSERT_RATINGS);

                statement.setInt(1,rating.getRatingID());
                statement.setInt(2, rating.getItemID());
                statement.setInt(3, rating.getRating());
                statement.setString(4, rating.getRatingDate());

                statement.execute();
                numberOfInsertedRows++;
            }
            catch (SQLException e){
                e.printStackTrace();
            }
        }
        return numberOfInsertedRows;
    }

    @Override
    public MenuItem[] getMenuItemsWithGivenIngredient(String name) {
        String QUERY = "SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price\n" +
                "FROM MenuItems MI, Ingredients I, Includes INC\n" +
                "WHERE INC.itemID = MI.itemID\n" +
                "AND INC.ingredientID = I.ingredientID\n" +
                "AND I.ingredientName = ?\n" +
                "ORDER BY MI.itemID ASC;";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            statement.setString(1,name);

            ResultSet resultSet = statement.executeQuery();

            ArrayList<MenuItem> rows = new ArrayList<>();
            while (resultSet.next()){
                MenuItem record = new MenuItem(
                        resultSet.getInt("itemID"),
                        resultSet.getString("itemName"),
                        resultSet.getString("cuisine"),
                        resultSet.getInt("price")
                );
                rows.add(record);
            }
            return rows.toArray(new MenuItem[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new MenuItem[0];
    }

    @Override
    public MenuItem[] getMenuItemsWithoutAnyIngredient() {
        String QUERY = "SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price\n" +
                "FROM MenuItems MI\n" +
                "WHERE MI.itemID NOT IN\n" +
                "(\n" +
                "SELECT DISTINCT MI.itemID\n" +
                "FROM MenuItems MI, Includes INC\n" +
                "WHERE MI.itemID = INC.itemID\n" +
                ")\n" +
                "ORDER BY MI.itemID ASC;";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);

            ResultSet resultSet = statement.executeQuery();

            ArrayList<MenuItem> rows = new ArrayList<>();
            while (resultSet.next()){
                MenuItem record = new MenuItem(
                        resultSet.getInt("itemID"),
                        resultSet.getString("itemName"),
                        resultSet.getString("cuisine"),
                        resultSet.getInt("price")
                );
                rows.add(record);
            }
            return rows.toArray(new MenuItem[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new MenuItem[0];
    }

    @Override
    public Ingredient[] getNotIncludedIngredients() {
        String QUERY = "SELECT DISTINCT I.ingredientID, I.ingredientName\n" +
                "FROM Ingredients I\n" +
                "WHERE I.ingredientID NOT IN\n" +
                "(\n" +
                "SELECT DISTINCT I.ingredientID\n" +
                "FROM Ingredients I, Includes INC\n" +
                "WHERE I.ingredientID = INC.ingredientID\n" +
                ")\n" +
                "ORDER BY I.ingredientID ASC;";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            ResultSet resultSet = statement.executeQuery();

            ArrayList<Ingredient> rows = new ArrayList<>();
            while (resultSet.next()){
                Ingredient record = new Ingredient(
                        resultSet.getInt("ingredientID"),
                        resultSet.getString("ingredientName")
                );
                rows.add(record);
            }
            return rows.toArray(new Ingredient[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new Ingredient[0];
    }

    @Override
    public MenuItem getMenuItemWithMostIngredients() {
        String QUERY = "SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price\n" +
                "FROM MenuItems MI, Includes INC\n" +
                "WHERE MI.itemID = INC.itemID\n" +
                "GROUP BY MI.itemID\n" +
                "HAVING COUNT(*) = (\n" +
                "SELECT MAX(temp.n_ingredients)\n" +
                "FROM (\n" +
                "SELECT COUNT(*) AS n_ingredients\n" +
                "FROM MenuItems MI2, Includes INC2\n" +
                "WHERE MI2.itemID = INC2.itemID\n" +
                "GROUP BY MI2.itemID\n" +
                ") AS temp\n" +
                ")\n" +
                "ORDER BY MI.itemID ASC;";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            ResultSet resultSet = statement.executeQuery();

            ArrayList<MenuItem> rows = new ArrayList<>();
            while (resultSet.next()){
                MenuItem record = new MenuItem(
                        resultSet.getInt("itemID"),
                        resultSet.getString("itemName"),
                        resultSet.getString("cuisine"),
                        resultSet.getInt("price")
                );
                rows.add(record);
            }
            return rows.get(0);
        }
        catch (SQLException e){
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public QueryResult.MenuItemAverageRatingResult[] getMenuItemsWithAvgRatings() {
        String QUERY = "SELECT DISTINCT MI.itemID, MI.itemName, AVG(R.rating) AS avg_rating\n" +
                "FROM MenuItems MI\n" +
                "LEFT JOIN Ratings R\n" +
                "ON MI.itemID = R.itemID\n" +
                "GROUP BY MI.itemID\n" +
                "ORDER BY avg_rating DESC;";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            ResultSet resultSet = statement.executeQuery();
            ArrayList<QueryResult.MenuItemAverageRatingResult> rows = new ArrayList<>();
            while (resultSet.next()) {
                QueryResult.MenuItemAverageRatingResult record = new QueryResult.MenuItemAverageRatingResult(
                        Integer.toString(resultSet.getInt("itemID")),
                        resultSet.getString("itemName"),
                        resultSet.getString("avg_rating")
                );
                rows.add(record);
            }
            return rows.toArray(new QueryResult.MenuItemAverageRatingResult[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new QueryResult.MenuItemAverageRatingResult[0];
    }

    @Override
    public MenuItem[] getMenuItemsForDietaryCategory(String category) {
        String QUERY = "SELECT DISTINCT MI.itemID, MI.itemName, MI.cuisine, MI.price\n" +
                "FROM MenuItems MI\n" +
                "WHERE NOT EXISTS (\n" +
                "SELECT I.ingredientID\n" +
                "FROM Ingredients I, Includes INC\n" +
                "WHERE INC.ingredientID = I.ingredientID\n" +
                "AND INC.itemID = MI.itemID\n" +
                "AND I.ingredientID NOT IN (\n" +
                "SELECT DC.ingredientID\n" +
                "FROM DietaryCategories DC\n" +
                "WHERE DC.dietaryCategory = ?\n" +
                ")\n" +
                ") AND\n" +
                "EXISTS (\n" +
                "SELECT INC.ingredientID\n" +
                "FROM Includes INC\n" +
                "WHERE INC.itemID = MI.itemID\n" +
                ")\n" +
                "ORDER BY MI.itemID ASC;";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            statement.setString(1, category);

            ResultSet resultSet = statement.executeQuery();

            ArrayList<MenuItem> rows = new ArrayList<>();
            while (resultSet.next()){
                MenuItem record = new MenuItem(
                        resultSet.getInt("itemID"),
                        resultSet.getString("itemName"),
                        resultSet.getString("cuisine"),
                        resultSet.getInt("price")
                );
                rows.add(record);
            }
            return rows.toArray(new MenuItem[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new MenuItem[0];
    }

    @Override
    public Ingredient getMostUsedIngredient() {
        String QUERY = "SELECT DISTINCT I.ingredientID, I.ingredientName\n" +
                "FROM Ingredients I, Includes INC\n" +
                "WHERE I.ingredientID = INC.ingredientID\n" +
                "GROUP BY I.ingredientID\n" +
                "HAVING COUNT(*) = (\n" +
                "SELECT MAX(temp.n_use)\n" +
                "FROM (\n" +
                "SELECT COUNT(*) AS n_use\n" +
                "FROM Ingredients I2, Includes INC2\n" +
                "WHERE I2.ingredientID = INC2.ingredientID\n" +
                "GROUP BY I2.ingredientID\n" +
                ") AS temp\n" +
                ")\n" +
                "ORDER BY I.ingredientID ASC;";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            ResultSet resultSet = statement.executeQuery();

            ArrayList<Ingredient> rows = new ArrayList<>();
            while (resultSet.next()){
                Ingredient record = new Ingredient(
                    resultSet.getInt("ingredientID"),
                    resultSet.getString("ingredientName")
                );
                rows.add(record);
            }
            return rows.get(0);
        }
        catch (SQLException e){
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public QueryResult.CuisineWithAverageResult[] getCuisinesWithAvgRating() {
        String QUERY = "SELECT DISTINCT MI.cuisine, AVG(R.rating) AS avg_rating\n" +
                "FROM MenuItems MI\n" +
                "LEFT JOIN Ratings R\n" +
                "ON MI.itemID = R.itemID\n" +
                "GROUP BY MI.cuisine\n" +
                "ORDER BY avg_rating DESC;\n";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            ResultSet resultSet = statement.executeQuery();

            ArrayList<QueryResult.CuisineWithAverageResult> rows = new ArrayList<>();
            while (resultSet.next()){
                QueryResult.CuisineWithAverageResult record = new QueryResult.CuisineWithAverageResult(
                    resultSet.getString("cuisine"),
                    resultSet.getString("avg_rating")
                );
                rows.add(record);
            }
            return rows.toArray(new QueryResult.CuisineWithAverageResult[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new QueryResult.CuisineWithAverageResult[0];
    }

    @Override
    public QueryResult.CuisineWithAverageResult[] getCuisinesWithAvgIngredientCount() {
        String QUERY = "SELECT DISTINCT MI.cuisine, 0 AS averageIngredientCount\n" +
                "FROM MenuItems MI\n" +
                "WHERE NOT EXISTS (\n" +
                "SELECT *\n" +
                "FROM MenuItems MI2, Includes INC\n" +
                "WHERE MI2.itemID = INC.itemID AND MI2.cuisine = MI.cuisine\n" +
                ")\n" +
                "UNION\n" +
                "SELECT temp1.cuisineName1, (ingredientCount/itemCount) AS averageIngredientCount\n" +
                "FROM\n" +
                "(\n" +
                "SELECT DISTINCT MI.cuisine AS cuisineName1, COUNT(*) AS itemCount\n" +
                "FROM MenuItems MI\n" +
                "GROUP BY MI.cuisine\n" +
                ") AS temp1,\n" +
                "(\n" +
                "SELECT DISTINCT MI.cuisine as cuisineName2, COUNT(*) AS ingredientCount\n" +
                "FROM MenuItems MI, Includes INC\n" +
                "WHERE INC.itemID = MI.itemID\n" +
                "GROUP BY MI.cuisine\n" +
                ") AS temp2\n" +
                "WHERE temp1.cuisineName1 = temp2.cuisineName2\n" +
                "ORDER BY averageIngredientCount DESC;\n";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            ResultSet resultSet = statement.executeQuery();

            ArrayList<QueryResult.CuisineWithAverageResult> rows = new ArrayList<>();
            while (resultSet.next()){
                QueryResult.CuisineWithAverageResult record = new QueryResult.CuisineWithAverageResult(
                        resultSet.getString("cuisine"),
                        resultSet.getString("averageIngredientCount")
                );
                rows.add(record);
            }
            return rows.toArray(new QueryResult.CuisineWithAverageResult[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new QueryResult.CuisineWithAverageResult[0];
    }

    @Override
    public int increasePrice(String ingredientName, String increaseAmount) {
        String QUERY = "UPDATE MenuItems MI\n" +
                "SET MI.price = MI.price + ?\n" +
                "WHERE EXISTS (\n" +
                "SELECT *\n" +
                "FROM Includes INC, Ingredients I\n" +
                "WHERE INC.ingredientID = I.ingredientID AND INC.itemID = MI.itemID AND I.ingredientName = ?\n" +
                ");";

        try{
            PreparedStatement statement = CONNECTION.prepareStatement(QUERY);
            statement.setInt(1, Integer.parseInt(increaseAmount));
            statement.setString(2, ingredientName);

            int numberOfRowsUpdated = statement.executeUpdate();
            return numberOfRowsUpdated;
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return 0;
    }

    @Override
    public Rating[] deleteOlderRatings(String date) {
        String QUERY1 = "SELECT R.ratingID, R.itemID, R.rating, R.ratingDate\n" +
                "FROM Ratings R\n" +
                "WHERE R.ratingDate < ?\n" +
                "ORDER BY R.ratingID ASC;\n";

        String QUERY2 = "DELETE FROM Ratings R WHERE R.ratingDate < ?;";

        try {
            PreparedStatement statement1 = CONNECTION.prepareStatement(QUERY1);
            PreparedStatement statement2 = CONNECTION.prepareStatement(QUERY2);

            statement1.setDate(1, Date.valueOf(date));
            statement2.setDate(1, Date.valueOf(date));

            ResultSet resultSet = statement1.executeQuery();

            ArrayList<Rating> rowsToBeDeleted = new ArrayList<>();
            while (resultSet.next()){
                Rating record = new Rating(
                        resultSet.getInt("ratingID"),
                        resultSet.getInt("itemID"),
                        resultSet.getInt("rating"),
                        resultSet.getDate("ratingDate").toString()
                );
                rowsToBeDeleted.add(record);
            }

            statement2.executeUpdate();

            return rowsToBeDeleted.toArray(new Rating[0]);
        }
        catch (SQLException e){
            e.printStackTrace();
        }

        return new Rating[0];
    }
}
