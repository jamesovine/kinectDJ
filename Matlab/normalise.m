function [bigAvX,bigAvY,bigAvZ]=normalise(seqs)

sumX=0;
sumY=0;
sumZ=0;

bigAvX=0;
bigAvY=0;
bigAvZ=0;

for s=1:10
    
    for i=1:60
        sumX=sumX+seqs(i,1,s);
        sumY=sumY+seqs(i,2,s);
        sumZ=sumZ+seqs(i,3,s);
    end
    
    avX=sumX/60;
    avY=sumY/60;
    avZ=sumZ/60;
    
    for i=1:60
        seqs(i,1,s)=seqs(i,1,s)-avX;
        seqs(i,2,s)=seqs(i,2,s)-avY;
        seqs(i,3,s)=seqs(i,3,s)-avZ;
    end
    
    bigAvX=bigAvX+avX;
    bigAvY=bigAvY+avY;
    bigAvZ=bigAvZ+avZ;
    
    sumX=0;
    sumY=0;
    sumZ=0;
    
end

bigAvX=bigAvX/10;
bigAvY=bigAvY/10;
bigAvZ=bigAvZ/10;

end