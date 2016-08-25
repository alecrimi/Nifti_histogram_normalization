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

// Parameter for removing background values and noise outlier
// These are common sensible values
#define MAX_VALUE    255 
#define MAX_BACKGROUND_VALUE   9  // %Assume that the background values are 0 or 1
#define MIN_NOISE_VALUE   254  //%Assume that the noise have values of 254 and 255
//Histogram dependent  parameters set by users. Not critical
//int p1j = 0; // left shoulder mode background template
#define p1i   0  // left shoulder mode background target
#define s1   1 // absolute min value 
#define s2  255 // absolute max value (It should be the same of MAX_VALUE)
// int threshold_noise_postdiff = 20;
#define p2i   (MIN_NOISE_VALUE-1)  // 248; %right shoulder mode foreground target, it is some sort of measurement of the variance

//Build histogram
void build_histogram(int myhist[], int cleaned_voxel_values[], int dummy_counter )
{ 
    //Search for all the possible values of the histogram
    for ( int jj = 0; jj <  MAX_VALUE; jj++   )
    {
        myhist[jj] = 0;  
        //For all the voxels
        for ( int ii = 0; ii <  dummy_counter; ii++   )
        {
            if (cleaned_voxel_values[ii] == jj)
               { 
                myhist[jj]++;
               }
        }
    }
}

//Find the index of the mean in the histogram
void find_max_index (int array[], int count, int &index)
{
   // The initial value for index is -1. If array is emptyy, this would be the returned value as well.
    int max = INT_MIN;
    index = -1;
    //Look into array to find any value greater than the most recent max value.
    for (int n = 0; n < count; n++)
    {
       // If value is found, replace the max value and index
        if (array [n] > max)
        {
            max = array [n];
            index = n;
        }
    }
}

// This function standardize the histogram and remove the background voxels from the histogram
void clean_data( itk::Image<unsigned short,3>::Pointer image_ref, int &counter_brain_voxels_ref)
{
    typedef itk::Image<unsigned short,3> ImageType;    
    //Normalize image within a specific range (e.g. 1-256)
    typedef itk::ImageRegionIterator<ImageType> ItType;
    ItType it_ref( image_ref, image_ref->GetRequestedRegion() );
    //Find max value
    int max_curr_im_val_ref = 0;
    for (it_ref = it_ref.Begin(); !it_ref.IsAtEnd(); ++it_ref)
    { 
        int temp_val = it_ref.Get();
        if (max_curr_im_val_ref < temp_val)
            max_curr_im_val_ref = temp_val;
    }
    //Iteration for Normalization
    for (it_ref = it_ref.Begin(); !it_ref.IsAtEnd(); ++it_ref)
        it_ref.Set( MAX_VALUE * it_ref.Get() / max_curr_im_val_ref ); //in Matlab: converted_data = MAX_VALUE * (converted_data - min(converted_data) )/( max(converted_data) - min(converted_data));  
    
    //Remove background and outliers
    //How many voxel really belong to the brain   //SUBSTITUTE THIS PART WITH A LIST!!!!!!!!
    for (it_ref = it_ref.Begin(); !it_ref.IsAtEnd(); ++it_ref)
    { 
        if (  it_ref.Get() > MAX_BACKGROUND_VALUE &&  it_ref.Get() < MIN_NOISE_VALUE )
            counter_brain_voxels_ref++;
    }
}

//Normalize histogram according to imput means
void normalize_hist(itk::Image<unsigned short,3>::Pointer image, int cleaned_voxel_values[], itk::Image<unsigned short,3>::IndexType* coordinates,  int dummy_counter, int mu_i_index, int mu_s_index)
{
    typedef itk::Image<unsigned short,3> ImageType; 
    int* standardized_hist = new int[dummy_counter];
    // This is the effective histogram normalization step
    for ( int ii = 0; ii <  dummy_counter; ii++   )
    {
        if (cleaned_voxel_values[ii] < mu_i_index) 
            standardized_hist[ii] =  mu_s_index + ( cleaned_voxel_values[ii] - mu_i_index ) * ( s1 - mu_s_index ) / ( p1i - mu_i_index ) ; 
        else  
            standardized_hist[ii] =  mu_s_index + ( cleaned_voxel_values[ii] - mu_i_index ) * ( s2 - mu_s_index ) / ( p2i - mu_i_index ) ; 
    }
    //Put back the normalized values
    for ( int ii = 0; ii <  dummy_counter; ii++   )
        image->SetPixel(coordinates[ii],standardized_hist[ii]);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int nyul_standardization(string inputFilename_ref, string  inputFilename)
{
    std::string outputFilename_ref = "Normalized_"  + inputFilename_ref;
    std::string outputFilename = "Normalized_"  + inputFilename;
    
    typedef itk::Image<unsigned short,3> ImageType;
    // Read Input Ref
    itk::ImageFileReader<ImageType>::Pointer reader_ref = itk::ImageFileReader<ImageType>::New();
    reader_ref->SetFileName( inputFilename_ref);
    try
    { reader_ref->Update(); }
    catch(itk::ExceptionObject & e)
    {
        std::cerr<<e.GetDescription()<<std::endl;
        return EXIT_FAILURE;
    }
    ImageType::Pointer image_ref = reader_ref->GetOutput();
    image_ref->Allocate();
    // Read Input
    itk::ImageFileReader<ImageType>::Pointer reader = itk::ImageFileReader<ImageType>::New();
    reader->SetFileName( inputFilename);
    try
    { reader->Update(); }
    catch(itk::ExceptionObject & e)
    {
        std::cerr<<e.GetDescription()<<std::endl;
        return EXIT_FAILURE;
    }
    ImageType::Pointer image = reader->GetOutput();
    image->Allocate();
    
    /////////////// Reference volume part //////////////
    //Standardize volume 
    int dummy_counter_ref = 0;
    int counter_brain_voxels_ref = 0;
    clean_data( image_ref,   counter_brain_voxels_ref );
    //Build structure containing indices of brain_voxels
    int* cleaned_voxel_values_ref = new int[counter_brain_voxels_ref] ;    
    typedef itk::ImageRegionIterator<ImageType> ItType;
    ItType it_ref( image_ref, image_ref->GetRequestedRegion() );
    for (it_ref = it_ref.Begin(); !it_ref.IsAtEnd(); ++it_ref)
    {  
        if (  it_ref.Get() > MAX_BACKGROUND_VALUE &&  it_ref.Get() < MIN_NOISE_VALUE )
        {  
            cleaned_voxel_values_ref[dummy_counter_ref] = it_ref.Get();
            dummy_counter_ref++;
        }
    }    
    //Build reference histogram
    int* myhist_ref = new int[MAX_VALUE];
    build_histogram(myhist_ref, cleaned_voxel_values_ref, dummy_counter_ref );
    //Find the index of the mean
    int mu_s_index;
    find_max_index(myhist_ref, MAX_VALUE, mu_s_index);
    cout <<  "The mean value of the reference hist. has index " <<  mu_s_index << endl;
 
    ////////////// Input part //////////////////////////
    int dummy_counter = 0;
    int counter_brain_voxels = 0;
    clean_data( image,  counter_brain_voxels);
    //Build structure containing indices of brain_voxels
    int* cleaned_voxel_values = new int[counter_brain_voxels] ;
    ImageType::IndexType* coordinates = new  ImageType::IndexType[counter_brain_voxels];
    typedef itk::ImageRegionIterator<ImageType> ItType;
    ItType it( image, image->GetRequestedRegion() );
    for (it = it.Begin(); !it.IsAtEnd(); ++it)
    {  
        if (  it.Get() > MAX_BACKGROUND_VALUE &&  it.Get() < MIN_NOISE_VALUE )
        {  
            cleaned_voxel_values[dummy_counter] = it.Get();
            coordinates[dummy_counter] =  it.GetIndex();            
            dummy_counter++;
        }
    }    
    //Build moving histogram
    int* myhist = new int[MAX_VALUE];
    build_histogram(myhist, cleaned_voxel_values,  dummy_counter );
    //Find the index of the mean
    int mu_i_index;
    find_max_index(myhist, MAX_VALUE, mu_i_index);
    cout <<  "The mean value of the input hist. has index "  <<  mu_i_index << endl;
    
    ///////// Normalize //////////////////////////////
    // This is the effective histogram normalization
    normalize_hist( image, cleaned_voxel_values, coordinates, dummy_counter, mu_i_index, mu_s_index );

    //Write outputs
    itk::ImageFileWriter<ImageType>::Pointer writer = itk::ImageFileWriter<ImageType>::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(image);
    try 
    {  writer->Update(); } 
    catch (itk::ExceptionObject & e) 
    {
        std::cerr<<e.GetDescription()<<std::endl;
        return EXIT_FAILURE;
    }
    writer->SetFileName(outputFilename_ref);
    writer->SetInput(image_ref);
    try 
    {  writer->Update(); } 
    catch (itk::ExceptionObject & e) 
    {
        std::cerr<<e.GetDescription()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
