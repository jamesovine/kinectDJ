function [avX,avY,avZ, Trans, Emit, labelData, lik, centroids, ordering] = trainGest (seqs)
%train Gest, computes the emission and transition matrices for
%the Hidden Markov Model for gesture recognition

%Number of states
N=5;

%Size of output alphabet. Equal when only one possible gesture
M=N;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Need to normalise point data to origin  %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sumX=0;
sumY=0;
sumZ=0;

for s=1:10
    
    for i=1:60
        sumX=sumX+seqs(i,1,s);
        sumY=sumY+seqs(i,2,s);
        sumZ=sumZ+seqs(i,3,s);
    end
   
    %Get the average points
    avX=sumX/60;
    avY=sumY/60;
    avZ=sumZ/60;
    
    
    %Normalise
    for i=1:60
        seqs(i,1,s)=seqs(i,1,s)-avX;
        seqs(i,2,s)=seqs(i,2,s)-avY;
        seqs(i,3,s)=seqs(i,3,s)-avZ;
    end
    
    sumX=0;
    sumY=0;
    sumZ=0;
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Get average sequence, use kmeans to get centroids %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

avSeq=zeros(60,3);
sumSeqX=0;
sumSeqY=0;
sumSeqZ=0;

for i=1:60
    for j=1:10
        sumSeqX=sumSeqX+seqs(i,1,j);
        sumSeqY=sumSeqY+seqs(i,2,j);
        sumSeqZ=sumSeqZ+seqs(i,3,j);
    end
    avSeq(i,1)=sumSeqX/10;
    avSeq(i,2)=sumSeqY/10;
    avSeq(i,3)=sumSeqZ/10;
    sumSeqX=0;
    sumSeqY=0;
    sumSeqZ=0;
end


%Create a sequence of 5 states
discreteAvSeq = zeros (5,3);

discSumX=0;
discSumY=0;
discSumZ=0;

%Populate the new discrete sequence
for i=1:5
    
    for j=1:12
        discSumX = discSumX + avSeq(j*i,1);
        discSumY = discSumY + avSeq(j*i,2);
        discSumZ = discSumZ + avSeq(j*i,3);
    end
    
    discreteAvSeq(i,1) = discSumX/12;
    discreteAvSeq(i,2) = discSumY/12;
    discreteAvSeq(i,3) = discSumZ/12;
    
    
    discSumX=0;
    discSumY=0;
    discSumZ=0;
    
end


%Perform K-means clustering to get 5 possible discrete states
[labels, centroids] = kmeans(discreteAvSeq, N);


%Generate sequence structure to pass to hmmtrain
labelData=zeros(10,5);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Cluster the raw data into one of 5 from kmeans    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

minDist=inf;

%build a new seqs object containing reduced states

discreteSeqs = zeros(5,3,10);

discSeqSumX=0;
discSeqSumY=0;
discSeqSumZ=0;

for s=1:10
    for i=1:5

        for j=1:12
            discSeqSumX = discSeqSumX + seqs(j*i,1,s);
            discSeqSumY = discSeqSumY + seqs(j*i,2,s);
            discSeqSumZ = discSeqSumZ + seqs(j*i,3,s);
        end

        discreteSeqs(i,1,s) = discSeqSumX;
        discreteSeqs(i,2,s) = discSeqSumY;
        discreteSeqs(i,3,s) = discSeqSumZ;


        discSeqSumX=0;
        discSeqSumY=0;
        discSeqSumZ=0;

    end
end



for s=1:10
    
    for i=1:5
        
        for c=1:N
            dist = sqrt( (centroids(c,1)-discreteSeqs(i,1,s))^2 + (centroids(c,2)-discreteSeqs(i,2,s))^2 + (centroids(c,3)-discreteSeqs(i,3,s))^2 );
            if (dist<minDist)
                minDist=dist;
                labelData(s,i)=c;
            end
            
        end
        minDist=inf;
        
    end
    
end

ordering=getOrdering(labels);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Initialise transition matrix to something simple  %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%Initialise the transition and emission matrices
T=rand(N,N);

E=zeros(N,N);

for i=1:N
    E(i,i)=1;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%do the training                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Built in matlab command hmmtrain. Uses Baum-Welch algorithm.
%Terminates when tolerance is met or maxiterations reached.

[Trans,Emit]=hmmtrain(labelData,T,E, 'Maxiterations', 1000);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Do testing on training data to compute likelihood threshold %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


lik= trainDataTest(Trans,labelData);

end