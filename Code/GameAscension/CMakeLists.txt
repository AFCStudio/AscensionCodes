option(OPTION_GAMEASCENSION "Build GameAscension AFC Studio project" ON)
if (NOT OPTION_GAMEASCENSION)
	return()
endif()

add_subdirectory(GameDll)
