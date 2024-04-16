package main

import (
	"fmt"
	"os"
	"os/exec"
	"runtime"

	cp "github.com/otiai10/copy"
)

type platform int

const (
	Windows platform = 0
	Linux            = 1
	MacOs            = 2
)

func getcurrentplatfrom() platform {
	if runtime.GOOS == "windows" {
		return Windows
	} else if runtime.GOOS == "linux" {
		return Linux
	} else if runtime.GOOS == "darwin" {
		return MacOs
	}
	return Linux
}
func tostring(platform platform) string {
	switch platform {
	case Linux:
		return "linux"
	case Windows:
		return "windows"
	case MacOs:
		return "darwin"
	}
	return ""
}
func main() {
	fmt.Println("Starting Build")

	platformtobuild := getcurrentplatfrom()
	isdebugbuild := true

	for _, item := range os.Args {
		if item == "windows" {
			platformtobuild = Windows
		} else if item == "linux" {
			platformtobuild = Linux
		} else if item == "macos" {
			platformtobuild = MacOs
		} else if item == "dist" {
			isdebugbuild = false
		}
	}
	{
		buildtype := tostring(platformtobuild)
		if isdebugbuild {
			buildtype += ", debug"
		} else {
			buildtype += ", dist"
		}
		fmt.Println("Building for " + buildtype)
	}

	{
		build := exec.Command("npm", "run", "build")
		build.Dir = "./frontend"

		output, err := build.CombinedOutput()
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(string(output))
	}

	cp.Copy("./frontend/dist", "./backend/build")

	{
		direrr := os.MkdirAll("./backend/output", os.ModePerm)

		if direrr != nil {
			fmt.Println(direrr)
			return
		}

		outfileext := ""
		//osstr := tostring(platformtobuild)
		if !isdebugbuild {
			outfileext += "dist"
		}
		if platformtobuild == Windows {
			outfileext += ".exe"
		} else if platformtobuild == MacOs {
			outfileext += ".app"
		}

		build := exec.Command("go", "build", "-o", "./output/ucodelangwebsite"+outfileext)
		build.Dir = "./backend"
		//build.Env = []string{"GOOS=" + osstr}
		if !isdebugbuild {
			//build.Args = append(build.Args, "-ldflags \"-s -w\"")
		}

		output, err := build.CombinedOutput()
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(string(output))
	}
}
