[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=5969051&assignment_repo_type=AssignmentRepo)
# Computer Graphics 203.3710
## Students Team data

Name: 

Student Number: 


Name: 

Student Number: 

### Lecturer
*Roi Poranne*

[roiporanne@cs.haifa.ac.il](mailto:roiporanne@cs.haifa.ac.il)

URL: [https://roipo.github.io/](https://roipo.github.io/)

Office Hours: By appointment.

### Lectures:
Sundays at 16:00pm - 20:00pm

## Course Description

This course provides an introduction to the foundations of Computer Graphics. We will cover the basic mathematical concepts, such as 2D and 3D transformations, shading models, and rendering techniques. The ultimate goal of this course is to develop a basic model viewer.

Students will experiment with modern graphics programming and build a renderer using C++ and OpenGL.

By the end of the course, you will be able to:

* Explain and apply the fundamental mathematical concepts used in rendering.
* Implement a basic graphics pipeline based on rasterization.
* Develop simple graphics programs in C++ using OpenGL and GLSL.

*Textbook*:
Interactive Computer Graphics: A Top-Down Approach with Shader-Based OpenGL ,6th Edition

## General Instructions

### Installing Git and CMAKE
Before we can begin, you must install Git, a version control system which you need for handing in your assignments, and for keeping track of your progress. We refer you to the online [Pro Git book](https://git-scm.com/book/en/v2) for more information. There you will also find [instructions](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git]) on how to to install it. We recommend using [SourceTree](https://www.sourcetreeapp.com/) for as a GUI for Git.

We use CMake for cross-platform builds. You can download it from: [https://cmake.org/download/](https://cmake.org/download/), but we recommend installing it with a package manager instead.

On Windows use  [Chocolatey](https://chocolatey.org/install), run command line as administrator and type
```
choco install cmake.install
```
On Debian/Ubuntu:
```
sudo apt-get install cmake
```
or with MacPorts on macOS:
```
sudo port install cmake.
```

### Cloning the Assignment Repository
Before you can clone your private assignment repository, you need to have an active [Github](https://github.com/) account. You can then create your own private repository or join and existing one by following the link we sent you via moodle. The name of the repository will always have the form "mesh-viewer-'Your_Team_Name'", where 'Your_Team_Name' is replace by the name you chose.

Next, you will need to clone it to your computer, either by using SourceTree, or by typing the following in command line.
```
git clone --recursive https://github.com/ComputerGraphics2020/mesh-viewer-'Your_Team_Name'.git
```
This can take several minutes.

Next, go into the newly created folder, and add the base assignment repository as a remote (pay attention to capital letters):
```
cd mesh-viewer-'Team_Name'
git remote add base https://github.com/HaifaGraphicsCourses/ComputerGraphicsCourse.git
```

Note: You only have to do this once. This will allow you to recieve updates to the repository from us.


You should now have your local clone of the assignment repository. Take a look at the new repository folder and open 'README.md'. It contains the text you are currently reading. Please fill in your name and student number (for both team members) at the top of this file and save. Then, you need to stage, commit and push the changes:
```
git add README.md
git commit -m "Adjust README.md"
git push
```
**Never push large files to the remote repository! You should almost always only push code. Never push executables or build data**

Please refer to the git book for an explanation on the differences between those steps.
You should be able to see your name online on your private repository: https://github.com/ComputerGraphicsCourse/mesh-viewer-'Your_Team_Name'.git

### Building the code
Use the CMAKE gui as explained in class to create a visual studio solution, or use your compiler/IDE of choice.
**Never push the build folder to the remote repository!**

Once the solution has been generated, open it with visual studio. Check that it compiles and runs.

### Workflow
In general, you should use Git to commit your changes as often as possible. This will help you to backtrack bugs and also serve as a backup mechanism. For more information we refer you to the [Pro Git book](https://git-scm.com/book/en/v2/Git-Basics-Recording-Changes-to-the-Repository).

In case there are updates to the skeleton, we will notify you. Updates can be pulled from the base repository by entering the command:
```
git pull base master
```

### Solution Submission

To submit your final solution of the assignment please add the following commit message: "Solution assignment X". E.g:
```
git commit -m "Solution assignment X"
git push
```