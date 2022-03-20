CXXFLAGS = -Wall -pedantic -Wextra -g -std=c++14

HEADERS = $(wildcard src/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

.PHONY: all
all: compile doc

.PHONY: run
run: compile
	./zabojdan

.PHONY: compile
compile: zabojdan

.PHONY: doc
doc: doc

zabojdan: build/main
	cp $< $@

build/main: $(OBJECTS)
	mkdir -p $(@D)
	g++ $^ -o $@

build/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(CXXFLAGS) $< -c -o $@

doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf zabojdan build/ doc/ 2>/dev/null

build/CApplication.o: src/CApplication.cpp src/CApplication.h src/CGame.h \
 src/CPlayer.h src/CStats.h src/CCard.h src/CEffect.h src/EEffectType.h \
 src/CInterface.h src/EGameType.h src/CPlayerHuman.h src/CPlayerRobot.h \
 src/CIOHandler.h
build/CCard.o: src/CCard.cpp src/CCard.h src/CEffect.h src/CStats.h \
 src/EEffectType.h
build/CEffect.o: src/CEffect.cpp src/CEffect.h src/CStats.h src/EEffectType.h
build/CEffectAttack.o: src/CEffectAttack.cpp src/CEffectAttack.h src/CEffect.h \
 src/CStats.h src/EEffectType.h
build/CEffectDefend.o: src/CEffectDefend.cpp src/CEffectDefend.h src/CEffect.h \
 src/CStats.h src/EEffectType.h
build/CEffectHeal.o: src/CEffectHeal.cpp src/CEffectHeal.h src/CEffect.h \
 src/CStats.h src/EEffectType.h
build/CEffectMana.o: src/CEffectMana.cpp src/CEffectMana.h src/CEffect.h \
 src/CStats.h src/EEffectType.h
build/CGame.o: src/CGame.cpp src/CGame.h src/CPlayer.h src/CStats.h src/CCard.h \
 src/CEffect.h src/EEffectType.h src/CInterface.h src/CIOHandler.h
build/CIOHandler.o: src/CIOHandler.cpp src/CIOHandler.h src/CCard.h \
 src/CEffect.h src/CStats.h src/EEffectType.h src/CPlayer.h \
 src/CInterface.h src/CGame.h src/CPlayerRobot.h src/CPlayerHuman.h \
 src/CEffectAttack.h src/CEffectDefend.h src/CEffectHeal.h \
 src/CEffectMana.h
build/CInterface.o: src/CInterface.cpp src/CInterface.h src/CPlayer.h \
 src/CStats.h src/CCard.h src/CEffect.h src/EEffectType.h
build/CPlayer.o: src/CPlayer.cpp src/CPlayer.h src/CStats.h src/CCard.h \
 src/CEffect.h src/EEffectType.h
build/CPlayerHuman.o: src/CPlayerHuman.cpp src/CPlayerHuman.h src/CPlayer.h \
 src/CStats.h src/CCard.h src/CEffect.h src/EEffectType.h \
 src/CInterface.h
build/CPlayerRobot.o: src/CPlayerRobot.cpp src/CPlayerRobot.h src/CPlayer.h \
 src/CStats.h src/CCard.h src/CEffect.h src/EEffectType.h
build/CStats.o: src/CStats.cpp src/CStats.h
build/main.o: src/main.cpp src/CApplication.h src/CGame.h src/CPlayer.h \
 src/CStats.h src/CCard.h src/CEffect.h src/EEffectType.h \
 src/CInterface.h src/EGameType.h