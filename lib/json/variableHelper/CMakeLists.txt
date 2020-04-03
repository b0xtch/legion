add_library (variableHelper ./variableHelper.cpp ./variableHelper.h)
target_link_libraries(variableHelper jsonDSL absl::strings nlohmann_json)