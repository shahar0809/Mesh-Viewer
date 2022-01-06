# Assignment 2 - Part 2
Shahar Tefler & Iris Berger

## 1 - Ambient Light and GUI controls
### GUI
For starters, we added a class to describe a light point. It holds the source point.



## 2 - Diffuse Light + Flat Shading
According to the PHong Reflection Model:

In Flat shading we calculate the shade of each polygon of the model, using:
* The angle between the face's normal and the light direction
* Light's color
* Model's material color

Because we use face normals, each face has its own color, AKA, Flat Shading.
We can see that in this shading the transition of the color is not smooth (we can see some of the faces lines).

```cpp
```



In diffuse light we have a light source. when the light touches the surface the ligth
Then the light reflected from it.
We can see that the position of the light source changes the amount of light reflected from the surface.

```cpp
```


Exampels for model with diffuse ligth and flat shading:


## 3 - Gouraud shading
With gouraud ahsding we tring to achieve smooth lighting for our model.

How we implement the Gouraud shading?
For this shading we ouse the vertix normals. 
Every vertix can have several normal so we do an interpolation on all of the to find only one normal.


```cpp
```

We can see that in this shading the transition of the colors are mash more smoother than in the flat shading.


## 4 - 

## 5 - Specular Light + Phong Shading
Specular light is only making the mobel surfaces looke brilliant. 
In this case if you are closer to the camera the direction of the reflection will be seen in high intensity
More.

Phong shading is a per-fragment color computation. 
The vertex shader provides the normal and position data as out variables to the fragment shader. 
The fragment shader then interpolates these variables and computes the color. 

## 6 - Directional Light


## 7 - Results


## 8 - Post Processing

## fin
This is a tradition by now:

![gif](part1_images/happy_gif.gif)