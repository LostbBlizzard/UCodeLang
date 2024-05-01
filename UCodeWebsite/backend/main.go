package main

import (
	"embed"
	"fmt"
	"io/fs"
	"log"
	"net/http"
	"strconv"
	"strings"

	"github.com/gin-gonic/contrib/static"
	"github.com/gin-gonic/gin"

	"database/sql"

	_ "github.com/go-sql-driver/mysql"
)

//go:embed build
var staticFS embed.FS

// AddRoutes serves the static file system for the UI React App.

type Getmodules_r struct {
	Id          int    `json:"id"`
	Module_name string `json:"module_name"`
}

func getmodules(c *gin.Context) {

	type FindBy int
	const (
		Newest         FindBy = 0
		LastUpdated    FindBy = 1
		Mostdownloaded FindBy = 2
		SearchTerm     FindBy = 3
	)
	test := c.Param("search")

	var searchterm string
	searchby := Mostdownloaded
	if test == "newest" {
		searchby = Newest
	} else if test == "lastupdated" {
		searchby = LastUpdated
	} else if test == "mostdownloaded" {
		searchby = Mostdownloaded
	} else if strings.HasPrefix(test, "search_query=") {
		searchby = SearchTerm
		searchterm = test[len("search_query="):]

		if searchterm == "" {
			c.IndentedJSON(http.StatusBadRequest, "Error You Cant Search Modules by Term but term is empty")
			return
		}
	} else {
		c.IndentedJSON(http.StatusBadRequest, "Error You Cant Search Modules by '"+test+"'")
		return
	}

	query := ""
	if searchby == Newest {
		query = "SELECT id,module_name FROM modules LIMIT 20"
	} else {
		query = "SELECT id,module_name FROM modules LIMIT 20"
	}

	results, err := database.Query(query)
	if err != nil {
		c.IndentedJSON(http.StatusInternalServerError, "")
		log.Print("found " + err.Error() + " from sql")
		return
	}

	returnitem := make([]Getmodules_r, 0)

	for results.Next() {
		var item Getmodules_r

		err = results.Scan(&item.Id, &item.Module_name)
		if err != nil {
			c.IndentedJSON(http.StatusInternalServerError, "")
			log.Print("error in scaning " + err.Error())
			return
		}
		returnitem = append(returnitem, item)
	}

	c.JSON(http.StatusOK, gin.H{
		"status": "OK",
		"code":   200,
		"data":   returnitem,
	})
}

type Getmodule_r struct {
	Owner_id           string         `json:"owner_id"`
	Module_name        string         `json:"module_name"`
	Module_descrpition sql.NullString `json:"module_descrpition"`
}

func getmodule(c *gin.Context) {

	var moduleid int = 0
	{
		idstr := c.Param("id")

		i, err := strconv.Atoi(idstr)
		if err != nil {
			c.IndentedJSON(http.StatusBadRequest, "id is not a number")
			return
		}
		moduleid = i
	}

	results, err := database.Query("SELECT owner_id,module_name,module_description FROM modules WHERE id = ?", moduleid)

	if err != nil {
		c.IndentedJSON(http.StatusNotFound, "cant find module")
		return
	}

	var r Getmodule_r
	for results.Next() {
		err = results.Scan(&r.Owner_id, &r.Module_name, &r.Module_descrpition)
		if err != nil {
			c.IndentedJSON(http.StatusInternalServerError, "")
			log.Print("error in scaning " + err.Error())
			return
		}
	}

	c.JSON(http.StatusOK, gin.H{
		"status": "OK",
		"code":   200,
		"data":   r,
	})
}
func AddRoutes(router gin.IRouter) {
	embeddedBuildFolder := newStaticFileSystem()
	router.Use(static.Serve("/", embeddedBuildFolder))
	router.GET("/API/modules/:search", getmodules)
	router.GET("/API/module/:id", getmodule)
}

// ----------------------------------------------------------------------
// staticFileSystem serves files out of the embedded build folder

type staticFileSystem struct {
	http.FileSystem
}

var _ static.ServeFileSystem = (*staticFileSystem)(nil)

func newStaticFileSystem() *staticFileSystem {
	sub, err := fs.Sub(staticFS, "build")

	if err != nil {
		panic(err)
	}

	return &staticFileSystem{
		FileSystem: http.FS(sub),
	}
}

func (s *staticFileSystem) Exists(prefix string, path string) bool {
	buildpath := fmt.Sprintf("build%s", path)

	// support for folders
	if strings.HasSuffix(path, "/") {
		_, err := staticFS.ReadDir(strings.TrimSuffix(buildpath, "/"))
		return err == nil
	}

	// support for files
	f, err := staticFS.Open(buildpath)
	if f != nil {
		_ = f.Close()
	}
	return err == nil
}

type User struct {
	id       int    `json:"id"`
	username string `json:"name"`
	email    string `json:"email"`
}
type Module struct {
	Id                 int            `json:"id"`
	Owner_id           int            `json:"owner_id"`
	Module_name        string         `json:"module_name"`
	Module_descrpition sql.NullString `json:"module_descrpition"`
	Module_repository  sql.NullString `json:"module_repository"`
}

var database *sql.DB = nil

func main() {
	db, err := sql.Open("mysql", "root:1234@tcp(127.0.0.1:3306)/ucodelangwebsite")

	// if there is an error opening the connection, handle it
	if err != nil {
		panic(err.Error())
	}

	database = db
	// defer the close till after the main function has finished
	// executing
	defer db.Close()

	severip := ":4000"

	router := gin.Default()

	AddRoutes(router)

	fmt.Println("runing sever on " + severip)
	router.Run(severip)
}
