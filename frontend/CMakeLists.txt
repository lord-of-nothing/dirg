set(UI_HEADERS
	mainwindow/mainwindow.ui
	editor/editor.ui
)

set(HEADERS
	mainwindow/mainwindow.h
	editor/editor.h
	area/area.h
)

set(SOURCES 
	mainwindow/mainwindow.cpp
	editor/editor.cpp
	area/area.cpp
	main.cpp
)

qt_add_executable(${PROJECT_NAME}
	MANUAL_FINALIZATION
	${SOURCES}
	${HEADERS}
	${UI_HEADERS}
)

target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
	Qt6::Core 
	Qt6::Widgets 
	Qt6::Gui 
	geometry
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	area
	editor
	mainwindow
)

