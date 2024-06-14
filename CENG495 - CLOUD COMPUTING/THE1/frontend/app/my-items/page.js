"use client";

import Body from "@/components/Body";
import Navbar from "@/components/Navbar";
import {useEffect, useState} from "react";
import ItemPopup from "@/components/ItemPopup";
import {Button, Card, CardContent, Typography} from "@mui/material";
import {activateItem, deactivateItem, deleteItem, getAllItemsByUserId} from "@/utils/api/services";
import {useUser} from "@auth0/nextjs-auth0/client";
import DeleteIcon from "@material-ui/icons/Delete";
import FavoritesStar from "@/components/FavoritesStar";
import {Pagination} from "@mui/material";


// Default base64 encoded image
const defaultImageBase64 = "data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgICAgMCAgIDAwMDBAYEBAQEBAgGBgUGCQgKCgkICQkKDA8MCgsOCwkJDRENDg8QEBEQCgwSExIQEw8QEBD/wAALCAFoAWgBAREA/8QAHgABAQEBAQEBAQEBAQAAAAAAAAkIBwYFAwQKAQL/xABZEAABAwMDAgIEBwwFBgsIAwABAAIDBAUGBwgREiEJExkxQZYUIjlRV7XUFTI4QlNVYXR1dtHTFiNxs7QkMzdigZEXGENEUmNyc4KhwTU2RVaDhpKlo7HS/9oACAEBAAA/AKpoiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiIiLjGq+8Tblohl39BNUNRDZb4KOKv+C/cevqf6iVz2sf1wQPZ3Mbxx1cjjuB2Xj/SQ7L/AKZD7uXb7KnpIdl/0yH3cu32VPSQ7L/pkPu5dvsqekh2X/TIfdy7fZU9JDsv+mQ+7l2+yp6SHZf9Mh93Lt9lT0kOy/6ZD7uXb7KnpIdl/wBMh93Lt9lT0kOy/wCmQ+7l2+yp6SHZf9Mh93Lt9lT0kOy/6ZD7uXb7KnpIdl/0yH3cu32VPSQ7L/pkPu5dvsqekh2X/TIfdy7fZU9JDsv+mQ+7l2+yp6SHZf8ATIfdy7fZU9JDsv8ApkPu5dvsqekh2X/TIfdy7fZU9JDsv+mQ+7l2+yp6SHZf9Mh93Lt9lT0kOy/6ZD7uXb7KnpIdl/0yH3cu32VPSQ7L/pkPu5dvsq9bpbvM22605jDgGmeo5vF+qKaarjpPuNcKfqii48x3XNAxg46m9i7k89uV2pERERERERERERFKbfrY7flfiB4ZiF4Ext2Q02IWetEEpjkNPU3aphl6Hju13RI7hw9R4K04fCz2qEk+Rm45PqGU1Xb/AM09FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8U9FltV/I5v701X8UHhZ7VAQTBm5/tymq/isr7J8etuH+Ivf8LsgmFsxyXMLPQieUyyCnp6uKKMPe7u53Swck9yVWVERERERERERERFLXel8pXph+uYJ9eTKpSIiIiIiIiIiIiIiKVu0/5UPPf21nf1gxVSRERERERERERERS13pfKV6YfrmCfXkyqUiIiIiIiIiIiIiIilbtP+VDz39tZ39YMVUkREREREREREREUtd6XylemH65gn15MqlIiIi+dkGRY/iVmqsiym+2+zWqhZ5lTXXCqZT08DeQOp8khDWjkgck+1cAufiK7MrVWyUM2tlJVPicWukt9nuNbByPmmgp3xuH6Q4hdM0r3D6Ia3MedKtULBkU8TDJLR01UG1kTAQOqSmf0zMbyQOXMAXREREREREREUrdp/yoee/trO/rBiqkiIiIiIiIiIiIilrvS+Ur0w/XME+vJlUpERF47V/VXFNEtNr9qjms8sdpsFL58jIW9U1RI5wZFBE3sDJJI5kbQSB1PHJA5Ilhj+GbkvE51MrsjyO7w2TDsfqRGXztfParC5wDm0tHTgtFZXdDmuklcW8AtLnMa6KI64sPhVbaLdbvg19umd32tcwB9ZPf30xDuO5ZHTNjjaOfUC136SfWuF7gvDCyjTmk/4TdtWY5Be6uwcVsdpqZxHfICz4xlt1bTtjc6VvYiJwD3AENkc7hju2eHxvSr9fLdU6Xam18U2cWSjFbSXERCIXy3hwY+VzAA1lRE5zGytAaCHsc0D44ZtBERERERERSt2n/Kh57+2s7+sGKqSIiIiIiIiIiIiKWu9L5SvTD9cwT68mVSkREU7PGIzq4WrFNOMGgeDQ11XdcjrI/yj6CCKOBp/R1VrncfOxp9YC2Jtm0qtui+hOGae2+mZHNQWqGW4yAd6i4StElVM4+0vme89/UOB6gF09FJ/XWyRbb/ExxfJsV6bfQZFfbJkDooh8VkV1qJbdcW8H2Pe2ebj1dUnbjgKsCIiIiIiIilbtP8AlQ89/bWd/WDFVJERERERERERERFLXel8pXph+uYJ9eTKpSIiKevjBae197wfT/P6dhNFaK644/Xva0kw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";

const PAGE_SIZE = 10;

function capitalizeWords(str) {
    if (typeof str !== 'string') {
        return ""; // Handle cases where str is not a string
    }
    return str.replace(/\b\w/g, function(char) {
        return char.toUpperCase();
    });
}

export default function MyItems() {
    const { user } = useUser();
    const [items, setItems] = useState(null);
    const [selectedItem, setSelectItem] = useState(null);
    const [popupOpen, setPopupOpen] = useState(false)
    const [currentPage, setCurrentPage] = useState(1);
    const [totalItemCountInThatCategory, setTotalItemCountInThatCategory] = useState(null);

    const handlePageChange = (event, page) => {
        setCurrentPage(page);
    };

    const fetchInitialData = async () => {
        if (user) {
            try {
                const data = await getAllItemsByUserId(user.sub);
                setTotalItemCountInThatCategory(data.length)
                let start_index = (currentPage - 1) * PAGE_SIZE;
                let end_index = start_index + PAGE_SIZE;
                end_index = Math.min(end_index, data.length);
                setItems(data.slice(start_index, end_index));
            } catch (error) {
                console.error('Error fetching initial data:', error);
            }
        }
    };

    useEffect(() => {
        fetchInitialData();
    }, [user, currentPage]);

    const activationHandler = async (item_id) => {
        await activateItem(item_id);
        fetchInitialData()
    }

    const deactivationHandler = async (item_id) => {
        await deactivateItem(item_id);
        fetchInitialData();
    }

    const deleteHandler = async (item_id) => {
        await deleteItem(item_id);
        fetchInitialData();
    }

    return (
        <Body singleRow>
            <Navbar></Navbar>
            <Typography variant="h4" align="center" style={{marginBottom: '20px'}}>My Items</Typography>
            <div style={{display: "flex", justifyContent: "center", alignItems: "center"}}>
                <Pagination
                    count={Math.floor(totalItemCountInThatCategory / PAGE_SIZE) + (totalItemCountInThatCategory % PAGE_SIZE === 0 ? 0 : 1)}
                    onChange={handlePageChange} style={{marginBottom: "2%"}}/>
            </div>
            <div>
                {popupOpen && <ItemPopup isContactDetailsSecret={false} open={popupOpen} item={selectedItem} onClose={() => {
                    setSelectItem(null);
                    setPopupOpen(false);
                }}></ItemPopup>}
                {items && items.map((item) => (
                    <Card key={item._id} style={{marginBottom: '20px', display: 'flex'}}>
                        <img src={item.image || defaultImageBase64} alt={item.title}
                             style={{width: '240px', height: '240px', objectFit: 'cover'}}/>
                        <CardContent style={{flex: '1', paddingLeft: '20px'}}>
                            <Typography gutterBottom variant="h5" component="div">
                                {item.title}
                            </Typography>
                            <Typography variant="h6" color="text.secondary" style={{marginBottom: '10px'}}>
                                Category: {capitalizeWords(item.category)}
                            </Typography>
                            <Typography variant="h6" color="primary" gutterBottom>
                                Price: {item.price} {item.currency}
                            </Typography>
                            <Typography variant={"h6"} style={{display: "inline"}}>Status: </Typography>
                            {
                                item.isActive ?
                                    <Typography variant={"h6"}
                                                style={{color: "green", display: "inline"}}>Active</Typography>
                                    :
                                    <Typography variant={"h6"}
                                                style={{color: "red", display: "inline"}}>Inactive</Typography>
                            }<br/>
                            <Button style={{
                                backgroundColor: "#25aae1",
                                color: "white",
                                marginRight: "5px",
                                marginTop: "5px"
                            }} onClick={() => {
                                setSelectItem(item);
                                setPopupOpen(true)
                            }}>View Details</Button>
                            {
                                item.isActive ?
                                    <Button style={{
                                        backgroundColor: "#25aae1",
                                        color: "white",
                                        background: "red",
                                        marginRight: "5px",
                                        marginTop: "5px"
                                    }} onClick={() => {
                                        deactivationHandler(item._id)
                                    }}>Deactivate</Button>
                                    :
                                    <Button style={{
                                        backgroundColor: "#25aae1",
                                        color: "white",
                                        background: "green",
                                        marginRight: "5px",
                                        marginTop: "5px"
                                    }} onClick={() => {
                                        activationHandler(item._id)
                                    }}>Activate</Button>
                            }
                            <Button style={{
                                backgroundColor: "#25aae1",
                                color: "white",
                                background: "red",
                                marginRight: "5px",
                                marginTop: "5px"
                            }} onClick={() => {
                                deleteHandler(item._id)
                            }}>DELETE <DeleteIcon></DeleteIcon></Button>
                        </CardContent>
                        <FavoritesStar item_id={item._id}></FavoritesStar>
                    </Card>
                ))}
            </div>
        </Body>
    );
}