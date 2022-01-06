# Assignment 2 - Part 2
Shahar Tefler & Iris Berger

## 1 - Ambient Light and GUI controls
### GUI
For starters, we added a class to describe a light point. It holds the source point, lighting type


## 2 - Diffuse Light + Flat Shading
In Flat shading we culculatr the shade of every polygon of the model based on the angle between the polygon’s face's normal and the direction of the light source,
and their respective colors.
In this shading every face has his own color. 
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
we culculatet the normal for every verticx by weighted average of all existing normals for each vertex.

## 4 - 

## 5 - Specular Light + Phong Shading

## 6 - Directional Light

## 7 - Results


## 8 - Post Processing

## *fin*
This is a tradition by now:

![gif](part1_images/happy_gif.gif)
