"use client";

import Link from "next/link";
import { useEffect, useState } from "react";


export default function Dashboard() {
  const [userName, setUserName] = useState("");
  const [expenseGroups, setExpenseGroups] = useState([]);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch(`http://localhost:8002/${localStorage.getItem("userEmail")}`);
        const data = await response.json();
        localStorage.setItem("loggedInUser", JSON.stringify(data));
        setUserName(data.name);
      } catch (error) {
        console.error('Error fetching data:', error);
      }
    };

    fetchData();
  }, []);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch(`http://localhost:8003/group/${localStorage.getItem("userEmail")}`);
        const data = await response.json();
        setExpenseGroups(data)
      } catch (error) {
        console.error('Error fetching data:', error);
      }
    };

    fetchData();
  }, []);


  return (
    <div>
      <h1>Expense Splitter Dashboard</h1>
      <hr></hr>
      <h2>Welcome {userName} !</h2>
      <hr></hr>

      <h2>Your Expense Groups</h2>
      {expenseGroups.map((expenseGroup) => 
        <>
          <div style={{ display: "flex", alignItems: "center" }}>
              <h3 style={{ marginRight: "10px" }}>{expenseGroup.expense_group_name}</h3>
              <Link href={"/details/" + expenseGroup["_id"]} style={{ textDecoration: "none", color: "blue", background: "lightblue", padding: "5px 10px", borderRadius: "5px" }}>Details</Link>
          </div>
        </>
      )}
      <Link legacyBehavior href={"/add-expense-group"}>
        <a className="button blue">Add Expense Group</a>  
      </Link>

      <style jsx>{`
        .button {
          display: inline-block;
          padding: 10px 20px;
          color: white;
          text-align: center;
          text-decoration: none;
          font-size: 16px;
          margin: 4px 2px;
          cursor: pointer;
          border-radius: 8px;
        }

        .green {
          background-color: #4CAF50;
        }

        .blue {
          background-color: #007bff;
        }

        .button:hover {
          opacity: 0.8;
        }
      `}</style>
    </div>
  );
}
