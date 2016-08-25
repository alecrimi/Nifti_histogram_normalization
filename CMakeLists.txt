
#------------------------------------------------------------
PROJECT(NYUL_STANDARDIZATION)
 
 
 
#SET(ITK_DIR "/udd/medical/ITK2/Linux")
#INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
 
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
    INCLUDE(${ITK_USE_FILE})

  ADD_EXECUTABLE (nyul_standardization test_nyul.cpp nyul_standardization.h nyul_standardization.cpp )

  TARGET_LINK_LIBRARIES(nyul_standardization
    ITKIO
  )


ELSE(ITK_FOUND)
     MESSAGE(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
ENDIF(ITK_FOUND)

 

 
