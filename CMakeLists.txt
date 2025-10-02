cmake_minimum_required(VERSION 3.19)
project(mydspproject LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets DataVisualization)

find_package(BLAS REQUIRED)
find_path(BLAS_INCLUDE_DIR 
	NAMES cblas.h
	PATHS 
	/usr/include
	/usr/local/include
	/opt/homebrew/include
)

qt_standard_project_setup()

qt_add_executable(main
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
	simulation.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/simulation_widget/simulation_widget.cpp
	time_widget/time_widget.cpp
	#time_widget/time_widget.h
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dep)

target_link_libraries(main
	PRIVATE
	Qt::Core
	Qt::Widgets
	Qt::DataVisualization
	${BLAS_LIBRARIES}
	QCustomPlotlib
)

target_include_directories(
	main 
	PRIVATE 
	${BLAS_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/simulation_widget
	${CMAKE_CURRENT_SOURCE_DIR}/dep/qcustomplot-source
	${CMAKE_CURRENT_SOURCE_DIR}/time_widget/include
)
