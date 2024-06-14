import {UserProvider} from "@auth0/nextjs-auth0/client";

export default function RootLayout({ children }) {
 return (
     <html lang="en">
     <head>
         <title>Cengden</title>
         <link rel="icon" type="image/png" href="/favicon.png"/>
     </head>
     <UserProvider>
         <body style={{margin: 0, padding: 0}}>{children}</body>
     </UserProvider>
     </html>
 )
}
