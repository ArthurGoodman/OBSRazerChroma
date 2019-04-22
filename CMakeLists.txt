project(obs-razer-chroma)

add_subdirectory(third_party/chroma_sdk)

set(${PROJECT_NAME}_SOURCES
    src/plugin.cpp
    )

add_library(${PROJECT_NAME} MODULE
    ${${PROJECT_NAME}_SOURCES}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE libobs
    PRIVATE obs-frontend-api
    PRIVATE chroma_sdk
    )

install_obs_plugin_with_data(${PROJECT_NAME} data)
