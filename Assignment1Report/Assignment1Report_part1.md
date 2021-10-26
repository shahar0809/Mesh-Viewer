# Assignment1 - Part 1
## Implementation of Bresenham's algorithm
In order to implement the algorithm, we split into cases:

- If the slope is bellow 1:
    - If the first point is before second point on the X coordinate (Standard case)
    - If the first point is after the second point 
- If the slope is above 1:
    - If the first point is before second point on the X coordinate (Standard case)
    - If the first point is after the second point 

Once we split it into cases, it was easier to implement the algorithm.
- If the first point is before the second one, we simply switch them.

	![switch_points](/Assignment1Report/part1_images/switch_points.jpeg) 

- If the slope is above 1, we switch between the X and Y values.

	![switch_coordinates](/Assignment1Report/part1_images/switch_coordinates.jpeg)

Then, we implemented the algorithm as presented in class, for the standard case.

- In each iteration we choose between 2 candidates:
    - (Xi, Yi)
    - (Xi, Y(i+1))
- We choose them according to the decision parameter, and then update the parameter according to our choice.

## Full Implementation
```C++
int x, y;

	int deltaX = p2.x - p1.x;
	int deltaY = p2.y - p1.y;

	// Calculate absoloute values of deltas
	int abs_deltaX = fabs(deltaX);
	int abs_deltaY = fabs(deltaY);

	// Precalculate the decision parameters
	int decisionParam_X = 2 * abs_deltaY - abs_deltaX;
	int decisionParam_Y = 2 * abs_deltaX - abs_deltaY;

	// Slope < 1
	if (abs_deltaY <= abs_deltaX)
	{
		// If the first points is after the second one (on X coordinates), switch them
		if (deltaX < 0)
		{
			DrawLine(p2, p1, color);
			return;
		}

		y = p1.y;
		// Go over all X's from start to end
		for (x = p1.x; x < p2.x; x++)
		{
			if (decisionParam_X < 0)
			{
				decisionParam_X += 2 * abs_deltaY;
			}
			else
			{
				// If the slope is positive, we want to increase Y
				if ((deltaX < 0 && deltaY < 0) || (deltaX > 0 && deltaY > 0))
				{
					y++;
				}
				// Otherwise, we'll decrease it (in the direction of the line)
				else
				{
					y--;
				}
				decisionParam_X = decisionParam_X + 2 * (abs_deltaY - abs_deltaX);
			}
			PutPixel(x, y, color);
		}
	}
	// We go over the Y coordinates, instead of X ( Slope >= 1)
	else
	{
		int endY;
		glm::ivec2 start, end;

		// Change points order if necessary
		if (deltaY < 0)
		{
			start = p2;
			end = p1;
		}
		else
		{
			start = p1;
			end = p2;
		}

		int x = start.x;
		for (int y = start.y; y < end.y; y++)
		{
			if (decisionParam_Y < 0)
			{
				decisionParam_Y += 2 * abs_deltaX;
			}
			else
			{
				// If the slope is positive, we want to increase X
				if ((deltaX < 0 && deltaY < 0) || (deltaX > 0 && deltaY > 0))
				{
					x = x + 1;
				}
				// Otherwise, we'll decrease it (in the direction of the line)
				else
				{
					x = x - 1;
				}
				decisionParam_Y += 2 * (abs_deltaX - abs_deltaY);
			}
			PutPixel(x, y, color);
		}
	}
```

## Our Sanity Check

In order to check our algorithm, we created a circle with a defined center `(X0, Y0)`, and a radius `R`.
Then, we drew lines from the center to points on the circle with step size `a` using basic geometry.

![sanity_check](/Assignment1Report/part1_images/drawing_cake.jpeg)

## Cake Drawing

We decided to draw a cake with candles, because, why not?

![switch_coordinates](/Assignment1Report/part1_images/switch_coordinates.jpeg)
	
## The End
![gif](https://www.google.com/url?sa=i&url=https%3A%2F%2Fgifer.com%2Fen%2F1VFb&psig=AOvVaw2oSTLkzyMJthJ7Sr8nRN-Q&ust=1635367465635000&source=images&cd=vfe&ved=0CAsQjRxqFwoTCLjO7dn46PMCFQAAAAAdAAAAABAb)
