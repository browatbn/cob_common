/****************************************************************
*
* Copyright (c) 2010
*
* Fraunhofer Institute for Manufacturing Engineering
* and Automation (IPA)
*
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Project name: care-o-bot
* ROS stack name: cob_common
* ROS package name: cob_vision_utils
* Description: Utility functions for Fraunhofer IPA vision library.
*
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Author: Jan Fischer, email:jan.fischer@ipa.fhg.de
* Supervised by: Jan Fischer, email:jan.fischer@ipa.fhg.de
*
* Date of creation: July 2008
* ToDo:
*
* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of the Fraunhofer Institute for Manufacturing
* Engineering and Automation (IPA) nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License LGPL as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License LGPL for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License LGPL along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
****************************************************************/
 
/// @file VisionUtils.h
/// Utility functions for Fraunhofer IPA vision library.
/// @author Jan Fischer
/// @date July, 2010.

#ifndef __IPA_VISIONUTILS_H__
#define __IPA_VISIONUTILS_H__

#include "StdAfx.h"

#ifdef __LINUX__
	#include "cob_vision_utils/GlobalDefines.h"
#else
	#include "cob_common/cob_vision_utils/common/include/cob_vision_utils/GlobalDefines.h"
#endif

#include <opencv/cv.h>
#include <iostream>

namespace ipa_Utils {


/// Combines different matrices row wise, so that the number of
/// columns remains the same and the number of rows increases.
/// @return The stacked matrix
cv::Mat vstack(const std::vector<cv::Mat> &mats);

/// Function to replace the buggy OpenCV 1.1 function.
void InitUndistortMap( const cv::Mat& A, const cv::Mat& dist_coeffs,
                    cv::Mat& mapxarr, cv::Mat& mapyarr );

/// Function to convert a 32 bit, n channel image into a eight bit, 1 channel image.
/// @param source The 32 bit, n channel source image
/// @param dest The resulting 8 bit, 1 channel image
/// @param channel The channel of the source image to process
/// @param min Minimal value that is converted, smaller values are clipped to min
/// @param max Maximal value that is converted, bigger values are clipped to max
/// @return Return code
unsigned long ConvertToShowImage(const cv::Mat& source, cv::Mat& dest, int channel = 1, double min = -1, double max = -1);

/// Function to mask image <code>source</code> with image <code>mask</code>. All Pixels in <code>source</code> will be set to 0
/// if the corresponding pixel in <code>mask</code> is outside the interval <code>[minMaskThresh, maxMaskTresh]</code>.
/// Additionally the source image values can be bounded to <code>[sourceMin, sourceMax]</code>.
/// @param source The source image.
/// @param dest The destination image.
/// @param mask The image used as mask for the source image.
/// @param destMask The resulting destination mask image (32F1).
/// @param minMaskThresh Lower border for masking.
/// @param maxMaskThresh Upper border for masking.
/// @param sourceChannel Channel to be bounded in source image.
/// @param sourceMin Lower border for bounding source image.
/// @param sourceMax Upper border for bounding source image.
unsigned long MaskImage(const cv::Mat& source, cv::Mat& dest, const cv::Mat& mask, cv::Mat& destMask, float minMaskThresh = 0,
						float maxMaskTresh = 20000, int sourceChannel = 1, double sourceMin = -1, double sourceMax = -1);

/// Evaluates a polynomial, calculated with <code>FitPolynomial</code>
/// @param x The function value
/// @param degree The degree of the polynomial
/// @param coefficients Pointer to an array of the <code>degree</code>+1 coefficients fo the polynomial
/// @param y The resulting function value
/// @return Return code
unsigned long EvaluatePolynomial(double x, int degree, double* coefficients, double* y);

/// Filters a 3D range image with help of the amplitude image
/// @param xyzImage A 3 channel, 32bit image, containing the xyz-data, filtered values are set to 0.
/// @param greyImage A 3 channel, 32bit image, containing the amplitude data
/// @param mask The resulting mask image (32F1).
/// @param maskColor The resulting color mask. Pixels are set to different colors if they are filtered or not.
/// @param minMaskThresh Lower border for filtering.
/// @param maxMaskThresh Upper border for filtering.
unsigned long FilterByAmplitude(cv::Mat& xyzImage, const cv::Mat& greyImage, cv::Mat* mask, cv::Mat* maskColor, float minMaskThresh, float maxMaskThresh);

/// Filters tear-off edges from a 3D range image.
/// All tear off edges are masked with a value of 255 in mask image. All
/// other points are 0.
/// @param xyzImage A 3 channel, 32bit image, containing the xyz-data, filtered values are set to 0.
/// @param mask The resulting image mask
/// @param piHalfFraction Angles between (PI/2)/'piHalfFraction' and (2*PI)-(PI/2)/'piHalfFraction'
///                       are not discarded
/// @return Return code
unsigned long FilterTearOffEdges(cv::Mat& xyzImage, cv::Mat* mask, float piHalfFraction = 6);

/// Description
unsigned long FilterSpeckles( cv::Mat& img, int maxSpeckleSize, double _maxDiff, cv::Mat& _buf );

/// Returns mat as HSV or gray image 
/// @param value Value to convert 
/// @param min Minimum for scaling
/// @param max Maximum for scaling
/// @return Vector containing RGB values
cv::Vec3b GrayColorMap(double value, double min, double max);

/// Returns mat as HSV or gray image 
/// @param img_32F Float matrix
/// @return Colorcoded image
cv::Mat GetColorcoded(const cv::Mat& img_32F);

/// Returns mat as HSV or gray image 
/// @param img_32F Float matrix
/// @param min Minimum for scaling
/// @param max Maximum for scaling
/// @return Colorcoded image
cv::Mat GetColorcoded(const cv::Mat& img_32F, double min, double max);

/// Save OpenCV matrix in binary format.
/// @param mat The OpenCV mat data structure
/// @param filename The filename
/// @return Return code
unsigned long SaveMat(cv::Mat& mat, std::string filename);

/// Load OpenCV matrix in binary format.
/// @param mat The OpenCV mat data structure
/// @param filename The filename
/// @return Return code
unsigned long LoadMat(cv::Mat& mat, std::string filename);

// Generator that yields an increasing sequence of integers
class UniqueNumber {
public:
  int current;
  UniqueNumber();
  int operator()();
};

// Returns random subset of input vector. Will not create duplicates.
// @param v Input vector
// @param n Number of items to return
// @return Vector of same type as input vector (length n)
template<class T> std::vector<T> takeRandomN(const std::vector<T> &v, int n)
{
    std::vector<int> indices(v.size());
    std::generate(indices.begin(), indices.end(), UniqueNumber());
    std::random_shuffle(indices.begin(), indices.end());
    
    std::vector<T> ret;
    for (int i = 0; i < n; i++)
    {
        ret.push_back(v[indices[i]]);
    }

    return ret;
}

} // end namespace __IPA_VISIONUTILS_H__


#endif
