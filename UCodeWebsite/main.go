package main

import (
	"errors"
	"fmt"
	"html/template"
	"io"
	"io/fs"
	"log"
	"net/http"
	"path/filepath"
	"strings"

	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"

	"embed"
)

//go:embed views/**
var folder embed.FS

//go:embed static/**
var staticfolder embed.FS

type Template struct {
	Template *template.Template
}

func (t *Template) Render(w io.Writer, name string, data interface{}, c echo.Context) error {
	return t.Template.ExecuteTemplate(w, name, data)
}

func newTemplate() *Template {
	htmlFiles, err := parseHTMLFiles("views")
	if err != nil {
		log.Fatal(err)
	}

	// Create a new template
	tmpl := template.New("")

	// Parse HTML files into the template
	for _, file := range htmlFiles {
		// Read the file
		content, err := folder.ReadFile(file)
		if err != nil {
			log.Printf("Error reading file %s: %v\n", file, err)
			continue
		}

		// Parse the file content into the template
		_, err = tmpl.Parse(string(content))
		if err != nil {
			log.Printf("Error parsing template %s: %v\n", file, err)
			continue
		}
	}
	return &Template{
		Template: tmpl,
	}
}

func getfileTypefromExtension(filePath string) string {
	ext := filepath.Ext(filePath)

	contentType := extensionToType[ext]

	if contentType == "" {
		return "application/octet-stream"
	}
	return contentType
}

var extensionToType = map[string]string{
	//web
	".css":  "text/css",
	".html": "text/html",
	".json": "application/json",
	//images
	".png": "image/png",
	".ico": "image/vnd.microsoft.icon",
	".svg": "image/svg+xml",
	//data types
	".txt":  "text/plain",
	".jpeg": "image/jpeg",
	".wav":  "audio/wav",
	".ttf":  "font/ttf",
	//code
	".js": "application/javascript",
}

type PageData struct {
	PageBody template.HTML
}

func executeSubpageTemplateHtml(temp *Template, templatename string, data interface{}) template.HTML {

	str := executeSubpageTemplate(temp, templatename, data)

	r := template.HTML(str)
	return r
}
func executeSubpageTemplate(temp *Template, templatename string, data interface{}) string {

	var outputBuilder strings.Builder
	err := temp.Template.ExecuteTemplate(&outputBuilder, templatename, data)
	if err != nil {
		panic(err)
	}

	return outputBuilder.String()
}

func get_doc_index_html() {

}
func main() {

	e := echo.New()
	e.Use(middleware.Logger())

	tep := newTemplate()
	e.Renderer = tep

	//main
	e.GET("/", func(c echo.Context) error {

		subpage := executeSubpageTemplateHtml(tep, "home", nil)
		pagedata := PageData{PageBody: subpage}

		return c.Render(200, "index", pagedata)
	})
	e.GET("/doc", func(c echo.Context) error {
		file, err := staticfolder.ReadFile("static/doc/index.html")
		if err != nil {
			return err
		}

		return c.HTML(http.StatusOK, string(file[:]))
	})

	e.GET("/doc/**", func(c echo.Context) error {

		url := c.Request().URL.Path

		fmt.Println(url)

		file, err := staticfolder.ReadFile("static/doc/index.html")
		if err != nil {
			return err
		}

		return c.HTML(http.StatusOK, string(file[:]))
	})
	e.GET("/install", func(c echo.Context) error {

		subpage := executeSubpageTemplateHtml(tep, "install", nil)
		pagedata := PageData{PageBody: subpage}

		return c.Render(200, "index", pagedata)

	})
	e.GET("/modules", func(c echo.Context) error {
		subpage := executeSubpageTemplateHtml(tep, "modules", nil)
		pagedata := PageData{PageBody: subpage}

		return c.Render(200, "index", pagedata)
	})
	e.GET("/community", func(c echo.Context) error {
		subpage := executeSubpageTemplateHtml(tep, "community", nil)
		pagedata := PageData{PageBody: subpage}

		return c.Render(200, "index", pagedata)
	})

	//htmx
	e.GET("/htmx/home", func(c echo.Context) error {
		return c.Render(200, "home", nil)
	})
	e.GET("/htmx/install", func(c echo.Context) error {
		return c.Render(200, "install", nil)
	})
	e.GET("/htmx/modules", func(c echo.Context) error {
		return c.Render(200, "modules", nil)
	})
	e.GET("/htmx/community", func(c echo.Context) error {
		return c.Render(200, "community", nil)
	})
	//webapi
	e.POST("/webapi/signup", func(c echo.Context) error {

		// Parse form data
		if err := c.Request().ParseForm(); err != nil {
			return err
		}

		Username := c.Request().FormValue("Username")
		Password := c.Request().FormValue("Password")

		fmt.Println(Username)
		fmt.Println(Password)

		return c.HTML(http.StatusOK, "")
	})
	e.POST("/webapi/login", func(c echo.Context) error {

		// Parse form data
		if err := c.Request().ParseForm(); err != nil {
			return err
		}

		Username := c.Request().FormValue("Username")
		Password := c.Request().FormValue("Password")

		fmt.Println(Username)
		fmt.Println(Password)

		return c.HTML(http.StatusOK, "")
	})
	//static
	e.GET("/static/**", func(c echo.Context) error {

		filetoread := c.Request().URL.Path[len("/"):]
		file, err := staticfolder.ReadFile(filetoread)

		if err != nil {
			if errors.Is(err, fs.ErrNotExist) {
				return c.String(http.StatusNotFound, "File not found")
			}
			return c.String(http.StatusInternalServerError, "Internal server error")
		}
		contentType := getfileTypefromExtension(filetoread)

		return c.Blob(http.StatusOK, contentType, file)
	})

	e.Logger.Fatal(e.Start((":1000")))
}
func parseHTMLFiles(dir string) ([]string, error) {
	var htmlFiles []string

	dirEntries, err := folder.ReadDir(dir)
	if err != nil {
		return nil, err
	}

	for _, entry := range dirEntries {
		if entry.IsDir() {
			subdir := dir + "/" + entry.Name()
			subFiles, err := parseHTMLFiles(subdir)
			if err != nil {
				return nil, err
			}
			htmlFiles = append(htmlFiles, subFiles...)
		} else if filepath.Ext(entry.Name()) == ".html" {
			htmlFiles = append(htmlFiles, dir+"/"+entry.Name())
		}
	}

	return htmlFiles, nil
}
