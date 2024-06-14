import React from "react";

const navbarHeight = 7;
const categoriesBarHeight = 7;

const bodyStyle = {
    display: "flex",
    flexDirection: "column",
    background: "#f5f5f5",
    minHeight: "100vh",
};

const contentStyle = {
    flex: 1,
    padding: "20px",
    marginTop: navbarHeight + categoriesBarHeight + "vh",
};

const contentStyle2 = {
    flex: 1,
    padding: "20px",
    marginTop: navbarHeight + "vh",
};

export default function Body({ children, singleRow }) {
    if (singleRow) {
        return (
            <div style={bodyStyle}>
                <div style={contentStyle2}>{children}</div>
            </div>
        );
    }

    return (
        <div style={bodyStyle}>
            <div style={contentStyle}>{children}</div>
        </div>
    );
}
