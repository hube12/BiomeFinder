#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include "gen/layers.h"
#include "gen/generator.h"
#define OFFSET 200 // adjust this one for how many per 16*16 chunks grid should be obtained (its 256-OFFSET)
#define PROPORTION 0.4 //adjust the proportion of biome you want in the 256*256 grid, 0.6 means about 39322 blocks
static unsigned int str2int(const char *str, int h) {
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ (unsigned int) (str[h]);
}

const char *versions[] = {"1.7", "1.8", "1.9", "1.10", "1.11", "1.12", "1.13", "1.13.2", "1.14", "1.15", "UNKNOWN"};


enum versions parse_version(char *s) {
    enum versions v;
    switch (str2int(s, 0)) {
        case 193357645 :
            v = MC_1_7;
            break;
        case 193366850:
            v = MC_1_8;
            break;
        case 193367875 :
            v = MC_1_9;
            break;
        case 2085846491 :
            v = MC_1_10;
            break;
        case 2085882490 :
            v = MC_1_11;
            break;
        case 2085918233 :
            v = MC_1_12;
            break;
        case 2085954232 :
            v = MC_1_13;
            break;
        case 3841915620:
            v = MC_1_13_2;
            break;
        case 2085703007:
            v = MC_1_14;
            break;
        case 2085739006:
            v = MC_1_15;
            break;
        default:
            v = MC_LEG;
            break;
    }
    return v;
}

void usage() {
    printf("Usage is ./MushroomFinder [mcversion] [seed] [searchRange]? [filter]? \n"
           "Valid [mcversion] are 1.7, 1.8, 1.9, 1.10, 1.11, 1.12, 1.13, 1.13.2, 1.14.\n"
           "Valid [searchRange] (optional) is in blocks, default is 150000 which correspond to -150000 to 150000 on both X and Z.\n");
    exit(EXIT_FAILURE);
}
int main(int argc, char *argv[]) {
    int mcversion = MC_1_12;
    int64_t seed = (int64_t) NULL;
    int searchRange = 300;
    char *endptr;
    if (argc > 2) {
        mcversion = parse_version(argv[1]);
        if (mcversion == MC_LEG) {
            usage();
        }
        errno = 0;
        seed = strtoll(argv[2], &endptr, 10);
        if ((errno == ERANGE && (seed == LLONG_MAX || seed == LLONG_MIN)) || (errno != 0 && seed == 0)) {
            fprintf(stderr, "Seed was not parsed correctly\n");
            usage();
        }
        if (endptr == argv[2]) {
            fprintf(stderr, "Seed contained letters\n");
            usage();
        }
    }else{
        usage();
    }
    if (argc > 3) {
        errno = 0;
        searchRange = (int) (strtoll(argv[3], &endptr, 10)  / 16/16);
        if ((errno == ERANGE && (seed == LLONG_MAX || seed == LLONG_MIN)) || (errno != 0 && seed == 0)) {
            fprintf(stderr, "Search Range was not parsed correctly\n");
            usage();
        }
        if (endptr == argv[3]) {
            fprintf(stderr, "Search Range contained letters\n");
            usage();
        }
        if (searchRange < 0) {
            fprintf(stderr, "Search range should be positive");
            usage();
        }
    }
    if (argc > 4) {
        usage();
    }
    initBiomes();

    Layer layerBiomeDummy;
    setupLayer(256, &layerBiomeDummy, NULL, 200, NULL);
    setWorldSeed(&layerBiomeDummy, seed);

    LayerStack g;
    if (mcversion >= MC_1_13) {
        g = setupGeneratorMC113();
    } else {
        g = setupGeneratorMC17();
    }
    printf("Using seed %ld and version %s\n", seed, versions[mcversion]);
    int *map = allocCache(&g.layers[g.layerNum - 1], 256, 256);
    applySeed(&g, seed);
    for (int reg16x = 0; reg16x < searchRange; ++reg16x) {
        for (int reg16z = 0; reg16z < searchRange; ++reg16z) {
            int hit=0;
            for (int chunkX = 0; chunkX < 16; ++chunkX) {
                for (int chunkZ = 0; chunkZ < 16; ++chunkZ) {
                    setChunkSeed(&layerBiomeDummy, 256*reg16x+16*chunkX+8 , 256*reg16z+16*chunkZ+8);
                    if(mcNextInt(&layerBiomeDummy, 6) == 5) hit++;
                }
            }
            if (hit>256-OFFSET){
                int count=0;
                genArea(&g.layers[g.layerNum - 1], map, 256*reg16x, 256*reg16z, 256, 256);
                for (int i = 0; i < 256*256; ++i) {
                    if (map[i]==mushroomIsland || map[i]==mushroomIslandShore){
                        count++;
                    }
                }
                if (count>256.0*256.0*PROPORTION){
                    printf("Good positions starting at (x,z): %d,%d with %f %% filling\n", reg16x * 256 , reg16z * 256,count/256.0/256.0*100);
                }
            }

        }
    }
}