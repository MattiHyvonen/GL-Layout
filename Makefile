cc = g++
cFlagit = -std=c++17
lFlagit = -lsfml-graphics -lsfml-window -lsfml-system
kohde = layout
srcPolku = src
includePolku = include
objPolku = obj

testiCFlagit = $(cFlagit)
testiLFlagit = $(lFlagit) -lgmock -lgtest -pthread
testikohde = ajaTestit
testiSrcPolku = testit/src
testiIncludePolku = testit/include
testiObjPolku = testit/obj

googletestBuildPolku = testit/googletest-build
googletestPolku = testit/googletest
googletestLibPolku = $(googletestBuildPolku)/lib
googletestIncludePolku = $(googletestPolku)/googletest/include
googlemockIncludePolku = $(googletestPolku)/googlemock/include

sourcet = $(wildcard $(srcPolku)/*.cpp)
objektit = $(sourcet:$(srcPolku)/%.cpp=$(objPolku)/%.o)

testiSourcet = $(wildcard $(testiSrcPolku)/*.cpp)
testiObjektit = $(testiSourcet:$(testiSrcPolku)/%.cpp=$(testiObjPolku)/%.o)

all: teeObjPolku $(kohde)

$(kohde): $(objektit)
	$(cc) $(objektit) $(lFlagit) -o $@

$(objektit): $(objPolku)/%.o : $(srcPolku)/%.cpp
	$(cc) -c $< $(cFlagit) -I$(includePolku) -o $@

#TODO: jos .h -tiedostoja muutetaan, ei "make testit" aina löydä
#		muutoksia, vaan on ajettava "make clean; make testit"!
testit: buildGoogletest teeObjPolku teeTestiObjPolku $(testikohde)
	./$(testikohde)

teeObjPolku:
	mkdir -p $(objPolku)

teeTestiObjPolku:
	mkdir -p $(testiObjPolku)

$(testikohde): $(testiObjektit)  $(filter-out obj/main.o,  $(objektit) )
	$(cc) $(testiObjektit) $(filter-out obj/main.o,  $(objektit) ) \
	$(testiLFlagit) -L$(googletestLibPolku) \
	-o $@

$(testiObjektit): $(testiObjPolku)/%.o : $(testiSrcPolku)/%.cpp
	$(cc) -c $< $(testiCFlagit) \
	-I$(includePolku) \
	-I$(testiIncludePolku) \
	-I$(googletestIncludePolku) \
	-I$(googlemockIncludePolku) \
	-o $@

buildGoogletest:
	mkdir -p $(googletestBuildPolku)
	cmake -B$(googletestBuildPolku) -H$(googletestPolku)
	make -C $(googletestBuildPolku)

clean:
	rm -Rf $(objPolku)
	rm -Rf $(testiObjPolku)
	rm -f $(testikohde)
	rm -f $(kohde)

remove: clean
	rm -Rf $(googletestBuildPolku)
