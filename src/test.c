#include <stdio.h>
#include "gen/layers.h"
#include "gen/generator.h"

void test1() {

    int64_t seed = 10000;
    int X = 1024;
    int Z = 1425664;
    int reg16x = X / 256;
    int reg16z = Z / 16;
    initBiomes();
    LayerStack g = setupGeneratorMC17();
    Layer layerBiomeDummy;
    setupLayer(256, &layerBiomeDummy, NULL, 200, NULL);
    setWorldSeed(&layerBiomeDummy, seed);
    applySeed(&g, seed);
    for (int chunkX = 0; chunkX < 16; ++chunkX) {
        for (int chunkZ = 0; chunkZ < 16; ++chunkZ) {
            setChunkSeed(&layerBiomeDummy, 256 * reg16x + 16 * chunkX + 8, 256 * reg16z + 16 * chunkZ + 8);
            if (mcNextInt(&layerBiomeDummy, 6) == 5) printf("%d,%d \n", X+16*chunkX, Z+16*chunkZ);
        }
    }
}

void test2() {
    int64_t seed = 1;
    int X = 2304;
    int Z = 632576;
    initBiomes();
    LayerStack g = setupGeneratorMC17();
    int *map = allocCache(&g.layers[g.layerNum - 1], 256, 256);
    applySeed(&g, seed);
    genArea(&g.layers[g.layerNum - 1], map, X, Z, 256, 256);
    for (int aX = 0; aX < 16; ++aX) {
        for (int aZ = 0; aZ < 16; ++aZ) {
            for (int x = 0; x < 16; ++x) {
                for (int z = 0; z < 16; ++z) {
                    if (map[aX * 4096 + aZ * 256 + x * 16 + z] == mushroomIsland || map[aX * 4096 + aZ * 256 + x * 16 + z] == mushroomIslandShore) {
                        printf("%d,%d %d\n", 2304 + aX * 16 + x, 632576 + aZ * 16 + z, map[aX * 4096 + aZ * 256 + x * 16 + z]);
                    }
                }
            }
        }
    }
}

int main() {
    test1();


}