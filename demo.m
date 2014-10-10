clear all;close all;clc;

disp('Fast Directional Chamfer Matching Demo');


%//==================================================================
%// Basic Configuration
%//==================================================================
templateEdgeMap = imread('Template/handdrawn_bottle.pgm');

figure(1);
subplot(2,2,1);
imshow(imread('Query/ray.jpg'));
title('Image 1');
subplot(2,2,2);
imshow(imread('Query/ceazanne.jpg'));
title('Image 2');
subplot(2,2,3);
imshow(imread('Query/four.jpg'));
title('Image 3');
subplot(2,2,4);
imshow(imread('Query/pale.jpg'));
title('Image 4');

x = input('Choose the image by number (Default 1) :');

switch x
    case 1
        query = imread('Query/ray_edges.pgm');
        queryColor = imread('Query/ray.jpg');
    case 2
        query = imread('Query/ceazanne_edges.pgm');
        queryColor = imread('Query/ceazanne.jpg');
    case 3
        query = imread('Query/four_edges.pgm');
        queryColor = imread('Query/four.jpg');
    case 4
        query = imread('Query/pale_edges.pgm');
        queryColor = imread('Query/pale.jpg');
    otherwise
        query = imread('Query/ray_edges.pgm');
        queryColor = imread('Query/ray.jpg');
end





threshold = 0.12;
lineMatchingPara = struct(...
    'NUMBER_DIRECTION',60,...
    'DIRECTIONAL_COST',0.5,...
    'MAXIMUM_EDGE_COST',30,...
    'MATCHING_SCALE',1.0,...
    'TEMPLATE_SCALE',0.6761,...
    'BASE_SEARCH_SCALE',1.20,...
    'MIN_SEARCH_SCALE',-7,...
    'MAX_SEARCH_SCALE',0,...
    'BASE_SEARCH_ASPECT',1.1,...
    'MIN_SEARCH_ASPECT',-1,...
    'MAX_SEARCH_ASPECT',1,...    
    'SEARCH_STEP_SIZE',2,...
    'SEARCH_BOUNDARY_SIZE',2,...
    'MIN_COST_RATIO',1.0...    
    );



%//==================================================================
%// Convert edge map into line representation
%//==================================================================
figure(1)
subplot(1,2,1);
imshow(255-templateEdgeMap,[]);
title('The tempalte edge map.');
disp('This is the template shape.');
disp('press Enter to continue...');
pause;

% Set the parameter for line fitting function
lineFittingPara = struct(...
    'SIGMA_FIT_A_LINE',0.5,...
    'SIGMA_FIND_SUPPORT',0.5,...
    'MAX_GAP',2.0,...
    'N_LINES_TO_FIT_IN_STAGE_1',300,...
    'N_TRIALS_PER_LINE_IN_STAGE_1',100,...
    'N_LINES_TO_FIT_IN_STAGE_2',100000,...
    'N_TRIALS_PER_LINE_IN_STAGE_2',1);

% convert the template edge map into a line representation
[lineRep lineMap] = mex_fitline(double(templateEdgeMap),lineFittingPara);

figure(1)
subplot(1,2,2);
imshow(255-lineMap,[]);

% display the top few line segments to illustrate the representation
nLine = size(lineRep,1);
fprintf('\n/*Illustration of the linear representation*/\n');
fprintf('Line ID ( Pt1.x , Pt1.y ) -> ( Pt2.x , Pt2.y )\n');
for i=1:min(nLine,3)
    fprintf('Line %02d ( %d , %d ) -> ( %d , %d )\n',i,...
        lineRep(i,1),lineRep(i,2),lineRep(i,3),lineRep(i,4));
end
fprintf('...\n');
title('The linear representation map.');
disp('This is the linear representation of the template shape.');
disp('press Enter to continue...');
pause;

%//==================================================================
%// FDCM detection
%//==================================================================

figure(2);
hold on;
imshow(queryColor,[]);
title('query image');
disp('This is the query image in which the template shape is used to detect similar object instances.');
disp('press Enter to continue...');
pause;

% Set the parameter for line fitting function
lineFittingPara2 = struct(...
    'SIGMA_FIT_A_LINE',0.5,...
    'SIGMA_FIND_SUPPORT',0.5,...
    'MAX_GAP',2.0,...
    'N_LINES_TO_FIT_IN_STAGE_1',0,...
    'N_TRIALS_PER_LINE_IN_STAGE_1',0,...
    'N_LINES_TO_FIT_IN_STAGE_2',100000,...
    'N_TRIALS_PER_LINE_IN_STAGE_2',1);


template = cell(1);
tempate{1} = lineRep;


[detWinds] = mex_fdcm_detect(double(query),tempate,threshold,...
    lineFittingPara2,lineMatchingPara);


nDetection = size(detWinds,1);
fprintf('\n/*Illustration of the detection result*/\n');
fprintf('Wind ID (  x0 ,  y0 , width , height, cost, count )\n');
for i=1:min(nDetection,2)
    fprintf('Wind %02d ( %d ,  %d ,    %d ,   %d , %2.2f ,  %d  )\n',...
        i,detWinds(i,1),detWinds(i,2),detWinds(i,3),detWinds(i,4),detWinds(i,5),detWinds(i,6));
end
fprintf('...\n');


color = [0 1 0];
lineWidth = 3;
for i=1:size(detWinds)
    sx = detWinds(i,1);
    ex = sx + detWinds(i,3);
    sy = detWinds(i,2);
    ey = sy + detWinds(i,4);
    line([sx ex],[sy sy],'Color',color,'LineWidth',lineWidth);
    line([sx ex],[ey ey],'Color',color,'LineWidth',lineWidth);
    line([sx sx],[sy ey],'Color',color,'LineWidth',lineWidth);
    line([ex ex],[sy ey],'Color',color,'LineWidth',lineWidth);
end
title('Detection result');
disp('This is the detected object isntance.');
disp('The demo is finished.');
