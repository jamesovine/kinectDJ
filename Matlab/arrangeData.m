%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                       %
%   Function arrangeData                %
%   Takes point data, gathered in oF    %
%   Prepares it for training            %
%                                       %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [seqs]=arrangeData(data)

x=data(:,1);
y=data(:,2);
z=data(:,3);

sizeX=size(x,1);

dataX=zeros(sizeX,40);
dataY=zeros(sizeX,40);
dataZ=zeros(sizeX,40);

prevY=0;
currentY=0;

j=1;
count=1;

for i=1:sizeX
    
    currentY=y(i);
    if i>1
        if abs(currentY-prevY)>100
            j=j+1;
            count=1;
        end            
    end
    
    dataX(count,j)=x(i);
    dataY(count,j)=y(i);
    dataZ(count,j)=z(i);
    
    count=count+1;
    
    prevY=y(i);
    
end

seqs=cell(j,1);

for i=1:j
    
    temp=[dataX(:,i),dataY(:,i),dataZ(:,i)];
    
    seqs{i}=temp;
    
end

for j=1:20
    
    %Remove zero rows from end each sequence
    seqs{j}(all(seqs{j}==0,2),:)=[]
end