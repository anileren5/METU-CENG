# Cengden

Note: I personally recommend you to read this README file via README editor in order to have better experience.

## Links for access to the application
Link for the backend: https://cengden-backend.onrender.com  
Link for the frontend: https://cengden.onrender.com   

## How to initiate the application ?
Remember that "Render" platform spin down the deployments because of inactivity for a long period. Since frontend uses
backend to fetch its content, backend should be activated before the frontend. So, follow the below instructions:

1 - Go to your browser and paste this link https://cengden-backend.onrender.com and press enter.  
2 - After some time, you will see a json response saying "Cengden Backend is up and working!", this means backend is ready to accept request and you can start the frontend application.  
3 - Go to your browser and paste this link https://cengden.onrender.com and press enter. After some time, pages will be loaded and you can start to use the application.

These needs to be done only after a long time. That is, you only need to do these once during grading.

## Pre-registered User Accounts


### Admin User
email: cengdenmetu@gmail.com  
password: cloudcomputing+495

### Sample User
email: e2448397@ceng.metu.edu.tr  
password: Aeren2001+

These are the two users that I defined for you. You can create your own account by signing up. Be careful that you should
use you ceng.metu.edu.tr email during registration. Otherwise, you would get such error: "WE'RE SORRY, SOMETHING WENT WRONG WHEN ATTEMPTING TO SIGN UP." 

## Login / Logout / Signup Instructions
You can login / signup from the top-tight corner. And similarly you can logout from there. In some case, logout may not work, you can try to click several times or refresh the page.
But be careful that when you refresh the page, you will automatically be logged out. So, be aware of this.

After login as a normal user, you can find all the menu by clicking on the human icon at the top-right corner.  
If you login as admin, you will see all menu on the home page.

## Filtering
Since I did not have much time while implementing the homework, I partially implemented the filtering.
I implemented filtering based on categories, price and Fuel Type (only for vehicles). I could do it much more
if I had time. I just implemented Fuel Type based filtering in order to show you that I am capable of this.

Note: After you applied the filters, please DO NOT FORGET TO PRESS "RESET FILTERS" BUTTON before applying the next filter.

## Design Decisions

### Why did I divide my application into backend and frontend ?
In order to implement the feature "Admin can delete user.", I should delete them from the Auth0 (deleting only from the database was not enough since I also should block the login via Auth0).
To achieve this, I needed to send a DELETE request to Auth0 management api and sending this request from the browser (frontend) was not allowed by Auth0 for security reasons. 
They only allow to send machine-machine requests. That's why I created the backend part and handle this functionality there.

### AuthO Flows
Auth0 provides very powerful mechanism to manage your signup and logins. They are called "flows". I have used two different flows. One of them is a
Pre User Registration flow and it was responsible for blocking signing up with non-ceng mail adresses. Other one is Post User Registration flow and it
was responsible for saving the information in the signup form to the database.

## Programming Languages and Frameworks
In the backend, I used Python as a programming language and FastAPI as the framework. The main reason behind this decision was the
fact that FastAPI is a framework which enables quick and easy development. Also, my past familiarity with this framework played a key role in this
decision.  

In the frontend, I used JavaScript as a programming language and React as the framework. The main reason behind this decision was the
fact that React is easy to use and enables faster development. I used JavaScript instead of TypeScript because I wanted to benefit from the typeless nature of JavaScript.


If you have any questions during the grading, you can contact me via anil.gocer@metu.edu.tr .


