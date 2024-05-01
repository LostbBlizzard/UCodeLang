import "./styles.css"
import {Link,useMatch,useResolvedPath,To} from "react-router-dom"

function Navbar() {
    return (
    <>
            <nav className="navbar">
                <img src="Logo.png" alt="UCodeLang Logo" width={55} height={55}></img>
                <a href="/" className="site-title">UCodeLang</a> 
                <ul>
                    <NavLink to="/">Home</NavLink>
                    <NavLink to="/playground">PlayGround</NavLink>
                    <NavLink to="/modules">Modules</NavLink>
                    <NavLink to="/frorms">Frorms</NavLink>
                    <NavLink to="/docs">Docs</NavLink>
                    
                    <li>
                        <a href="https://github.com/LostbBlizzard/UCodeLang">GitHub</a>
                    </li>
                </ul>
        </nav>
    </>
    )
}
function NavLink({to, children, ...props} :{to: To,children:any,props?:any} ) {
    const resovledPath = useResolvedPath(to);
    const isActive = useMatch({ path: resovledPath.pathname, end: true })
    return (
        <li className={isActive ? "active" : ""}>
            <Link to={to} {...props}>{ children}</Link>
        </li>
    )
}
export default Navbar;