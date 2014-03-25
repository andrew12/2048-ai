#include <stdlib.h>

/* The fundamental trick: the 4x4 board is represented as a 64-bit word,
 * with each board square packed into a single 4-bit nibble.
 * 
 * The maximum possible board value that can be supported is 32768 (2^15), but
 * this is a minor limitation as achieving 65536 is highly unlikely under normal circumstances.
 * 
 * The space and computation savings from using this representation should be significant.
 * 
 * The nibble shift can be computed as (r,c) -> shift (4*r + c). That is, (0,0) is the LSB.
 */

typedef uint64_t board_t;
typedef uint16_t row_t;

#define ROW_MASK 0xFFFFULL
#define COL_MASK 0x000F000F000F000FULL

/* unif_random is defined as a random number generator returning a value in [0..n-1]. */
#if defined(__APPLE__)
static inline unsigned unif_random(unsigned n) {
    return arc4random_uniform(n);
}
#elif defined(__linux__)
// Warning: This is a slightly biased RNG.
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
static inline unsigned unif_random(unsigned n) {
    static int seeded = 0;

    if(!seeded) {
        int fd = open("/dev/urandom", O_RDONLY);
        unsigned short seed[3];
        if(fd < 0 || read(fd, seed, sizeof(seed)) < (int)sizeof(seed)) {
            srand48(time(NULL));
        } else {
            seed48(seed);
        }
        if(fd >= 0)
            close(fd);

        seeded = 1;
    }

    return (int)(drand48() * n);
}
#else
// Warning: This is a slightly biased RNG.
#include <time.h>
static inline unsigned unif_random(unsigned n) {
    static int seeded = 0;

    if(!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    return rand() % n;
}
#endif

static inline int rank_to_value(int rank) {
    return 1 << (rank + 1);
}

static inline void print_board(board_t board) {
    int i, j, rank;
    for(i=0; i<4; i++) {
        for(j=0; j<4; j++) {
            rank = board & 0xf;
            if (rank == 0) {
                printf("     .");
            } else {
                printf(" %5d", rank_to_value(rank));
            }
            board >>= 4;
        }
        printf("\n");
    }
    printf("\n");
}

static inline row_t pack_col(board_t col) {
    return (row_t)(col | (col >> 12) | (col >> 24) | (col >> 36));
}

static inline board_t unpack_col(row_t row) {
    board_t tmp = row;
    return (tmp | (tmp << 12ULL) | (tmp << 24ULL) | (tmp << 36ULL)) & COL_MASK;
}

static inline row_t reverse_row(row_t row) {
    return (row >> 12) | ((row >> 4) & 0x00F0)  | ((row << 4) & 0x0F00) | (row << 12);
}

/* Functions */
#ifdef __cplusplus
extern "C" {
#endif

void init_score_tables(void);
void init_move_tables(void);

typedef int (*get_move_func_t)(board_t);
float score_toplevel_move(board_t board, int move);
int find_best_move(board_t board);
int ask_for_move(board_t board);
void play_game(get_move_func_t get_move);

#ifdef __cplusplus
}
#endif
