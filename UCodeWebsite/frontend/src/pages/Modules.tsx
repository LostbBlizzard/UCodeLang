import React, { useState, useEffect } from "react";
import "./Modules.css";
import "./Page.css";
import { Link } from "react-router-dom";
export default function Modules() {
  const [newestmoduledata, newestsetData] = useState(
    [] as Get_Modules_Module[]
  );
  const [mostusedmoduledata, mostusedsetData] = useState(
    [] as Get_Modules_Module[]
  );
  const [lastupdatedmoduledata, lastupdatedsetData] = useState(
    [] as Get_Modules_Module[]
  );

  useEffect(() => {
    fetch("/API/modules/newest/")
      .then((response) => response.json())
      .then((data) => newestsetData(data.data));
  }, []);

  useEffect(() => {
    fetch("/API/modules/mostdownloaded/")
      .then((response) => response.json())
      .then((data) => mostusedsetData(data.data));
  }, []);

  useEffect(() => {
    fetch("/API/modules/lastupdated/")
      .then((response) => response.json())
      .then((data) => lastupdatedsetData(data.data));
  }, []);

  return (
    <div>
      <h1 className="ModuleSectionLabelText">Modules</h1>

      <Link to={"/signup"} style={{ textDecoration: "none" }}>
        <button type="button">Sign Up</button>
      </Link>
      <Link to={"/logoin"} style={{ textDecoration: "none" }}>
        <button type="button">Log In</button>
      </Link>
      <div className="topnav">
        <input type="text" placeholder="Search.."></input>
      </div>

      <TagList
        tagname="Most Popular"
        modulelist={mostusedmoduledata}
        modulecallback={mostusedsetData}
      ></TagList>
      <TagList
        tagname="Most Newest"
        modulelist={newestmoduledata}
        modulecallback={newestsetData}
      ></TagList>
      <TagList
        tagname="Last Updated"
        modulelist={lastupdatedmoduledata}
        modulecallback={lastupdatedsetData}
      ></TagList>
    </div>
  );
}

function TagList({
  tagname,
  modulelist,
  modulecallback,
}: {
  tagname: string;
  modulelist: Get_Modules_Module[];
  modulecallback: React.Dispatch<React.SetStateAction<Get_Modules_Module[]>>;
}) {
  return (
    <>
      <h1 className="MainLabel">{tagname}</h1>
      {modulelist.map((mod) => (
        <ModuleLabel module={mod} />
      ))}
    </>
  );
}

function ModuleLabel({ module }: { module: Get_Modules_Module }) {
  return (
    <>
      <li key={module.id} className="modulelist">
        <Link to={"/module/" + module.id} style={{ textDecoration: "none" }}>
          <div className="modulelabelbox">
            <h1 className="modulelabel">{module.module_name}</h1>
            {}
          </div>
        </Link>
      </li>
    </>
  );
}
