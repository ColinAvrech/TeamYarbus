/*
This will contain the combined headers for all collision functions
Types of collisions:
Point - Point
Point - Circle
Point - Box (maybe)

Circle - Point (same as point - circle)
Circle - Circle
Circle - Box (maybe)

Box - Point (same as point - box)
Box - Circle (same as circle box)
Box - Box

For all duplicate functions, use function overloading.
Maybe use operator* for collision
Both overloaded functions will call the same function internally
and return the returned value. The headers for the shared functions
go in these files.

Note: Box collisions may not be required in our game so no need to code 
that stuff for now.
*/