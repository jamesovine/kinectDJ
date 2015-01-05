function [] =plotCentroids (centroids)

hold all

for i=1:size(centroids)
    
    if i==1
        plot(centroids(i,1),centroids(i,2),'xr');
    end
    
    if i==2
        plot(centroids(i,1),centroids(i,2),'xg');
    end
    if i==3
        plot(centroids(i,1),centroids(i,2),'xb');
    end
    if i==4
        plot(centroids(i,1),centroids(i,2),'xc');
    end
    if i==5
        plot(centroids(i,1),centroids(i,2),'xk');
    end
end

hold off



end