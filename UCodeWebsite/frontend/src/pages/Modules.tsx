import { useEffect } from "react";
export default function Modules() {
    
   /*
   .then((response) => console.log(response);  response.json())
        .then((data) => {
            console.log(data);
        });
   */
    fetch('/API/modules/newest')
        .then((response) => console.log(response));
    return <h1>Modules</h1>
}

