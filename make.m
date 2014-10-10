mex -v -c Fitline/LFLineFitter.cpp -I.
mex -v -c Fitline/LFLineSegment.cpp -I.
mex -v -c Image/DistanceTransform.cpp -I.
mex -v -c Fdcm/EIEdgeImage.cpp -I.
mex -v -c Fdcm/LMDirectionalIntegralDistanceImage.cpp -I.
mex -v -c Fdcm/LMDisplay.cpp -I.
mex -v -c Fdcm/LMDistanceImage.cpp -I.
mex -v -c Fdcm/LMLineMatcher.cpp -I.
mex -v -c Fdcm/LMNonMaximumSuppression.cpp -I.
mex -v -c Fdcm/MatchingCostMap.cpp -I.

% make fdcm
mex -v mex_fdcm_detect.cpp...
    LFLineFitter.obj LFLineSegment.obj DistanceTransform.obj ...
    EIEdgeImage.obj LMDirectionalIntegralDistanceImage.obj LMDisplay.obj...
    LMDistanceImage.obj LMLineMatcher.obj LMNonMaximumSuppression.obj...
    MatchingCostMap.obj

% make fitline
mex -v mex_fitline.cpp LFLineFitter.obj LFLineSegment.obj

delete *.obj