# RedGreen

I have a rectangular boundary (blue). Within this space, there is a list of collidable geometries, including contour lines and other rectangles (marked in red). Additionally, I have other rectangles (marked in green) that must not collide with the red ones.

When a green rectangle collides with a red rectangle, I need to determine the nearest valid positions for the green rectangle that ensures it does not overlap with any of the red rectangles and lines.

## Input

Input is read from a space delimited text file.

- Input line with first letter 'g' specifies a green rectangle: center location ( x and y ), width and height.
- Input line with first letter 'r' specifies a red rectangle: center location ( x and y ), width and height.
- Input line with first letter 'b' specifies the boundary points: x1 y1 x2 y2 ...

Example
```
g 5 5 5 5
r 4 4 5 5
b 1 1 60 1 60 60 1 60
```

## Example display

![image](https://github.com/user-attachments/assets/39024686-9c06-4046-aa46-fcd96a740f85)


'm' indicates that the green rectangle has been moved to avoid collision with red recangles.
