# RedGreen

I have a rectangular boundary (blue). Within this space, there is a list of collidable geometries, including contour lines and other rectangles (marked in red). Additionally, I have other rectangles (marked in green) that must not collide with the red ones.

When a green rectangle collides with a red rectangle, I need to determine the nearest valid positions for the green rectangle that ensures it does not overlap with any of the red rectangles and lines.

## Input

Input is read from a space delimited text file.

Each input line specifies a rectangle: color, center location ( x and y ), width and height.

` red|green x y w h`

Example
```
g 5 5 5 5
r 4 4 5 5
```

## Example display

![image](https://github.com/user-attachments/assets/63047714-a226-4e9c-8d4d-b5a17b30e4c7)


'm' indicates that the green rectangle has been moved to avoid collision with red recangles.
