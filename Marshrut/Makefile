# Простой Makefile для лабораторной работы №3
# Все файлы в одном каталоге

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
LDFLAGS =
TEST_LIBS = -lUnitTest++

# Имена целевых файлов
MAIN_TARGET = main_program
TEST_TARGET = test_program

# Все исходные файлы
MAIN_SRCS = main.cpp RouteCipher.cpp
TEST_SRCS = test_RouteCipher.cpp RouteCipher.cpp

# Объектные файлы
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Правило по умолчанию
all: $(MAIN_TARGET) $(TEST_TARGET)

# Основная программа
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN_OBJS) $(LDFLAGS)

# Тестовая программа
$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS) $(TEST_LIBS) $(LDFLAGS)

# Компиляция .cpp в .o
%.o: %.cpp RouteCipher.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Запуск тестов
test: $(TEST_TARGET)
	@echo "=== Запуск тестов RouteCipher ==="
	./$(TEST_TARGET)

# Запуск основной программы
run: $(MAIN_TARGET)
	@echo "=== Запуск основной программы ==="
	./$(MAIN_TARGET)

# Очистка
clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET) *.o

# Справка
help:
	@echo "Доступные команды:"
	@echo "  make all     - сборка программы и тестов"
	@echo "  make         - то же что 'make all'"
	@echo "  make test    - сборка и запуск тестов"
	@echo "  make run     - сборка и запуск программы"
	@echo "  make clean   - удаление скомпилированных файлов"
	@echo "  make help    - эта справка"

# Файлы, которые не являются реальными файлами
.PHONY: all test run clean help
