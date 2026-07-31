
// animation_point_with_realtime.sce

clear; xdel(winsid());

// Create motion data -- (1)
t = 0:0.01:1;     // Time data
x = sin(2*%pi*t); // Position data

// Draw initial figure -- (2)
h_fig = figure;
h_fig.background = 8;
h_point = plot(x(1), 0, 'Marker', 'o', 'MarkerSize', 20,..
    'MarkerEdgeColor', 'blue', 'MarkerFaceColor', 'blue');
h_axes = gca();
h_axes.data_bounds = [-1.5, -1.5; 1.5, 1.5];

// Animation Loop -- (3)
realtimeinit(0.1); // set time unit duration, in seconds
realtime(0);       // set current time to 0
for i = 1:length(x)
    drawlater();
    h_point.data = [x(i), 0];
    realtime(i); // wait until i time units (0.1*i seconds)
    drawnow();
end
