# CMakeList.txt : projet CMake pour TP3_Agent_Logique, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (TP3_Agent_Logique "TP3_Agent_Logique.cpp" "TP3_Agent_Logique.h" "Forest.h" "Forest.cpp" "Sensor.h" "Sensor.cpp" "Effector.h" "Effector.cpp" "Fait.h" "Fait.cpp" "Rule.h" "Rule.cpp" "Agent.h" "Agent.cpp")

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
