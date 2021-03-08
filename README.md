# 3DReconstruction
3D modeling of organs based on medical images (CT, MRI ...). The modeling is done using computer graphics algorithms.

I am using a transperancy based approach to enable us to see specific parts of the organs (for example only bones or only tissues).

The model enables us to see planes that doesn't exist in the original images.

Video demonstration available at https://weizmannacil-my.sharepoint.com/:v:/g/personal/avner_shultzman_weizmann_ac_il/EU1J8t9oIrROuJ-nLyn_QB8BXd3rvbhZUSRTm_Y6U_B9OQ?e=6chrbi

To run with visual studio add the following:
 - Project -> Properties -> C/C++ -> General -> Additional include directories : $(SolutionDir)/../External Libs/GLEW/include
  
                                                                                 $(SolutionDir)/../External Libs/GLFW64/include
                                                                                 
                                                                                 $(SolutionDir)/../External Libs/GLM
                                                                                 
 - Project -> Properties -> C/C++ -> Preprocessor -> Preprocessor definitions : GLEW_STATIC 
  
 - Project -> Properties -> Library -> General -> Additional Library directories : $(SolutionDir)/../External Libs/GLEW/lib/Release/x64
  
                                                                                   $(SolutionDir)/../External Libs/GLFW64/lib-vc2017
                                                                                   
 - Project -> Properties -> Library -> Input -> Additional dependencies : opengl32.lib
  
                                                                          glew32s.lib
                                                                          
                                                                          glfw3.lib
                                                                          
