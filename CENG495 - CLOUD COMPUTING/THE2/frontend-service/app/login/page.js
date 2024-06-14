"use client";

import { useRouter } from 'next/navigation';
import { useState } from 'react';

export default function Login() {
  const router = useRouter();
  const [formData, setFormData] = useState({
    email: '',
    password: ''
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
    // Handle form submission here (e.g., send data to backend)
    const { name, email, password } = formData;
    const response = await fetch('http://localhost:8001/login', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        email: email,
        password: password
      })
    });

    if (response.status === 200) {
      setSuccessMessage('Logged in! Being directed to dashboard ...');
      setErrorMessage('');
      setFormData({
        email: '',
        password: ''
      });
      localStorage.setItem('userEmail', email);
      setTimeout(() => {
        router.push('/dashboard');
      }, 1000);
    } else {
      setSuccessMessage('');
      setErrorMessage('Something went wrong with login process.');
    }
  };

  return (
    <div className="sign-up-container">
      <h1>Login</h1>
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label htmlFor="email">Email:</label>
          <input 
            type="email" 
            id="email" 
            name="email" 
            value={formData.email} 
            onChange={handleChange} 
            required 
          />
        </div>
        <div className="form-group">
          <label htmlFor="password">Password:</label>
          <input 
            type="password" 
            id="password" 
            name="password" 
            value={formData.password} 
            onChange={handleChange} 
            required 
          />
        </div>
        <div>
          <button type="submit" className="submit-button">Login</button>
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
