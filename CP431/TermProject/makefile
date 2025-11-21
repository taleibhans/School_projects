NUM_PROCESSORS = 8

clean:
	rm output/*

build_compute: compute_juliaset.c
	mpicc -o ./output/compute_juliaset compute_juliaset.c -lm -Wall

run_compute: ./output/compute_juliaset
	mpirun -np $(NUM_PROCESSORS) ./output/compute_juliaset

run_compute_linux: ./output/compute_juliaset
	mpirun -np $(NUM_PROCESSORS) --use-hwthread-cpus ./output/compute_juliaset

build_render_mac: render_juliaset.c
	gcc -o output/render_juliaset lib/lodepng.c render_juliaset.c -framework OpenGL -framework GLUT -Wall -Wno-deprecated-declarations -DGL_SILENCE_DEPRECATION

build_render: render_juliaset.c
	gcc -o output/render_juliaset lib/lodepng.c render_juliaset.c -Wall -lfreeglut -lglew32 -lopengl32 -lglu32

build_render_linux: render_juliaset.c
	gcc -o output/render_juliaset lib/lodepng.c render_juliaset.c -Wall -lGL -lglut -lGLU

run_render: ./output/render_juliaset
	./output/render_juliaset

run: build_compute run_compute build_render run_render
