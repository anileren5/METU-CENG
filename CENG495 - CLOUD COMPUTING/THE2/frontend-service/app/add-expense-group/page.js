"use client";

import { useRouter } from 'next/navigation';
import { useState } from 'react';

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

export default function AddExpenseGroup() {
  const router = useRouter();
  const [formData, setFormData] = useState({
    name: '',
  });
  const [successMessage, setSuccessMessage] = useState('');
  const [errorMessage, setErrorMessage] = useState('');

  const handleChange = (event) => {
    const { name, value } = event.target;
    setFormData({
      ...formData,
      [name]: value
    });
  };

  const handleSubmit = async (event) => {
    event.preventDefault();
    const { name } = formData;
    const response = await fetch('http://localhost:8003/group', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        user_email: localStorage.getItem("userEmail"),
        expense_group_name: name,
        time: getCurrentDateAndHour(),
        expenses:[]
      })
    });
    if (response.status === 200) {
      setSuccessMessage('Expense group created! Being directed to dashboard ...');
      setErrorMessage('');
      setFormData({
        name: '',
      });
      setTimeout(() => {
        router.push('/dashboard');
      }, 1000);
    } else {
      setSuccessMessage('');
      setErrorMessage('Something went wrong with the process.');
    }
  };

  return (
    <div className="sign-up-container">
      <h1>Add New Expense Group</h1>
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label htmlFor="name">Name:</label>
          <input 
            type="text" 
            id="name" 
            name="name" 
            value={formData.email} 
            onChange={handleChange} 
            required 
          />
        </div>
        <div>
          <button type="submit" className="submit-button">Add</button>
        </div>
      </form>

      {successMessage && (
        <div className="success-message">{successMessage}</div>
      )}

      {errorMessage && (
        <div className="error-message">{errorMessage}</div>
      )}

      <style jsx>{`
        .sign-up-container {
          max-width: 400px;
          margin: 0 auto;
          padding: 20px;
          border: 1px solid #ccc;
          border-radius: 8px;
          box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        .form-group {
          margin-bottom: 20px;
        }

        label {
          display: block;
          margin-bottom: 5px;
        }

        input[type="text"],
        input[type="email"],
        input[type="password"] {
          width: 100%;
          max-width: 300px;
          padding: 10px;
          border: 1px solid #ccc;
          border-radius: 4px;
        }

        .submit-button {
          background-color: #007bff;
          color: white;
          border: none;
          border-radius: 4px;
          padding: 10px 20px;
          cursor: pointer;
          transition: background-color 0.3s ease;
        }

        .submit-button:hover {
          background-color: #0056b3;
        }

        .success-message {
          background-color: #4CAF50;
          color: white;
          padding: 10px;
          border-radius: 4px;
          margin-top: 10px;
        }

        .error-message {
          background-color: #ff0000;
          color: white;
          padding: 10px;
          border-radius: 4px;
          margin-top: 10px;
        }
      `}</style>
    </div>
  );
}
