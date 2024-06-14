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

export default function PrivateLessons() {
    const { user } = useUser();
    const [role, setRole] = useState(null);

    useEffect(() => {
        if (user) getUserRolesById(user.sub).then(response => {if (response) setRole(response["name"]);});
    },[user]);

    return (
        <div style={{display: "flex", flexDirection: "column"}}>
            <Navbar />
            {(!user || role !== "admin") && <CategoriesBar />}
            <div>
                <Body title={"my title"}>
                    {!user && <UnauthanticatedUserDashboard selectedCategory={"private lesson"}/>}
                    {user && role === "normal"&& <NormalUserDashboard selectedCategory={"private lesson"}/>}
                    {user && role === "admin"&& <AdminDashboard/>}
                </Body>
            </div>
        </div>
    );
}