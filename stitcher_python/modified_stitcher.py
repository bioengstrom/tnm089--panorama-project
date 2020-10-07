from imutils import paths
import numpy as np
import cv2

################################
#####      References      #####
################################

# Python openCV - stitcher test
# https://github.com/opencv/opencv/blob/master/modules/stitching/misc/python/test/test_stitching.py

# Python openCV - detailed example 
# https://github.com/opencv/opencv/blob/master/samples/python/stitching_detailed.py
# sample images: https://github.com/opencv/opencv/tree/master/samples/data


# C++ openCV - stitcher implementation
# https://github.com/opencv/opencv/blob/master/modules/stitching/src/stitcher.cpp

################################
#####    Implementation    #####
################################


class modified_stitcher():

    def __init__(self):
        
        super().__init__()


        finder = cv2.SIFT_create
        feature_finder = cv2.feature_f

        estimator = cv2.detail_HomographyBasedEstimator()
        matcher = cv2.detail_BestOf2NearestMatcher()
        adjuster = cv2.detail_BundleAdjusterRay()

        seam_finder = cv2.detail_GraphCutSeamFinder("COST_COLOR")
        compensator = cv2.detail.ExposureCompensator_createDefault(cv2.detail.ExposureCompensator_GAIN_BLOCKS)
        blender = cv2.detail.Blender_createDefault(cv2.detail.Blender_MULTI_BAND)




