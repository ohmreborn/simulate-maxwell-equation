cmake_minimum_required(VERSION 3.19)
project(mydspproject LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
find_package(Qt6 6.5 REQUIRED COMPONENTS 
	Core 
	Gui
   	Widgets
   	DataVisualization
	Charts
)

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
	simulation/simulation.cpp
	simulation/source_wave/source_wave.cpp


	simulation_widget/simulation_widget.cpp
	time_widget/time_widget.cpp
)

target_link_libraries(main
	PRIVATE
	Qt::Core
	Qt::Widgets
	Qt::DataVisualization
	Qt::Charts
	${BLAS_LIBRARIES}
)

target_include_directories(
	main 
	PRIVATE 
	${BLAS_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/simulation_widget
	${CMAKE_CURRENT_SOURCE_DIR}/time_widget
)
