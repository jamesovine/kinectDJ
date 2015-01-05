function [] =plotClusters (clusteredData)

hold all

for i=1:size(clusteredData)
    
    if clusteredData(i,4)==1
        plot(clusteredData(i,1),clusteredData(i,2),'xr');
    end
    
    if clusteredData(i,4)==2
        plot(clusteredData(i,1),clusteredData(i,2),'xg');
    end
    if clusteredData(i,4)==3
        plot(clusteredData(i,1),clusteredData(i,2),'xb');
    end
    if clusteredData(i,4)==4
        plot(clusteredData(i,1),clusteredData(i,2),'xy');
    end
    if clusteredData(i,4)==5
        plot(clusteredData(i,1),clusteredData(i,2),'xk');
    end
    if clusteredData(i,4)==6
        plot(clusteredData(i,1),clusteredData(i,2),'ok');
    end
    if clusteredData(i,4)==7
        plot(clusteredData(i,1),clusteredData(i,2),'or');
    end
    if clusteredData(i,4)==8
        plot(clusteredData(i,1),clusteredData(i,2),'ob');
    end
end

hold off



end