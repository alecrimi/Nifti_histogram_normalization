#include <iostream>
#include "nyul_standardization.h"
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

int main(int argc, char * argv[])
{
    cout << "Be sure that the images have the skull removed" << endl;
    // Verify command line arguments
    if( argc < 3 )
    {
        std::cerr << "Usage: " << std::endl;
        std::cerr << argv[1] << "inputImageFileRef inputImageFile" << std::endl;
        return EXIT_FAILURE;
    }
    // Parse command line arguments
    std::string inputFilename_ref = argv[1];
    std::string inputFilename = argv[2];
     
    // Call the main function
    nyul_standardization(inputFilename_ref, inputFilename);
}
