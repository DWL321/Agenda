OBJS := build/AgendaUI.o build/Storage.o build/Meeting.o build/Date.o build/User.o build/AgendaSeervice.o build/Error.o build/Log.o
AgendaUI : $(OBJS)
	g++ $(OBJS) -o AgendaUI

build/%.o: src/%.cpp
	@mkdir -p build
	g++ -g -I./include -c -o $@ $^
clean:
	@rm -rf build AgendaUI
