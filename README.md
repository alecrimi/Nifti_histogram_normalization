This folder contains the source code for image processing steps described in the paper:

@inproceedings{crimi2013semi,
  title={Semi-automatic classification of lesion patterns in patients with clinically isolated syndrome},
  author={Crimi, Alessandro and Commowick, Olivier and Ferr{\'e}, Jean-Christophe and Maarouf, Adil and Edan, Gilles and Barillot, Christian},
  booktitle={2013 IEEE 10th International Symposium on Biomedical Imaging},
  pages={1102--1105},
  year={2013},
  organization={IEEE}
}
-------------------------------------
This script assumes that the skull has already been stripped (e.g. with the bet script of FSL).
The given files are:

- nyul_standardization.cpp and nyul_standardization.h 
The core algorithm is given by these files. 

- test_nyul.cpp
An example program which expects as inputs two 3D volumes given as NIFTI or Analyze files.

- Make and CMake files (CMakeLists.txt)
use "ccmake" . and then "make"

Requirements:
  ITK library 3.x version, future versions might have compability issues.

How to run it:
nyul_standardization reference.nifti input.nifti 
The test data are taken from the free ICBM dataset


All code is given freely as GNU/General Public License v. 3.0
https://www.gnu.org/licenses/gpl-3.0.html
