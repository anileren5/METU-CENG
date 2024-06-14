"use client";

import { useParams } from 'next/navigation'
import { useEffect, useState } from 'react';

function getCurrentDateAndHour() {
    var currentTime = new Date();
    var currentDate = currentTime.getDate();
    var currentMonth = currentTime.getMonth() + 1; // Months are zero-indexed, so we add 1
    var currentYear = currentTime.getFullYear();
    var currentHour = currentTime.getHours();
    var currentMinute = currentTime.getMinutes();
    var formattedDate = currentYear + "-" + (currentMonth < 10 ? "0" : "") + currentMonth + "-" + (currentDate < 10 ? "0" : "") + currentDate;
    var formattedHour = (currentHour < 10 ? "0" : "") + currentHour;
    var formattedMinute = (currentMinute < 10 ? "0" : "") + currentMinute;
    return formattedDate + " " + formattedHour + ":" + formattedMinute;
}

function isBalanceReached(net_balances) {
    var flag = true;
    net_balances.forEach((item) => {
        if (Math.abs(item.balance) > 0.1) {
            flag = false;
        }
    })
    return flag;
}

function calculateWhoOwesWhom(expenseGroup) {
    if (expenseGroup) {    
        var average_expense = getAmountPerParticipant(expenseGroup);
        var total_expenses = getTotalExpensePerParticipant(expenseGroup);

        const net_balances = [];
        total_expenses.forEach((item) => {
            var name = item.name;
            var net_balance = average_expense - item.amount;
            net_balances.push({
                name: name,
                balance: net_balance
            })
        })
        
        const owes = [];

        while (!isBalanceReached(net_balances)) {
            console.log(net_balances);
            for (let i = 0; i < net_balances.length; i++) {
                if (net_balances[i].balance > 0) {
                    for (let j = 0; j < net_balances.length; j++) {
                        if (i != j){
                            if (net_balances[j].balance < 0) {
                                if (Math.abs(net_balances[i].balance) >= Math.abs(net_balances[j].balance)) {
                                    owes.push([net_balances[i].name, net_balances[j].name, Math.abs(net_balances[j].balance)])
                                    net_balances[i].balance = net_balances[i].balance + net_balances[j].balance;
                                    net_balances[j].balance = 0;
                                }
                                else {
                                    owes.push([net_balances[i].name, net_balances[j].name, Math.abs(net_balances[i].balance)])
                                    net_balances[j].balance = net_balances[i].balance + net_balances[j].balance;
                                    net_balances[i].balance = 0;
                                }
                            }
                        }
                    }
                }
            }        
        }
        
        const result = [];
        owes.forEach((owe) => {
            if (owe.at(2) != 0) {
                result.push(owe);
            }
        })
        
        return result;
    }
    else return [];
}

function getTotalExpensePerParticipant(expenseGroup) {
    if (expenseGroup) {
        const expensesPerParticipant = {}; 

        expenseGroup.expenses.forEach((expense) => {
            const payer = expense.payer;
            const amount = Number(expense.amount);
            expensesPerParticipant[payer] = (expensesPerParticipant[payer] || 0) + amount;
        });

        const participants = new Set(); 
        const loggedInUserName = JSON.parse(localStorage.getItem("loggedInUser")).name;
        participants.add(loggedInUserName);
        expenseGroup.expenses.forEach((expense) => {
            participants.add(expense.payer);
        });

        const totalExpensePerParticipant = [];
        participants.forEach((participant) => {
            totalExpensePerParticipant.push({ name: participant, amount: expensesPerParticipant[participant] || 0 });
        });

        return totalExpensePerParticipant;
    }
    else return [];
}

function getParticipants(expenseGroup) {
    if (expenseGroup) {
        const participants = new Set(); 
        const loggedInUserName = JSON.parse(localStorage.getItem("loggedInUser")).name;
        participants.add(loggedInUserName);
        expenseGroup.expenses.forEach((expense) => {
            participants.add(expense.payer);
        });
        return [...participants].join(", ");
    }
}

function getTotalExpense(expenseGroup) {
    if (expenseGroup) {
        var sum = 0;
        expenseGroup.expenses.forEach((expense) => {
            sum += Number(expense.amount);
        });
        return sum;
    }
}

function getAmountPerParticipant(expenseGroup) {
    if (expenseGroup) {
        var sum = 0;
        expenseGroup.expenses.forEach((expense) => {
            sum += Number(expense.amount);
        });
        const participants = new Set(); 
        const loggedInUserName = JSON.parse(localStorage.getItem("loggedInUser")).name;
        participants.add(loggedInUserName);
        expenseGroup.expenses.forEach((expense) => {
            participants.add(expense.payer);
        });
        return sum / participants.size;
    }
}

export default function Details() {
    const params = useParams()
    const [expenseGroup, setExpenseGroup] = useState(null);
    const [newExpense, setNewExpense] = useState({
        name: '',
        amount: '',
        payer: '', 
    });

    useEffect(() => {
        const fetchData = async () => {
            try {
                const response = await fetch(`http://localhost:8004/details/${params["id"]}`);
                const data = await response.json();
                setExpenseGroup(data)
            } catch (error) {
                console.error('Error fetching data:', error);
            }
        };
    
        fetchData();
    }, []);

    const handleInputChange = (e) => {
        const { name, value } = e.target;
        setNewExpense(prevState => ({
            ...prevState,
            [name]: value
        }));
    };

    const handleSubmit = async (e) => {
        var temp = newExpense;
        temp.date = getCurrentDateAndHour();
        console.log(temp);
        e.preventDefault();

        const fetchGroup = async () => {
            try {
                const response = await fetch(`http://localhost:8004/details/${params["id"]}`);
                const data = await response.json();
                setExpenseGroup(data)
            } catch (error) {
                console.error('Error fetching data:', error);
            }
        };
    
        try {
            await fetch('http://localhost:8004/details/'+params["id"], {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(temp),
            });
            setNewExpense({
                name: '',
                amount: '',
                payer: '', 
            });
            fetchGroup();
        } catch (error) {
            console.error('Error adding expense:', error);
        }
    };


    return (
        expenseGroup && 
        <div>
            <div>
                <div>
                    <h1>Expense Group Details</h1>
                    <hr></hr>
                    <h3>Group Name: {expenseGroup["expense_group_name"]}</h3>
                    <h3>Created by: Me ({JSON.parse(localStorage.getItem("loggedInUser")).name})</h3>
                    <h3>Date created: {expenseGroup["time"]}</h3>
                    <h3>Participants: {getParticipants(expenseGroup)}</h3>
                    <h3>Total Expense: {getTotalExpense(expenseGroup)} TL</h3>
                    <h3>Amount per Participant: {getAmountPerParticipant(expenseGroup)} TL</h3>
                </div>
                <hr></hr>
                <div>
                    <h2>Expenses</h2>
                    <ul>
                        {expenseGroup.expenses.map((expense) => (
                            <li><b>{expense.name}</b>, Amount: <b style={{color:"red"}}>{expense.amount}</b> TL, Payer: <b>{expense.payer}</b>, Date-Time: {expense.date}</li>
                        ))}
                    </ul>
                </div>
                <hr></hr>
                <div>
                    <h2>Total Expenses Made by Each Participant</h2>
                    <ul>
                      {getTotalExpensePerParticipant(expenseGroup).map((result) => (
                        <li><b>{result.name}</b> : {result.amount} TL</li>
                      ))} 
                    </ul>
                </div>
                <hr></hr>
                <div>
                    <h2>Who Owes Whow</h2>
                    <ul>
                        {calculateWhoOwesWhom(expenseGroup).map((owe) => (
                            <li><b>{owe.at(0)}</b> owes <b>{owe.at(1)}</b> <b style={{color:"red"}}>{owe.at(2)}</b> TL</li>
                        ))}
                    </ul>
                </div>
                <hr></hr>
            </div>
            <div style={{ clear: 'both' }}>
                <h2>Add New Expense</h2>
                <div style={{ maxWidth: '400px', float: 'left', marginRight: '20px' }}>
                    <div style={{ marginBottom: '15px' }}>
                        <label htmlFor="expenseName" style={{ display: 'block', marginBottom: '5px' }}>Expense Name:</label>
                        <input type="text" id="expenseName" name="name" value={newExpense.name} onChange={handleInputChange} required style={{ width: '100%', padding: '10px', border: '1px solid #ccc', borderRadius: '5px', fontSize: '16px' }} />
                    </div>
                    <div style={{ marginBottom: '15px' }}>
                        <label htmlFor="expenseAmount" style={{ display: 'block', marginBottom: '5px' }}>Amount:</label>
                        <input type="number" id="expenseAmount" name="amount" value={newExpense.amount} onChange={handleInputChange} required style={{ width: '100%', padding: '10px', border: '1px solid #ccc', borderRadius: '5px', fontSize: '16px' }} />
                    </div>
                    <div style={{ marginBottom: '15px' }}>
                        <label htmlFor="expensePayer" style={{ display: 'block', marginBottom: '5px' }}>Payer:</label>
                        <input type="text" id="expensePayer" name="payer" value={newExpense.payer} onChange={handleInputChange} required style={{ width: '100%', padding: '10px', border: '1px solid #ccc', borderRadius: '5px', fontSize: '16px' }} />
                    </div>
                    <button type="submit" onClick={handleSubmit} style={{ padding: '10px 20px', backgroundColor: '#007bff', color: '#fff', border: 'none', borderRadius: '5px', fontSize: '16px', cursor: 'pointer' }}>Add Expense</button>
                </div>
            </div> 
        </div>
    );
}

