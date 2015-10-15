plot([0 .5 1 1.8 2.6 3], [12/20 11/20 10/20 7.5/20 8/40 4.5/40], 'r.');

seconds = 3*60;
ticksPerSecond = 1000;
ticksBetweenShifts = 12/20*ticksPerSecond;
x = [];
y = [];
t = 0;
while t < ticksPerSecond*seconds
    t = t + ticksBetweenShifts;
    ticksBetweenShifts = ticksBetweenShifts-1;
    if ticksBetweenShifts < 1
        ticksBetweenShifts = 1
    end
    x = [x t/ticksPerSecond/60];
    y = [y ticksBetweenShifts/ticksPerSecond];
end
hold on;
plot(x, y);
