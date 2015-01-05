function [T, E, LL] = forBackDHMM(data,centroids,N)

%Baum-Welch Algorithm for training data

%no. of iterations of algorithm
reps=100;

%convergence threshold
thresh=0.001;

%observations
O=centroids

%Randomly initialise A, B and pi
%pi is vector describing initial state distribution
pi=zeros(1,N);

%A is the stochatic transtition matrix for the HMM
A=zeros(N,N);

%B is the emission matrix. B(:,j) describes the PDF for 
%observations at state j
B=zeros(N,N);

%repeat until convergence
for c=1:reps
    
    %forward-backward loop
    for i=1:N
        
    end
end



end