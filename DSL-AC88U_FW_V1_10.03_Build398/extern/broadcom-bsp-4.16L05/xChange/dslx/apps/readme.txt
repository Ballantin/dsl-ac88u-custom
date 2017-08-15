Cablex applications provide:

1) A set of make targets that define the modules contained in the final image.


2) A set of custom build configuration header files (e.g. bosCfg.h and 
   bosCfgCustom.h) that enable/disable compile-time features. Custom 
   configuration header files may exist in the following locations:

   - ../cablex/apps/<app_name>/cfginc/: 
      These are application specific, and override common app configuration header 
      files and default configuration header files. The compiler will search
      these directories first for custom config files.
      
      This directory should contain subdirectories for each "VOB" 
      (e.g. xchg_drivers, xchg_common, etc.) + the MTA.


   - ../cablex/apps/cfginc/
      These are common to all applications. The compiler will search these
      directories after looking in the application specific directory.
      
      This directory should contain subdirectories for each "VOB", excluding the
      MTA. MTA configuration options that are common to all applications should be
      located in ../cablex/mta/inc/cfg/.


   - ../<module>/inc
      Modules that provide build configuration headers should provide an empty
      default custom header. The compiler will search here last.
      
      Default MTA custom configuration headers should be located in 
      ../cablex/mta/inc/cfg/customdefault/.
