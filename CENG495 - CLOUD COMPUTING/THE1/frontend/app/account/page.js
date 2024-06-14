"use client";

import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Container, Grid, Box } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import {useUser} from "@auth0/nextjs-auth0/client";
import {getUserByIdDB, updateUserById} from "@/utils/api/services";
import Body from "@/components/Body";
import Navbar from "@/components/Navbar";

const useStyles = makeStyles((theme) => ({
    formContainer: {
        marginTop: theme.spacing(4),
    },
    textField: {
        marginBottom: theme.spacing(2),
        '& .MuiInputBase-root:hover': {
            backgroundColor: 'transparent',
        },
        '& .MuiOutlinedInput-root.Mui-focused .MuiOutlinedInput-notchedOutline': {
            borderColor: 'transparent',
        },
    },
}));

const UserProfile = () => {
    const { user } = useUser();
    const classes = useStyles();
    const [isEditing, setIsEditing] = useState(false);
    const [userData, setUserData] = useState({
        custom_name: '',
        custom_surname: '',
        custom_phone: '',
        custom_address: '',
    });
    const [account, setAccount] = useState(null);

    useEffect(() => {
        if (user) getUserByIdDB(user.sub).then(response => {
            if (response) {
                setAccount(response);
                setUserData({
                    custom_name: response.user_metadata.custom_name,
                    custom_surname: response.user_metadata.custom_surname,
                    custom_phone: response.user_metadata.custom_phone,
                    custom_address: response.user_metadata.custom_address,
                });
            }
        });
    },[user]);

    const handleEdit = () => {
        setIsEditing(true);
    };

    const handleChange = (e) => {
        const { name, value } = e.target;
        setUserData({ ...userData, [name]: value });
    };

    const saveHandler = async () => {
        let temp = account;
        temp.user_metadata.custom_name = userData.custom_name;
        temp.user_metadata.custom_surname = userData.custom_surname;
        temp.user_metadata.custom_phone = userData.custom_phone;
        temp.user_metadata.custom_address = userData.custom_address;
        setIsEditing(false);
        console.log(temp);
        await updateUserById(user.sub, temp);
        if (user) getUserByIdDB(user.sub).then(response => {
            if (response) {
                setAccount(response);
                setUserData({
                    custom_name: response.user_metadata.custom_name,
                    custom_surname: response.user_metadata.custom_surname,
                    custom_phone: response.user_metadata.custom_phone,
                    custom_address: response.user_metadata.custom_address,
                });
            }
        });
    };

    if (!user) return null;

    return (
        <Body singleRow>
            <Navbar></Navbar>
            <Container maxWidth="sm">
                <Typography variant="h4" align="center" style={{marginBottom: '20px'}}>My Profile</Typography>
                <Box mt={4} className={classes.formContainer}>
                    <Grid container spacing={2}>
                        <Grid item xs={12}>
                            <TextField
                                name="email"
                                label="Email"
                                value={user.email}
                                InputProps={{
                                    readOnly: true,
                                }}
                                disabled={true}
                                fullWidth
                                className={classes.textField}
                            />
                        </Grid>
                        <Grid item xs={12}>
                            <TextField
                                name="custom_name"
                                label="Name"
                                value={userData.custom_name}
                                onChange={handleChange}
                                disabled={!isEditing}
                                fullWidth
                                className={classes.textField}
                            />
                        </Grid>
                        <Grid item xs={12}>
                            <TextField
                                name="custom_surname"
                                label="Surname"
                                value={userData.custom_surname}
                                onChange={handleChange}
                                disabled={!isEditing}
                                fullWidth
                                className={classes.textField}
                            />
                        </Grid>
                        <Grid item xs={12}>
                            <TextField
                                name="custom_phone"
                                label="Phone"
                                value={userData.custom_phone}
                                onChange={handleChange}
                                disabled={!isEditing}
                                fullWidth
                                className={classes.textField}
                            />
                        </Grid>
                        <Grid item xs={12}>
                            <TextField
                                name="custom_address"
                                label="Address"
                                value={userData.custom_address}
                                onChange={handleChange}
                                disabled={!isEditing}
                                fullWidth
                                className={classes.textField}
                            />
                        </Grid>
                        <Grid item xs={12}>
                            {isEditing ? (
                                <Button variant="contained" color="primary" onClick={saveHandler} fullWidth>
                                    Save
                                </Button>
                            ) : (
                                <Button variant="contained" color="primary" onClick={handleEdit} fullWidth>
                                    Edit
                                </Button>
                            )}
                        </Grid>
                    </Grid>
                </Box>
            </Container>
        </Body>
    );
};

export default UserProfile;
