import "./App.css";
import Navbar from "./components/Navbar";
import Docs from "./pages/Docs";
import Froms from "./pages/Froms";
import Home from "./pages/Home";
import LoginIn from "./pages/LogIn";
import Module from "./pages/Module";
import Modules from "./pages/Modules";
import Playground from "./pages/Playground";
import { Route, Routes } from "react-router-dom";
import SignUp from "./pages/SignUp";

function App() {
  return (
    <>
      <Navbar />
      <div className="container">
        <Routes>
          <Route path="/" element={<Home />}></Route>
          <Route path="/playground" element={<Playground />}></Route>
          <Route path="/modules" element={<Modules />}></Route>
          <Route path="/module/:id" element={<Module />}></Route>
          <Route path="/login" element={<LoginIn />}></Route>
          <Route path="/signup" element={<SignUp />}></Route>
          <Route path="/frorms" element={<Froms />}></Route>
          <Route path="/docs" element={<Docs />}></Route>
        </Routes>
      </div>
    </>
  );
}

export default App;
