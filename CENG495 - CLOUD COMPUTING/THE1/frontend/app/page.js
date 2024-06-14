'use client';

import Navbar from "@/components/Navbar";
import Body from "@/components/Body";
import CategoriesBar from "@/components/CategoriesBar";
import {useUser} from "@auth0/nextjs-auth0/client";
import {getUserRolesById} from "@/utils/api/services";
import {useEffect, useState} from "react";
import UnauthanticatedUserDashboard from "@/components/UnauthanticatedUserDashboard";
import NormalUserDashboard from "@/components/NormalUserDashboard";
import AdminDashboard from "@/components/AdminDashboard";
import {Typography} from "@mui/material";

export default function Home() {
    const { user } = useUser();
    const [role, setRole] = useState(null);

    useEffect(() => {
        if (user) getUserRolesById(user.sub).then(response => {if (response) setRole(response["name"]);});
    },[user]);

    return (
        <div style={{display: "flex", flexDirection: "column"}}>
            <Navbar/>
            {(!user || role !== "admin") && <CategoriesBar />}
            <div>
                <Body title={"my title"}>
                    {user && !user.email_verified && <Typography variant="h1" align="center" style={{marginBottom: '20px'}}>Please Verify Your Email</Typography>}
                    {!user && <UnauthanticatedUserDashboard/>}
                    {user && user.email_verified && role === "normal"&& <NormalUserDashboard/>}
                    {user && user.email_verified && role === "admin"&& <AdminDashboard/>}
                </Body>
            </div>
        </div>
    );
}
