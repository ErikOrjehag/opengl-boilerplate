#include "Terrain.hh"

TextureData ttex;



void Terrain::generate(std::string terrain_file) {

    LoadTGATextureData("fft-terrain.tga", &ttex);
}