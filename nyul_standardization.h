#ifndef NYUL_STANDARDIZATION 
#define NYUL_STANDARDIZATION 

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h" 
#include <iostream>
#include <limits.h>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////
/*
 This program is performing the histogram normalization given a reference volume
 - The program is assuming that the skull is removed!!!!!
 - It is based on the image processing algorithm described in 

 Crimi, Alessandro, et al. "Semi-automatic classification of lesion patterns in patients with clinically isolated syndrome." 2013 IEEE 10th International   
Symposium on Biomedical Imaging. IEEE, 2013.

 This code attempts to use the same variable names of the article.
 
 The algorithm first standardizes both the reference and the input volume to a standard range (e.g. 1-256),
 then the histogram of the input image is stretched to match the reference image.
 the normalized voxels are then mapped back to the input volume and saved. 
 The output is given by the normalized input volume and by the reference volume standardized within the range
 
 Alessandro Crimi INRIA 9/10/2012
 */
////////////////////////////////////////////////////////////////////////////////////////////////

//Build histogram
void build_histogram(int myhist[], int cleaned_voxel_values[], int dummy_counter );
 
//Find the index of the mean in the histogram
void find_max_index (int array[], int count, int &index);
 
// This function standardize the histogram and remove the background voxels from the histogram
void clean_data( itk::Image<unsigned short,3>::Pointer image_ref, int &counter_brain_voxels_ref);
 
//Normalize histogram according to imput means
void normalize_hist(itk::Image<unsigned short,3>::Pointer image, int cleaned_voxel_values[], itk::Image<unsigned short,3>::IndexType* coordinates,  int dummy_counter, int mu_i_index, int mu_s_index);
 
int nyul_standardization(string inputFilename_ref, string  inputFilename);

#endif 
