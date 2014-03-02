#include "conways_life.h"
#include "random.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>

int seed;
int quiet = 0;
int fast = 0;
double live_p = 0.2;
unsigned usec_delay = 50000;

// input: spaces dead, other alive
void set_lines(ConwaysLife& cl, const char ** lines) {
  unsigned x, y;
  const char * line;
  char cell;
  for (y = 0, line = lines[y];
       y < cl.size_y() && line != NULL;
       ++y, line = lines[y]) {
    for (x = 0, cell = line[x];
	 x < cl.size_x() && cell != '\0';
	 ++x, cell = line[x]) {
      cl.set(x, y, ConwaysLife::State(cell != ' '));
    }
  }
}

void print(const ConwaysLife &cl) {
  for (unsigned y = 0; y < cl.size_y(); ++y) {
    for (unsigned x = 0; x < cl.size_x(); ++x) {
      putchar(cl.get(x, y) ? 'o' : ' ');
    }
    putchar('\n');
  }
}

void random_setup(ConwaysLife &cl) {
  int LIVE_MAX = int (live_p * MY_RAND_MAX);
  for (unsigned y = 0; y < cl.size_y(); ++y) {
    for (unsigned x = 0; x < cl.size_x(); ++x) {
      cl.set(x, y, ConwaysLife::State(myrand() <= LIVE_MAX));
    }
  }
}

double gettimeofday_d() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + (t.tv_usec / 1000000.0);
}

void parse_options(int argc, char ** argv) {
  seed = time(NULL);
  int opt;
  //  -s 1393697847 -p 0.32 has period 3
  // -p 0.34 -s 327 has a pentadecathlon
  while ((opt = getopt(argc, argv, "fp:qs:u:")) != -1) {
    switch (opt) {
    case 'f':
      fast = 1;
      break;
    case 'p':
      live_p = atof(optarg);
      break;
    case 'q':
      fast = 1;
      quiet = 1;
      break;
    case 's':
      seed = atoi(optarg);
      break;
    case 'u':
      usec_delay = atoi(optarg);
      break;
    default: /* '?' */
      // 704 produces 7889 generations :)
      fprintf(stderr, "Usage: %s [-p live (0.0-1.0)] [-s seed (try 704)] [-q(uiet)] [-f(ast)\n",
	      argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  if (optind < argc) {
      fprintf(stderr, "Superfluous argument '%s'\n",
	      argv[optind]);
      exit(EXIT_FAILURE);
  }
}

int main(int argc, char ** argv) {
  parse_options(argc, argv);
  mysrand(seed);

  ConwaysLife cl;
  random_setup(cl);

  int generations = 0;
  int period;
  double start_time = gettimeofday_d();
  do {
    if (!fast) {
      usleep(usec_delay);
    }
    if (!quiet) {
      puts("\e[1;1H");		// VT102 mover cursor to pos 1; 1
      print(cl);
    }
    cl.next();
    ++generations;
    if (!quiet) {
      printf("%d \n", generations);
    }
    period = cl.stabilized();
  } while (period == 0);
  double elapsed = gettimeofday_d() - start_time;
  fprintf(stderr, "%d generations, period: %d, live_p: %g, seed: %d\n",
	  generations, period, live_p, seed);
  fprintf(stderr, "   %g s elapsed, %g cells/s\n",
	 elapsed,
	 generations * cl.size_x() * cl.size_y() / elapsed);
  return 0;
}
