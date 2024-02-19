all: hw2_tests/.dirstamp Interpreter
hw2_tests/.dirstamp:
	mkdir -p hw2_tests
	touch hw2_tests/.dirstamp 

#tests: FORCE Schedule_Tests
#	# valgrind --leak-check=yes ./Schedule_Tests
#	./Schedule_Tests

#FORCE: ;

Interpreter: Interpreter.cpp hw2_tests/MultStatement.o hw2_tests/AddStatement.o hw2_tests/SubStatement.o hw2_tests/DivStatement.o hw2_tests/IfStatement.o hw2_tests/PrintStatement.o hw2_tests/LetStatement.o hw2_tests/PrintAllStatement.o hw2_tests/JumpStatement.o hw2_tests/JumpSubStatement.o hw2_tests/ReturnStatement.o hw2_tests/EndStatement.o hw2_tests/ProgramState.o 
	g++ -Wall -Ilib -g $^ -o Interpreter

hw2_tests/AddStatement.o: AddStatement.cpp AddStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/AddStatement.o
hw2_tests/SubStatement.o: SubStatement.cpp SubStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/SubStatement.o
hw2_tests/MultStatement.o: MultStatement.cpp MultStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/MultStatement.o
hw2_tests/DivStatement.o: DivStatement.cpp DivStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/DivStatement.o
hw2_tests/IfStatement.o: IfStatement.cpp IfStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/IfStatement.o
hw2_tests/LetStatement.o: LetStatement.cpp LetStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/LetStatement.o
hw2_tests/JumpStatement.o: JumpStatement.cpp JumpStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/JumpStatement.o
hw2_tests/JumpSubStatement.o: JumpSubStatement.cpp JumpSubStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/JumpSubStatement.o
hw2_tests/PrintStatement.o: PrintStatement.cpp PrintStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/PrintStatement.o
hw2_tests/PrintAllStatement.o: PrintAllStatement.cpp PrintAllStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/PrintAllStatement.o
hw2_tests/ReturnStatement.o: ReturnStatement.cpp ReturnStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/ReturnStatement.o
hw2_tests/EndStatement.o: EndStatement.cpp EndStatement.h Statement.h ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/EndStatement.o
hw2_tests/ProgramState.o: ProgramState.cpp ProgramState.h
	g++ -Wall -Ilib -g -c $< -o hw2_tests/ProgramState.o
#Interpreter.o: Interpreter.cpp Statement.h LetStatement.h AddStatement.h SubStatement.h DivStatement.h IfStatement.h PrintAllStatement.h PrintStatement.h JumpStatement.h JumpSubStatement.h ReturnStatement.h EndStatement.h ProgramState.h
#	g++ -Wall -Ilib -g -c $< -o hw2_tests/Interpreter.o
.PHONY: clean
clean:
	rm hw2_tests/AddStatement.o
	rm hw2_tests/SubStatement.o
	rm hw2_tests/DivStatement.o
	rm hw2_tests/IfStatement.o
	rm hw2_tests/PrintStatement.o
	rm hw2_tests/LetStatement.o
	rm hw2_tests/PrintAllStatement.o
	rm hw2_tests/JumpStatement.o
	rm hw2_tests/JumpSubStatement.o
	rm hw2_tests/ReturnStatement.o
	rm hw2_tests/EndStatement.o
	rm hw2_tests/ProgramState.o
	rm hw2_tests/MultStatement.o
	rm Interpreter