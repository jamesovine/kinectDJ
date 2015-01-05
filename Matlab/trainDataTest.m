%Compute average log likelihood to use as
%threshold in openFrameworks

function [likeAv] = trainDataTest(T , labelData)

    likeSum=0;
    likelihood=1;

    for s=1:10
        
        for i=2:5
            likelihood=likelihood * T(labelData(s,i-1), labelData(s,i));
        end
        
        likelihood = log(likelihood);
        disp(likelihood);
        likeSum = likeSum+likelihood;
        disp(likeSum);
        likelihood = 1;
        
    end

    likeAv=likeSum/10;

end