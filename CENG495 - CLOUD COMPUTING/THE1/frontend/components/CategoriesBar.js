import React from "react";
import AppBar from "@mui/material/AppBar";
import Toolbar from "@mui/material/Toolbar";
import Typography from "@mui/material/Typography";
import Link from "next/link";

const categories = [
    { name: "Vehicles", icon: "🚗", address: "vehicles" },
    { name: "Computers", icon: "💻", address: "computers" },
    { name: "Phones", icon: "📱", address: "phones" },
    { name: "Private Lessons", icon: "🎓", address: "private-lessons" },
];

export default function CategoriesBar() {
    return (
        <AppBar position="fixed" style={{ top: "7vh", height: "7vh"}}>
            <Toolbar
                style={{
                    background: "white",
                    display: "flex",
                    justifyContent: "space-around", // Center items with space around them
                    alignItems: "center",
                    paddingLeft: 20,
                    paddingRight: 20,
                }}
            >
                {categories.map((category, index) => (
                    <Link href={category.address} passHref key={index} style={{textDecoration: "none"}}>
                        <Typography variant="h6" noWrap style={{ color: "black" }}>
                            {category.icon} {category.name}
                        </Typography>
                    </Link>
                ))}
            </Toolbar>
        </AppBar>
    );
}
