import React, { useState, useEffect } from "react";

import { useParams } from "react-router-dom";
export default function Module() {
  const { id } = useParams();

  const [moduledata, setData] = useState({} as Get_Module);
  useEffect(() => {
    fetch("/API/module/" + id)
      .then((response) => response.json())
      .then((data) => setData(data.data));
  });

  return <>{moduledata.module_name}</>;
}
