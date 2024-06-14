import React from "react";
import AppBar from "@mui/material/AppBar";
import Toolbar from "@mui/material/Toolbar";
import Typography from "@mui/material/Typography";
import IconButton from "@mui/material/IconButton";
import PersonIcon from "@mui/icons-material/Person";
import ShoppingCartIcon from "@mui/icons-material/ShoppingCart";
import Link from "next/link";
import {useUser} from "@auth0/nextjs-auth0/client";
import {Menu, MenuItem} from "@mui/material";
import {AccountCircle} from "@mui/icons-material";

export default function Navbar() {
    const { user } = useUser();

    const [anchorEl, setAnchorEl] = React.useState(null);
    const handleClose = () => {
        setAnchorEl(false);
    };

    const handleMenu = (event) => {
        setAnchorEl(event.currentTarget);
    };

    return (
        <AppBar position="fixed" style={{height: "7vh"}}>
            <Toolbar>
                <IconButton
                    size="large"
                    edge="start"
                    color="inherit"
                    aria-label="open drawer"
                >
                    <ShoppingCartIcon />
                </IconButton>
                <Typography variant="h6" noWrap>
                    <Link href={"/"} style={{textDecoration:"none", color:"inherit"}}>CENGDEN</Link>
                </Typography>
                <div style={{ flexGrow: 1 }} />
                {!user ?
                    <Link href="/api/auth/login" passHref legacyBehavior>
                    <IconButton
                        size="large"
                        edge="end"
                        aria-label="account of current user"
                        aria-haspopup="true"
                        color="inherit"
                    >
                        <PersonIcon style={{color: "white"}}/>
                        <Typography variant="body1" style={{ marginLeft: 5, color: "white"}}>
                            Sign Up / Log in
                        </Typography>
                    </IconButton>
                    </Link>
                    :
                    <div>
                        <IconButton
                            size="large"
                            aria-label="account of current user"
                            aria-controls="menu-appbar"
                            aria-haspopup="true"
                            onClick={handleMenu}
                            color="inherit"
                        >
                            <AccountCircle />
                        </IconButton>
                        <Menu
                            id="menu-appbar"
                            anchorEl={anchorEl}
                            anchorOrigin={{
                                vertical: 'top',
                                horizontal: 'right',
                            }}
                            keepMounted
                            transformOrigin={{
                                vertical: 'top',
                                horizontal: 'right',
                            }}
                            open={Boolean(anchorEl)}
                            onClose={handleClose}
                        >
                                {
                                    user.email !== "cengdenmetu@gmail.com" && (
                                        <>
                                        <MenuItem onClick={handleClose} style={{display:"block", padding:"10px"}}><Link href="/account" passHref legacyBehavior><Typography>Account</Typography></Link></MenuItem>
                                        <MenuItem onClick={handleClose} style={{display:"block", padding:"10px"}}><Link href="/my-items" passHref legacyBehavior><Typography>My Items</Typography></Link></MenuItem>
                                            <MenuItem onClick={handleClose} style={{display:"block", padding:"10px"}}><Link href="/update-my-items" passHref legacyBehavior><Typography>Update My Items</Typography></Link></MenuItem>
                                            <MenuItem onClick={handleClose} style={{display:"block", padding:"10px"}}><Link href="/favorites" passHref legacyBehavior><Typography>My Favorites</Typography></Link></MenuItem>
                                        </>
                                    )
                                }
                                <MenuItem onClick={handleClose} style={{display:"block", padding:"10px"}}><Link href="/api/auth/logout" passHref legacyBehavior><Typography>Logout</Typography></Link></MenuItem>
                        </Menu>
                    </div>
                }
            </Toolbar>
        </AppBar>
    );
}
