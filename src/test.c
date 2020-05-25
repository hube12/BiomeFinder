#include <stdio.h>
#include "gen/layers.h"
#include "gen/generator.h"

int main() {
    int64_t seed=1;
    int X=2304;
    int Z=632576;
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
                        printf("%d,%d %d\n", 2304 + aX * 16 + x, 632576 + aZ * 16 + z,map[aX * 4096 + aZ * 256 + x * 16 + z]);
                    }
                }
            }
        }
    }

}