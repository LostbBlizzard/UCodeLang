import React, { useState, useEffect } from "react";
import "./Modules.css"
import "./Page.css"
export default function Modules() {

    const [newestmoduledata, newestsetData] = useState(new Array() as Get_Modules_Module[]);
    const [mostusedmoduledata, mostusedsetData] = useState(new Array() as Get_Modules_Module[]);
    const [lastupdatedmoduledata, lastupdatedsetData] = useState(new Array() as Get_Modules_Module[]);

  useEffect(() => {
        fetch("/API/modules/newest/")
            .then(response => response.json())
            .then(data => newestsetData(data.data) );
  }, []);   
   
    
    useEffect(() => {
        fetch("/API/modules/mostdownloaded/")
            .then(response => response.json())
            .then(data => mostusedsetData(data.data) );
  }, []);   

    useEffect(() => {
        fetch("/API/modules/lastupdated/")
            .then(response => response.json())
            .then(data => lastupdatedsetData(data.data) );
  }, []);   
    
    return (
        <div>
            <h1 className="ModuleSectionLabelText">Modules</h1> 
            <div className="topnav">
                <input type="text" placeholder="Search.."></input>
            </div>

            <TagList tagname="Most Popular" modulelist={mostusedmoduledata} modulecallback={mostusedsetData}></TagList>      
            <TagList tagname="Most Newest" modulelist={newestmoduledata} modulecallback={newestsetData}></TagList>      
            <TagList tagname="Last Updated" modulelist={lastupdatedmoduledata} modulecallback={lastupdatedsetData}></TagList>      
        </div>
    )
}

function TagList(
    { tagname, modulelist, modulecallback } 
        : {tagname: string,modulelist: Get_Modules_Module[],modulecallback: React.Dispatch<React.SetStateAction<Get_Modules_Module[]>>})
{
    return (
        <>
        <h1 className="MainLabel">{tagname}</h1>
        <ul>
            {modulelist.map((mod) => <ModuleLabel module={mod} />)}
            </ul>
        </>
    )
}

function ModuleLabel(
    {module}: {module:Get_Modules_Module} 
)
{

    return (
        <>
            <li className="modulelist">
                <div className="modulelabelbox">

                    <h1 className="modulelabel">{module.module_name}</h1>{}
                </div>
            </li>
        </>
    )
}
