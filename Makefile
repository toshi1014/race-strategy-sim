BUILD_DIR=build

run:
	@cmake -B ${BUILD_DIR}
	@cmake --build ${BUILD_DIR}
	@echo "\n\n=== out =========\n"
	@${BUILD_DIR}/a.out
	@echo

debug:
	@cmake -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
	@cmake --build ${BUILD_DIR}
	@gdb ${BUILD_DIR}/a.out

clean:
	@yes | rm -r ${BUILD_DIR}

.PHONY:
	run debug clean