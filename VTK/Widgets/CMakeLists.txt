SET(KIT Widgets)
SET(UKIT WIDGETS)

SET(KIT_TCL_LIBS vtkRenderingTCL vtkHybridTCL)
SET(KIT_PYTHON_LIBS vtkRenderingPythonD vtkHybridPythonD)
SET(KIT_JAVA_LIBS vtkRenderingJava vtkHybridJava)
SET(KIT_LIBS vtkRendering vtkHybrid)

SET ( Kit_SRCS
vtk3DWidget.cxx
vtkAbstractWidget.cxx
vtkAffineRepresentation.cxx
vtkAffineRepresentation2D.cxx
vtkAffineWidget.cxx
vtkAngleRepresentation.cxx
vtkAngleRepresentation2D.cxx
vtkAngleWidget.cxx
vtkBalloonRepresentation.cxx
vtkBalloonWidget.cxx
vtkBezierContourLineInterpolator.cxx
vtkBiDimensionalRepresentation2D.cxx
vtkBiDimensionalWidget.cxx
vtkBorderRepresentation.cxx
vtkBorderWidget.cxx
vtkBoundedPlanePointPlacer.cxx
vtkBoxWidget.cxx
vtkCameraRepresentation.cxx
vtkCameraWidget.cxx
vtkCaptionRepresentation.cxx
vtkCaptionWidget.cxx
vtkCenteredSliderWidget.cxx
vtkContourLineInterpolator.cxx
vtkContourRepresentation.cxx
vtkFocalPlaneContourRepresentation.cxx
vtkContourWidget.cxx
vtkCheckerboardRepresentation.cxx
vtkCheckerboardWidget.cxx
vtkConstrainedPointHandleRepresentation.cxx
vtkDistanceWidget.cxx
vtkDistanceRepresentation.cxx
vtkDistanceRepresentation2D.cxx
vtkEvent.cxx
vtkFocalPlanePointPlacer.cxx
vtkHandleRepresentation.cxx
vtkHandleWidget.cxx
vtkHoverWidget.cxx
vtkImageActorPointPlacer.cxx
vtkImageOrthoPlanes.cxx
vtkImagePlaneWidget.cxx
vtkImageTracerWidget.cxx
vtkImplicitPlaneRepresentation.cxx
vtkImplicitPlaneWidget.cxx
vtkImplicitPlaneWidget2.cxx
vtkLineRepresentation.cxx
vtkLineWidget.cxx
vtkLineWidget2.cxx
vtkLogoRepresentation.cxx
vtkLogoWidget.cxx
vtkOrientationMarkerWidget.cxx
vtkOrientedGlyphContourRepresentation.cxx
vtkOrientedGlyphFocalPlaneContourRepresentation.cxx
vtkParallelopipedRepresentation.cxx
vtkParallelopipedWidget.cxx
vtkPlaneWidget.cxx
vtkPlaybackRepresentation.cxx
vtkPlaybackWidget.cxx
vtkPointHandleRepresentation2D.cxx
vtkPointHandleRepresentation3D.cxx
vtkPointPlacer.cxx
vtkPointWidget.cxx
vtkPolyDataSourceWidget.cxx
vtkPolyDataPointPlacer.cxx
vtkPolygonalHandleRepresentation3D.cxx
vtkPolygonalSurfacePointPlacer.cxx
vtkPolyDataContourLineInterpolator.cxx
vtkPolygonalSurfaceContourLineInterpolator.cxx
vtkRectilinearWipeRepresentation.cxx
vtkRectilinearWipeWidget.cxx
vtkScalarBarWidget.cxx
vtkSeedRepresentation.cxx
vtkSeedWidget.cxx
vtkSliderRepresentation.cxx
vtkSliderRepresentation2D.cxx
vtkSliderRepresentation3D.cxx
vtkSliderWidget.cxx
vtkSphereHandleRepresentation.cxx
vtkSphereWidget.cxx
vtkSplineWidget.cxx
vtkTerrainDataPointPlacer.cxx
vtkTerrainContourLineInterpolator.cxx
vtkTextRepresentation.cxx
vtkTextWidget.cxx
vtkWidgetCallbackMapper.cxx
vtkWidgetEvent.cxx
vtkWidgetEventTranslator.cxx
vtkWidgetRepresentation.cxx
vtkWidgetSet.cxx
vtkXYPlotWidget.cxx
)

SET_SOURCE_FILES_PROPERTIES(
vtk3DWidget
vtkAbstractWidget
vtkAffineRepresentation
vtkAngleRepresentation
vtkPolyDataContourLineInterpolator
vtkContourLineInterpolator
vtkContourRepresentation
vtkFocalPlaneContourRepresentation
vtkDistanceRepresentation
vtkHandleRepresentation
vtkPolyDataSourceWidget
vtkSliderRepresentation
vtkWidgetRepresentation
ABSTRACT
)

SET(Kit_EXTRA_SRCS)
SET(Kit_EXTRA_CMDS)
SET(Kit_TCL_EXTRA_SRCS)
SET(Kit_PYTHON_EXTRA_SRCS)
SET(Kit_JAVA_EXTRA_SRCS)
SET(KIT_TCL_DEPS)
SET(KIT_PYTHON_DEPS)
SET(KIT_JAVA_DEPS)

#-----------------------------------------------------------------------------
# Include CMake code common to all kits.
INCLUDE(${VTK_CMAKE_DIR}/KitCommonBlock.cmake)
#-----------------------------------------------------------------------------
