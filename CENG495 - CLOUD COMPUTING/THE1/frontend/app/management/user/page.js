'use client';

import Navbar from "@/components/Navbar";
import Typography from "@mui/material/Typography";
import {useState, useEffect} from "react";
import {deleteUserById, getAllUsers, getAllUsersDB, getUserRolesById} from "@/utils/api/services";
import CustomListUsers from "@/components/CustomListUsers";

export default function UserManagement(){
    const [users, setUsers] = useState([]);

    useEffect(() => {
        const fetchData = async () => {
            try {
                const usersData = await getAllUsers();
                const modifiedUsersData = usersData.map(user => ({
                    ...user,
                }));
                setUsers(modifiedUsersData);
            } catch (error) {
                console.error('Error fetching user roles:', error);
            }
        };
        fetchData();
    }, []);

    const onDeleteHandler = async (id) => {
        await deleteUserById(id)
        const filteredData = users.filter(item => item.user_id !== id);
        setUsers(filteredData);
    }


    return (
        <div style={{display: "flex", flexDirection: "column"}}>
            <Navbar/>
            <Typography variant="h2" align="center" style={{ marginTop: '10vh' }}>User Deletion Admin Panel</Typography>
            <CustomListUsers data={users} onDelete={onDeleteHandler}></CustomListUsers>
        </div>
    );
}