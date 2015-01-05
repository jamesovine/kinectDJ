function [ordering] = getOrdering(labels)

    ordering=zeros(5,1);
    ordering(1)=labels(1);
    pos=2;
    
    for i=2:5
        if (labels(i)~=labels(i-1))
            ordering(pos)=labels(i);
            pos=pos+1;
        end
        if (pos>5)
            break
        end
        
    end
end