function [newCent] =rearrCentroids (centroids)


newCent=zeros(8,3);
for i=1:3
    newCent(1,i)=centroids(8,i);
    newCent(2,i)=centroids(6,i);
    newCent(3,i)=centroids(3,i);
    newCent(4,i)=centroids(1,i);
    newCent(5,i)=centroids(2,i);
    newCent(6,i)=centroids(4,i);
    newCent(7,i)=centroids(5,i);
    newCent(8,i)=centroids(7,i);

end

end