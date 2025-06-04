# Makefile za Rubikovu Kocku 3D

# Kompajler i flagovi
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Naziv izvršne datoteke
TARGET = rubik

# Izvorni kod
SOURCE = main.cpp

# Default target
all: $(TARGET)

# Kompajliranje
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Pokretanje
run: $(TARGET)
	./$(TARGET)

# Čišćenje
clean:
	rm -f $(TARGET)
	rm -f *.txt
	rm -f *.bin

# Čišćenje samo izvršnih datoteka
clean-exe:
	rm -f $(TARGET)

# Čišćenje samo datoteka igre
clean-data:
	rm -f *.txt
	rm -f *.bin

# Debug verzija
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release verzija
release: CXXFLAGS += -DNDEBUG
release: $(TARGET)

# Instalacija (kopira u /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Deinstalacija
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Help
help:
	@echo "Dostupne komande:"
	@echo "  make        - Kompajlira program"
	@echo "  make run    - Kompajlira i pokreće program"
	@echo "  make clean  - Briše sve generirane datoteke"
	@echo "  make debug  - Kompajlira debug verziju"
	@echo "  make release- Kompajlira optimizovanu verziju"
	@echo "  make install- Instalira program sistemski"
	@echo "  make help   - Prikazuje ovu pomoć"

.PHONY: all run clean clean-exe clean-data debug release install uninstall help