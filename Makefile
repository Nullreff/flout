BUILD_DIR := build
FLOUT := ./build/src/flout
VALGRIND := valgrind --error-exitcode=1 --leak-check=full --show-reachable=yes
COMPILE := make --no-print-directory

.PHONY: all clean release debug run install test bench memcheck docs publish help

all: release

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR}

release: ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake -DCMAKE_BUILD_TYPE=RELEASE .. && ${COMPILE}

debug: ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake -DCMAKE_BUILD_TYPE=DEBUG .. && ${COMPILE}

run:
	${FLOUT}

install:
	cd ${BUILD_DIR} && make install --no-print-directory

memcheck: debug
	${VALGRIND} ${FLOUT}

