default: AGL3-example

# Komentarz: powyżej co chcemy aby powstało (można więcej)
# Sprawdzamy jeśli poniższe zmodyfikowane to także rekompilacja
DEPS=AGL3Window.cpp AGL3Window.hpp AGL3Drawable.hpp

# Reguła jak zbudować plik wykonywalny z pliku .cpp
%: %.cpp $(DEPS)
	g++ -I. $< -o $@ AGL3Window.cpp  -lepoxy -lGL -lglfw 

# A tu dodatkowy target, którego wywołanie "make clean" czyści
clean:
	rm a.out *.o *~ AGL3-example
