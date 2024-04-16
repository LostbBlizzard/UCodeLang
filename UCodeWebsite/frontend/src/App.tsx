
import './App.css';
import Navbar from './components/Navbar';
import Docs from './pages/Docs';
import Froms from './pages/Froms';
import Home from './pages/Home';
import Modules from './pages/Modules';
import Playground from './pages/Playground';
import { Route,Routes } from 'react-router-dom';

function App() { 
  return (
    <>
      <Navbar />
      <div className='container'>
        <Routes>
          <Route path="/" element={<Home />}></Route>
          <Route path="/playground" element={<Playground />}></Route>
          <Route path="/modules" element={<Modules />}></Route>
          <Route path="/frorms" element={<Froms />}></Route>
          <Route path="/docs" element={<Docs />}></Route>
        </Routes>                  
      </div>
    </>
  )
}

export default App
