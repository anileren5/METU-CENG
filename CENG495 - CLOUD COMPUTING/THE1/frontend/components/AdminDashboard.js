import Link from "next/link";
import IconButton from "@mui/material/IconButton";
import {People, ShoppingCart} from "@mui/icons-material";
import {Box} from "@mui/material";
import Typography from "@mui/material/Typography";

export default function AdminDashboard() {
    return (
        <div>
            <Typography variant="h1" align="center" style={{ marginBottom: '20px' }}>Admin Dashboard</Typography>
            <Box display="flex" justifyContent="center" alignItems="flex-end" height="100vh">
                <Box textAlign="center" mr={4} style={{ marginBottom: '70vh' }}>
                    <Link href="/management/user" passHref>
                        <IconButton aria-label="People" style={{ fontSize: 96 }}>
                            <People fontSize="inherit" />
                        </IconButton>
                    </Link>
                    <Typography variant="subtitle1">User Management</Typography>
                </Box>
                <Box textAlign="center" ml={4} style={{ marginBottom: '70vh' }}>
                    <Link href="/management/product" passHref>
                        <IconButton aria-label="Shopping Cart" style={{ fontSize: 96 }}>
                            <ShoppingCart fontSize="inherit" />
                        </IconButton>
                    </Link>
                    <Typography variant="subtitle1">Product Management</Typography>
                </Box>
            </Box>
        </div>
    );
}