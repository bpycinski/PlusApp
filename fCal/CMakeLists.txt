PROJECT(fCal)

FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QT3SUPPORT TRUE) 
INCLUDE(${QT_USE_FILE})

# --------------------------------------------------------------------------
# Sources

SET (fCal_SRCS
  fCalMain.cxx
  fCalMainWindow.cxx
  Toolboxes/ConfigurationToolbox.cxx
  Toolboxes/CapturingToolbox.cxx
  Toolboxes/SpatialCalibrationToolbox.cxx
  Toolboxes/SingleWallCalibrationToolbox.cxx
  Toolboxes/TemporalCalibrationToolbox.cxx
  Toolboxes/StylusCalibrationToolbox.cxx
  Toolboxes/PhantomRegistrationToolbox.cxx
  Toolboxes/VolumeReconstructionToolbox.cxx
  Toolboxes/LandmarkTransformationToolbox.cxx
  )

if (WIN32)  
  SET (fCal_SRCS
    ${fCal_SRCS}
    )
endif (WIN32)  

SET (fCal_UI_HDRS
    fCalMainWindow.h
    Toolboxes/AbstractToolbox.h
    Toolboxes/ConfigurationToolbox.h
    Toolboxes/CapturingToolbox.h
    Toolboxes/SpatialCalibrationToolbox.h
    Toolboxes/SingleWallCalibrationToolbox.h
    Toolboxes/TemporalCalibrationToolbox.h
    Toolboxes/StylusCalibrationToolbox.h
    Toolboxes/PhantomRegistrationToolbox.h
    Toolboxes/VolumeReconstructionToolbox.h
    Toolboxes/LandmarkTransformationToolbox.h
  )

SET (fCal_UI_SRCS
  fCalMainWindow.ui 
  Toolboxes/ConfigurationToolbox.ui
  Toolboxes/CapturingToolbox.ui
  Toolboxes/SpatialCalibrationToolbox.ui
  Toolboxes/SingleWallCalibrationToolbox.ui
  Toolboxes/TemporalCalibrationToolbox.ui
  Toolboxes/StylusCalibrationToolbox.ui
  Toolboxes/PhantomRegistrationToolbox.ui
  Toolboxes/VolumeReconstructionToolbox.ui
  Toolboxes/LandmarkTransformationToolbox.ui
  )

SET (fCal_QT_Resources
  fCal.qrc
  ${PlusApp_SOURCE_DIR}/CommonWidgets/CommonWidgets.qrc
  )
  

# --------------------------------------------------------------------------
# Qt specific commands

QT4_WRAP_CPP( fCal_QT_MOC_Files ${fCal_UI_HDRS} )        
QT4_WRAP_UI( fCal_QT_UI_Headers ${fCal_UI_SRCS} )
QT4_WRAP_CPP( fCal_QT_MOC_UI_Files ${fCal_QT_UI_Headers} )
QT4_ADD_RESOURCES( fCal_QT_RCC_Files ${fCal_QT_Resources} )

SOURCE_GROUP("Generated Files" FILES ${fCal_QT_MOC_Files} ${fCal_QT_UI_Headers} ${fCal_QT_MOC_UI_Files} ${fCal_QT_RCC_Files})
SOURCE_GROUP("UI Files" FILES ${fCal_UI_SRCS})

include_directories(${PlusApp_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Toolboxes)
include_directories(${PlusApp_BINARY_DIR}/CommonWidgets)
include_directories(${PlusApp_SOURCE_DIR}/CommonWidgets)

# --------------------------------------------------------------------------
# Build the library

SET (fCal_LIBS 
    ${fCal_LIBS} 
    ${QT_LIBRARIES} 
    CommonWidgets
    vtkPlusCommon
    ${ITK_LIBRARIES}
    itkvnl 
    itkvnl_algo 
    vtkCommon 
    vtkRendering 
    vtkHybrid 
    vtkCharts
    vtkCalibrationAlgo 
    vtkDataCollection 
    vtkVolumeReconstruction
    )

IF (PLUS_USE_OpenIGTLink)
    SET (fCal_LIBS ${fCal_LIBS} OpenIGTLink)
ENDIF(PLUS_USE_OpenIGTLink)

ADD_EXECUTABLE( fCal MACOSX_BUNDLE ${fCal_SRCS} ${fCal_UI_HDRS} ${fCal_QT_UI_Headers} ${fCal_QT_UI_Headers} ${fCal_QT_MOC_Files} ${fCal_QT_MOC_UI_Files} ${fCal_QT_RCC_Files})

TARGET_LINK_LIBRARIES( fCal QVTK ${fCal_LIBS} )

# --------------------------------------------------------------------------
# Install
#
IF(PLUSAPP_INSTALL_BIN_DIR)
# show up in the Start Menu in Windows
if( WIN32 AND NOT UNIX )
    SET(CPACK_PACKAGE_EXECUTABLES "fCal" "fCal")
endif()

INSTALL(TARGETS fCal 
  DESTINATION ${PLUSAPP_INSTALL_BIN_DIR}
  COMPONENT RuntimeLibraries
  )
  
ENDIF(PLUSAPP_INSTALL_BIN_DIR)
