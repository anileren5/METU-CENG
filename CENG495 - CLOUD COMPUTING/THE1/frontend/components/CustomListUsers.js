import React from 'react';
import { Box, Button, Typography, Divider } from '@mui/material';
import DeleteIcon from '@mui/icons-material/Delete';

export default function CustomListUsers({ data, onDelete }) {
    const handleDelete = (id) => {
        onDelete(id);
    };

    return (
        data &&
        <div>
            {data.filter(item => item.role !== "admin").map((item, index) => (
                <Box key={item._id} sx={{maxWidth: '80%', margin: '0 auto', marginBottom: 1}}>
                    <Box sx={{
                        display: 'flex',
                        alignItems: 'center',
                        justifyContent: 'space-between',
                        p: 2,
                        bgcolor: index % 2 === 0 ? 'background.paper' : 'transparent'
                    }}>
                        <Typography>{item.user_metadata.custom_name + " " + item.user_metadata.custom_surname + " (" + item.email + ")"}</Typography>
                        <Button
                            variant="outlined"
                            color="error"
                            onClick={() => handleDelete(item.user_id)}
                            startIcon={<DeleteIcon/>}
                        >
                            Delete
                        </Button>
                    </Box>
                    {index !== data.length - 1 && <Divider/>}
                </Box>
            ))}
        </div>
    );
};
