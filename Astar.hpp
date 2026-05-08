#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <utility>
#include <cmath>
#include <array>
#include <algorithm>
#include <queue>
#include <deque>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <random>

#include "Puppet.hpp"
#include "AlgMani.hpp"

#pragma pack(push, 1) 
struct KeyCorners {
    Puppet::ShapeFast shape;
    Puppet::Corners corners;
    bool operator==(const KeyCorners& other) const {
        return shape == other.shape && corners == other.corners;
    }
    bool operator<(const KeyCorners& other) const{
        if (shape.bits[0] != other.shape.bits[0])
            return shape.bits[0] < other.shape.bits[0];
        if (shape.bits[1] != other.shape.bits[1])
            return shape.bits[1] < other.shape.bits[1];
        for (int i = 0; i < 7; ++i) {
            if (corners.data[i] != other.corners.data[i])
                return corners.data[i] < other.corners.data[i];
        }
        return false;
    }
};
struct KeyEdges {
    Puppet::Edges edges;
    Puppet::FlipsFast flips;
    bool operator==(const KeyEdges& other) const {
        return edges == other.edges && flips == other.flips;
    }
    bool operator<(const KeyEdges& other) const{
        for(int i = 0; i < 9; ++i) {
            if (edges.data[i] != other.edges.data[i])
                return edges.data[i] < other.edges.data[i];
        }//no memcmp because of weird error
        return flips.bits < other.flips.bits;
    }
};
struct KeySF {
    Puppet::ShapeFast shape;
    Puppet::FlipsFast flips;
    bool operator==(const KeySF& other) const {
        return shape == other.shape && flips == other.flips;
    }
    bool operator<(const KeySF& other) const{
        if (shape.bits[0] != other.shape.bits[0])
            return shape.bits[0] < other.shape.bits[0];
        if (shape.bits[1] != other.shape.bits[1])
            return shape.bits[1] < other.shape.bits[1];
        return flips.bits < other.flips.bits;
    }
};
struct Key4E {
    Puppet::ShapeFast shape;
    Puppet::fourE fe;
    bool operator==(const Key4E& other) const {
        return shape == other.shape && fe == other.fe;
        /*return shape.bits[0] == other.shape.bits[0] && 
               shape.bits[1] == other.shape.bits[1] &&
               std::memcmp(fe.data, other.fe.data, 4) == 0;*/
    }
    bool operator<(const Key4E& other) const{
        if (shape.bits[0] != other.shape.bits[0])
            return shape.bits[0] < other.shape.bits[0];
        if (shape.bits[1] != other.shape.bits[1])
            return shape.bits[1] < other.shape.bits[1];
        for(int i = 0; i < 4; ++i) {
            if (fe.data[i] != other.fe.data[i])
                return fe.data[i] < other.fe.data[i];
        }
        return false;
    }
};
struct KeySE {
    Puppet::ShapeFast shape;
    Puppet::Edges edges;
    bool operator==(const KeySE& other) const {
        return shape == other.shape && edges == other.edges;
        /*return shape.bits[0] == other.shape.bits[0] && 
               shape.bits[1] == other.shape.bits[1] &&
               std::memcmp(fe.data, other.fe.data, 4) == 0;*/
    }
    bool operator<(const KeySE& other) const{
        if (shape.bits[0] != other.shape.bits[0])
            return shape.bits[0] < other.shape.bits[0];
        if (shape.bits[1] != other.shape.bits[1])
            return shape.bits[1] < other.shape.bits[1];
        for(int i = 0; i < 8; ++i) {
            if (edges.data[i] != other.edges.data[i])
                return edges.data[i] < other.edges.data[i];
        }
        return false;
    }
};
template <typename K, typename V> struct Entry{
    K key;
    V value;
    bool operator<(const Entry<K,V>& other) const{
        return key < other.key;
    }
};
#pragma pack(pop) // Reaktiviert Standard-Padding für den Rest des Codes

#define S_SIZE 13174
#define C_SIZE 474264
#define E_SIZE 362880
#define F_SIZE 256
#define CF_SIZE 121411584
#define SE_SIZE 4780581120

class Astar{
    private:
        Puppet puppet;
        AlgMani am;
    public:
        Astar();
        /*void writeData(const std::string& filename, const void* data, size_t size) {
            std::ofstream os(filename, std::ios::binary | std::ios::trunc);
            os.write(reinterpret_cast<const char*>(data), size);
            os.close();
        }*/
        void writeData(const std::string& filename, const void* data, size_t size) {
            std::ofstream os(filename, std::ios::binary | std::ios::trunc);
            if (!os) {
                std::cerr << "error while opening file!" << std::endl;
                return;
            }

            const char* ptr = reinterpret_cast<const char*>(data);
            size_t remaining = size;
            size_t chunkSize = 256 * 1024 * 1024; // 256 MB pro Schritt

            std::cout << "start saving: " << size / (1024*1024) << " MB..." << std::endl;
            while (remaining > 0) {
                size_t toWrite = std::min(remaining, chunkSize);
                os.write(ptr, toWrite);
                
                if (!os.good()) {
                    std::cerr << "error while writing!" << std::endl;
                    break;
                }

                ptr += toWrite;
                remaining -= toWrite;
                
                // Fortschrittsanzeige
                //std::cout << "to write: " << remaining / (1024*1024) << " MB" << std::endl;
            }
            os.close();
            std::cout << "saved." << std::endl;
        }
        template <typename K,  typename V> void convertToFlatFile(const std::unordered_map<K, V>& map, const std::string& file){
            if(map.empty()){return;}
            std::vector<Entry<K, V>> temp;
            temp.reserve(map.size());
            for (auto const& [key, val] : map) {
                temp.push_back({key, val});
            }
            std::sort(temp.begin(), temp.end());
            writeData(file, temp.data(), temp.size() * sizeof(Entry<K, V>));
        }
        template <typename E> void loadLargeHeuristic(std::vector<E>& heuristic, const std::string& file){
            std::ifstream cor(file, std::ios::binary | std::ios::ate);
            if (!cor) return;

            std::streamsize fileSize = cor.tellg();
            cor.seekg(0, std::ios::beg);

            size_t numEntries = static_cast<size_t>(fileSize) / sizeof(E);
            heuristic.assign(numEntries, 0); // Vektor vorbereiten

            char* buffer = reinterpret_cast<char*>(heuristic.data());
            std::streamsize remaining = fileSize;
            std::streamsize chunkSize = 512 * 1024 * 1024; // 512 MB Blöcke

            while (remaining > 0) {
                std::streamsize toRead = std::min(remaining, chunkSize);
                cor.read(buffer, toRead);
                
                std::streamsize actualRead = cor.gcount();
                if (actualRead == 0) break; // Fehler oder Dateiende

                buffer += actualRead;
                remaining -= actualRead;
            }
        }
        template <typename E> void loadHeuristic(std::vector<E>& heuristic, const std::string& file){
            std::ifstream cor(file, std::ios::binary | std::ios::ate); // 'ate' springt zum Ende
            if (!cor) return;
            std::streamsize fileSize = cor.tellg();
            cor.seekg(0, std::ios::beg);
            size_t numEntries = fileSize / sizeof(E); 
            heuristic.resize(numEntries); 
            cor.read(reinterpret_cast<char*>(heuristic.data()), fileSize);
        }
        uint8_t getHeuVal4E(const std::vector<Entry<Key4E, uint8_t>>& heuristic, const bool G, const Puppet::StateFast& sf, const uint8_t max){
            Entry<Key4E, uint8_t> e;
            if(!G){
                e.key = {sf.shape, puppet.get4E(sf.edges, 0)};
            }else{
                e.key = {sf.shape, puppet.get4E(sf.edges, 1)};
            }
            auto i = std::lower_bound(heuristic.begin(), heuristic.end(), e);
            if (i != heuristic.end()) {
                if(!(e.key == i->key)){
                    return max+1;
                }
                return  i->value;
            }
            return max+1;
        }
        std::unordered_map<KeyCorners, uint8_t> getCornerMap();
        std::unordered_map<KeyEdges, uint8_t> getEdgeMap();
        std::unordered_map<KeySF, uint8_t> getSFMap();
        void getS4EMap(bool G, uint8_t max, std::unordered_map<Key4E, uint8_t>& Map);
        void getSEMap(const uint8_t max, std::unordered_map<KeySE, uint8_t>& Map);

        void getLinkToShape(std::unordered_map<KeyCorners, Puppet::Shape>& Map);

        void getLinkS(std::unordered_map<Puppet::ShapeFast, uint32_t>& Map);
        void getLinkC(std::unordered_map<KeyCorners, uint32_t>& Map);
        void getLinkE(std::unordered_map<Puppet::Edges, uint32_t>& Map);
        void getLinkF(std::unordered_map<Puppet::FlipsFast, uint32_t>& Map);

        void getCFarray(std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF);
        void getSEarray(std::vector<uint8_t>& HSE, std::unordered_map<Puppet::ShapeFast, uint32_t>& MS, std::unordered_map<Puppet::Edges, uint32_t>& ME);

        uint8_t getMapValCE(std::unordered_map<KeyCorners, uint8_t>& HeuC, std::unordered_map<KeyEdges, uint8_t>& HeuE, Puppet::StateFast& sf);

        uint8_t getHeuValCE(const std::vector<Entry<KeyCorners, uint8_t>>& heuC, const std::vector<Entry<KeyEdges, uint8_t>>& heuE, const Puppet::StateFast& sf);
        uint8_t getHeuValS4(const std::vector<Entry<KeyCorners, uint8_t>>& HC, const std::vector<Entry<KeySF, uint8_t>>& HSF, 
            const std::vector<Entry<Key4E, uint8_t>>& H4E1, const std::vector<Entry<Key4E, uint8_t>>& H4E2, const Puppet::StateFast& sf, const uint8_t max);
        uint8_t getHeuValS(const std::vector<Entry<KeyCorners, uint8_t>>& HC, const std::vector<Entry<KeySF, uint8_t>>& HSF, const std::vector<Entry<KeySE, uint8_t>>& HSE, 
            const Puppet::StateFast& sf, const uint8_t max);
        uint8_t getHeuValSE(const std::vector<Entry<KeyCorners, uint8_t>>& HC, const std::vector<Entry<KeySF, uint8_t>>& HSF, const std::vector<Entry<KeyEdges, uint8_t>>& HE, const Puppet::StateFast& sf);

        std::vector<std::array<uint8_t, 2>> astarCE(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HeuC, std::vector<Entry<KeyEdges, uint8_t>>& HeuE);
        std::vector<std::array<uint8_t, 2>> AstarShape4(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HC, std::vector<Entry<KeySF, uint8_t>>& HSF, 
            std::vector<Entry<Key4E, uint8_t>>& H4E1, std::vector<Entry<Key4E, uint8_t>>& H4E2, const uint8_t max);
        void AstarSE(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HC, std::vector<Entry<KeySF, uint8_t>>& HSF, std::vector<Entry<KeyEdges, uint8_t>>& HE, std::vector<std::array<uint8_t, 2>>& alg);
        std::vector<std::array<uint8_t, 2>> AstarS(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HC, std::vector<Entry<KeySF, uint8_t>>& HSF, 
            std::vector<Entry<KeySE, uint8_t>>& HSE, const uint8_t max);

        uint8_t getSuperValS(std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
            const std::vector<Entry<KeySE, uint8_t>>& HSE, const uint8_t max, const Puppet::StateFast& sf);
        
        std::vector<std::array<uint8_t, 2>> superstar(const Puppet::State& start, 
            std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
            std::vector<Entry<KeySE, uint8_t>>& HSE, const uint8_t max); //const????

        uint8_t getUltraValS(std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
            std::vector<uint8_t>& HSE, std::unordered_map<Puppet::ShapeFast, uint32_t>& MS, std::unordered_map<Puppet::Edges, uint32_t>& ME, const Puppet::StateFast& sf);
        
        std::vector<std::array<uint8_t, 2>> ultrastar(const Puppet::State& start, 
            std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
            std::vector<uint8_t>& HSE, std::unordered_map<Puppet::ShapeFast, uint32_t>& MS, std::unordered_map<Puppet::Edges, uint32_t>& ME); //const????

        std::vector<std::array<uint8_t, 2>> Astar_RSS(std::unordered_map<KeyCorners, uint32_t>& MC,
            std::unordered_map<Puppet::FlipsFast, uint32_t>& MF,
            std::unordered_map<Puppet::ShapeFast, uint32_t>& MS,
            std::unordered_map<Puppet::Edges, uint32_t>& ME,
            //std::vector<Entry<KeyCorners, uint32_t>>& HC,
            std::vector<Entry<Puppet::FlipsFast, uint32_t>>& HF,
            //std::vector<Entry<Puppet::ShapeFast, uint32_t>>& HS,
            std::vector<Entry<Puppet::Edges, uint32_t>>& HE,
            std::vector<Entry<KeyCorners, Puppet::Shape>>& H_shape,
            std::vector<uint8_t>& HCF,
            std::vector<uint8_t>& HSE);
};

namespace std {
    template <> struct hash<Puppet::Corners> {
        size_t operator()(const Puppet::Corners& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;
            uint64_t cData = 0;
            std::memcpy(&cData, k.data, 7);
            seed ^= std::hash<uint64_t>{}(cData) + magic + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
    template <> struct hash<Puppet::ShapeFast> {
        size_t operator()(const Puppet::ShapeFast& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;
            seed ^= std::hash<uint64_t>{}(k.bits[0]) + magic + (seed << 6) + (seed >> 2);
            seed ^= std::hash<uint64_t>{}(k.bits[1]) + magic + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
    template <> struct hash<Puppet::FlipsFast> {
        size_t operator()(const Puppet::FlipsFast& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;
            seed ^= std::hash<uint16_t>{}(k.bits) + magic + (seed << 6) + (seed >> 2); 
            return seed;
        }
    };
    template <> struct hash<Puppet::Edges> {
        size_t operator()(const Puppet::Edges& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;

            uint64_t e8;
            std::memcpy(&e8, k.data, 8);
            seed ^= std::hash<uint64_t>{}(e8) + magic + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
    template <> struct hash<KeyCorners> {
        size_t operator()(const KeyCorners& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;

            // 1. Hash Shape (unsere 2 uint64_t)
            seed ^= std::hash<uint64_t>{}(k.shape.bits[0]) + magic + (seed << 6) + (seed >> 2);
            seed ^= std::hash<uint64_t>{}(k.shape.bits[1]) + magic + (seed << 6) + (seed >> 2);

            // 2. Hash Corners (als 64-bit Block)
            uint64_t cData = 0;
            std::memcpy(&cData, k.corners.data, 7);
            seed ^= std::hash<uint64_t>{}(cData) + magic + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
    template <> struct hash<KeyEdges> {
        size_t operator()(const KeyEdges& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;

            // 1. Hash Edges (9 Bytes)
            // Wir nehmen die ersten 8 Bytes auf einmal
            uint64_t e8;
            std::memcpy(&e8, k.edges.data, 8);
            seed ^= std::hash<uint64_t>{}(e8) + magic + (seed << 6) + (seed >> 2);
            
            // Das 9. Byte der Edges
            //seed ^= std::hash<uint8_t>{}(k.edges.data[8]) + magic + (seed << 6) + (seed >> 2);//brauchen wir nicht weil ist eh determined

            // 2. Hash Flips (uint16_t)
            seed ^= std::hash<uint16_t>{}(k.flips.bits) + magic + (seed << 6) + (seed >> 2); 

            return seed;
        }
    };
    template <> struct hash<KeySF>{
        size_t operator()(const KeySF& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;

            seed ^= std::hash<uint64_t>{}(k.shape.bits[0]) + magic + (seed << 6) + (seed >> 2);
            seed ^= std::hash<uint64_t>{}(k.shape.bits[1]) + magic + (seed << 6) + (seed >> 2);

            seed ^= std::hash<uint16_t>{}(k.flips.bits) + magic + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
    template <> struct hash<Key4E>{
        size_t operator()(const Key4E& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;

            seed ^= std::hash<uint64_t>{}(k.shape.bits[0]) + magic + (seed << 6) + (seed >> 2);
            seed ^= std::hash<uint64_t>{}(k.shape.bits[1]) + magic + (seed << 6) + (seed >> 2);

            uint32_t e4 = 0;
            std::memcpy(&e4, k.fe.data, 4);
            seed ^= std::hash<uint32_t>{}(e4) + magic + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
    template <> struct hash<KeySE>{
        size_t operator()(const KeySE& k) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;

            seed ^= std::hash<uint64_t>{}(k.shape.bits[0]) + magic + (seed << 6) + (seed >> 2);
            seed ^= std::hash<uint64_t>{}(k.shape.bits[1]) + magic + (seed << 6) + (seed >> 2);

            uint64_t e8 = 0;
            std::memcpy(&e8, k.edges.data, 8);
            seed ^= std::hash<uint64_t>{}(e8) + magic + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
    template <> struct hash<Puppet::StateFast> {
        size_t operator()(const Puppet::StateFast& s) const {
            size_t seed = 0;
            const size_t magic = 0x9e3779b9;

            // --- 1. SHAPE HASH (aus KeyCorners Logik) ---
            seed ^= std::hash<uint64_t>{}(s.shape.bits[0]) + magic + (seed << 6) + (seed >> 2);
            seed ^= std::hash<uint64_t>{}(s.shape.bits[1]) + magic + (seed << 6) + (seed >> 2);

            // --- 2. CORNERS HASH (aus KeyCorners Logik) ---
            uint64_t cData = 0;
            std::memcpy(&cData, s.corners.data, 7);
            seed ^= std::hash<uint64_t>{}(cData) + magic + (seed << 6) + (seed >> 2);

            // --- 3. EDGES HASH (aus KeyEdges Logik) ---
            uint64_t e8;
            std::memcpy(&e8, s.edges.data, 8);
            seed ^= std::hash<uint64_t>{}(e8) + magic + (seed << 6) + (seed >> 2);
            //seed ^= std::hash<uint8_t>{}(s.edges.data[8]) + magic + (seed << 6) + (seed >> 2);

            // --- 4. FLIPS HASH (aus KeyEdges Logik) ---
            seed ^= std::hash<uint16_t>{}(s.flips.bits) + magic + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}

namespace Map {
    template<typename K, typename V>
    void saveMap(const std::unordered_map<K, V>& map, const std::string& filename) {
        std::ofstream os(filename, std::ios::binary);
        
        // 1. Anzahl der Elemente speichern
        size_t size = map.size();
        os.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // 2. Jedes Key-Value Paar schreiben
        for (const auto& [key, value] : map) {
            os.write(reinterpret_cast<const char*>(&key), sizeof(K));
            os.write(reinterpret_cast<const char*>(&value), sizeof(V));
        }
        os.close();
    }

    template<typename K, typename V>
    void loadMap(std::unordered_map<K, V>& map, const std::string& filename) {
        std::ifstream is(filename, std::ios::binary);
        if (!is) {std::cout << "FEHLER: Datei " << filename << " gibtz ned" << std::endl; return;} // Datei nicht gefunden

        size_t size;
        is.read(reinterpret_cast<char*>(&size), sizeof(size));

        map.clear();
        map.reserve(size); // WICHTIG: Performance!

        for (size_t i = 0; i < size; ++i) {
            K key;
            V value;
            is.read(reinterpret_cast<char*>(&key), sizeof(K));
            is.read(reinterpret_cast<char*>(&value), sizeof(V));
            map.insert({key, value});
        }
        is.close();
    }
}