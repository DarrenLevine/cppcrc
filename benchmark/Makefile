

.PHONY: benchmark
benchmark:
	@mkdir -p build && \
	cd build && \
	cmake .. && \
	make && \
	./benchmark

.PHONY: clean
clean:
	@rm -rf build

# all other libraries should get auto-fetched using CMake from github
install_boost_ubuntu:
	sudo apt-get install libboost-all-dev
