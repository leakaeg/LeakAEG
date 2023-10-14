add_llvm_library( libtestk MODULE
  utility.cpp
  simple_set.cpp
  testk.cpp
  knobs.cpp
  spot.cpp
  DEPENDS
  PLUGIN_TOOL
  opt
)

target_compile_features(libtestk PRIVATE cxx_range_for cxx_auto_type)

target_link_libraries( libtestk )
