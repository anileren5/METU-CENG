import React from 'react';
import { useRouter } from 'next/navigation';
import { IconButton, makeStyles } from '@material-ui/core';
import { Add } from '@material-ui/icons';
import {Button} from "@mui/material";


const AddButton = () => {
    const router = useRouter();

    const handleClick = () => {
        router.push('/add-item');
    };

    return (
        <Button style={{backgroundColor: "#008DDA", color: "white"}} onClick={handleClick}>Add Item</Button>
    );
};

export default AddButton;
