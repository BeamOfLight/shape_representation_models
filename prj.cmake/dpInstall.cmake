# Function for setting install properties:
#
# install_units(unitA)
# install_units("unitA,unitB")


function(install_units units)
  foreach(unit IN ITEMS ${units})
    install(TARGETS "${unit}"
      LIBRARY DESTINATION "lib/${PLATFORM}.${CRT}/debug"
      ARCHIVE DESTINATION "lib/${PLATFORM}.${CRT}/debug"
      CONFIGURATIONS Debug
    )
    install(TARGETS "${unit}"
      LIBRARY DESTINATION "lib/${PLATFORM}.${CRT}/release"
      ARCHIVE DESTINATION "lib/${PLATFORM}.${CRT}/release"
      CONFIGURATIONS Release
    )
  endforeach()
endfunction(install_units)
