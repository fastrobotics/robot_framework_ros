// 1. Define the x-axis values from 0 to 2*pi with steps of 0.1
x = 0 : 0.1 : 2*%pi;

// 2. Define the y-axis values as the sine of x
y = sin(x);

// 3. Generate the 2D plot
plot(x, y);
