# - try to find Laretz libraries and include files
# LARETZ_INCLUDE_DIR where to find qwt_plot.h, etc.
# LARETZ_LIBRARIES libraries to link against
# LARETZ_FOUND If false, do not try to use Qwt

find_path (LARETZ_INCLUDE_DIRS
	NAMES laretzversion.h
	PATHS
	/usr/local/include/laretz
	/usr/include/laretz
)

find_library (LARETZ_LIBRARIES
	NAMES laretz_ops
	PATHS
	/usr/local/lib
	/usr/lib
)

# handle the QUIETLY and REQUIRED arguments and set QWT_FOUND to TRUE if
# all listed variables are TRUE
include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Laretz DEFAULT_MSG LARETZ_LIBRARIES LARETZ_INCLUDE_DIRS )
mark_as_advanced(LARETZ_LIBRARIES LARETZ_INCLUDE_DIRS)

