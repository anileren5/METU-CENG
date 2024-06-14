// StarButton.js
import React, {useEffect, useState} from 'react';
import StarIcon from '@mui/icons-material/Star';
import StarBorderIcon from '@mui/icons-material/StarBorder';
import IconButton from '@mui/material/IconButton';
import {useUser} from "@auth0/nextjs-auth0/client";
import {addToFavorites, getUserByIdDB, removeFromFavorites} from "@/utils/api/services";

const FavoritesStar = ({item_id}) => {
    const { user } = useUser();
    const [account, setAccount] = useState(null);
    const [isFavorite, setIsFavorite] = useState(false);


    const fetchUserAccount = () => {
        if (user) getUserByIdDB(user.sub).then(response => {
            if (response) setAccount(response);
            setIsFavorite(response.favoriteItems.includes(item_id));
        });
    };

    useEffect(() => {
        fetchUserAccount();
    },[user]);

    const buttonStyle = {
        position: 'relative',
        borderRadius: '50%',
        backgroundColor: '#6a1b9a', // Purple color
        width: 48,
        height: 48,
        '&:hover': {
            backgroundColor: '#4a148c', // Darker shade of purple on hover
        },
        margin: "10px"
    };

    const starStyle = {
        position: 'absolute',
        top: '50%',
        left: '50%',
        transform: 'translate(-50%, -50%)',
        color: isFavorite ? '#FFD700' : '#fff', // Yellow if favorite, otherwise white
    };

    const handleStarClick = async () => {
        setIsFavorite(!isFavorite);
        if (isFavorite) {
            const response = await removeFromFavorites(user.sub, item_id);
            console.log(response.message);
        }
        else {
            const response = await addToFavorites(user.sub, item_id);
            console.log(response.message);
        }
    };

    return (
        <IconButton onClick={handleStarClick} aria-label="add to favorites" style={buttonStyle}>
            {isFavorite ? <StarIcon style={starStyle} /> : <StarBorderIcon style={starStyle} />}
        </IconButton>
    );
};

export default FavoritesStar;
