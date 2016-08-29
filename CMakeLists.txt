cmake_minimum_required(VERSION 2.8)
#------------------------------------------------------------
PROJECT(test_nyul)
 
 
 
#SET(ITK_DIR "/udd/medical/ITK2/Linux")
#INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
 
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
    INCLUDE(${ITK_USE_FILE})

  ADD_EXECUTABLE (nyul_standardization test_nyul.cpp nyul_standardization.h nyul_standardization.cpp )

  TARGET_LINK_LIBRARIES(nyul_standardization
    ${ITK_LIBRARIES}
  )


ELSE(ITK_FOUND)
     MESSAGE(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
ENDIF(ITK_FOUND)

 

 
