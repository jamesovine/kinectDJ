function [] = plotData(dataSet)
hold all
plot3(dataSet(:,1,1),dataSet(:,2,1),dataSet(:,3,1));
plot3(dataSet(:,1,2),dataSet(:,2,2),dataSet(:,3,2),'r');
plot3(dataSet(:,1,3),dataSet(:,2,3),dataSet(:,3,3),'y');
plot3(dataSet(:,1,4),dataSet(:,2,4),dataSet(:,3,4),'k');
plot3(dataSet(:,1,5),dataSet(:,2,5),dataSet(:,3,5),'c');
plot3(dataSet(:,1,6),dataSet(:,2,6),dataSet(:,3,6),'g');
plot3(dataSet(:,1,7),dataSet(:,2,7),dataSet(:,3,7),'p');
plot3(dataSet(:,1,8),dataSet(:,2,8),dataSet(:,3,8),'m');
plot3(dataSet(:,1,9),dataSet(:,2,9),dataSet(:,3,9),'b');
plot3(dataSet(:,1,10),dataSet(:,2,10),dataSet(:,3,10),'p');


end