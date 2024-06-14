"use client";

import Link from "next/link";

export default function Home() {
  return (
    <div>
      <h1>Expense Splitter Home Page</h1>
      <Link legacyBehavior href={"/login"}>
        <a className="button green">Login</a>
      </Link>
      <Link legacyBehavior href={"/signup"}>
        <a className="button blue">Sign-up</a>
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
