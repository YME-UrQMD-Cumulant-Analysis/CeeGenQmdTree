#ifndef __PIDCVT___
#define __PIDCVT___

/*
    The converting code is taken from urqmd-4.0/ityp2pdg.f.
    In case higher version of UrQMD changed the raw file, this script will no longer work!
    Date: 12.12.2025 - Yige Huang

    Usage:

    #include "PIDConvert.h"

    ... // in your code 
    auto pcvt = PIDHelper();
    ... // in your track loop
        auto pdgID = pcvt.GetPdgID(ityp, iso3);
*/

#include <vector>
#include "TMath.h"

using std::vector;

class PIDHelper {

    private:
    
        const vector<vector<int>> PID_table = {
            {  1, -1,  2112},
            {  1,  1,  2212},
            {  2, -1, 12112}, {2,  1, 12212},
            {  3, -1,  1214}, {3,  1,  2124},
            {  4, -1, 22112}, {4,  1, 22212},
            {  5, -1, 32112}, {5,  1, 32212},
            {  6, -1,  2116}, {6,  1,  2216},
            {  7, -1, 12116}, {7,  1, 12216},
            {  8, -1, 21214}, {8,  1, 22124},
            {  9, -1, 42112}, {9,  1, 42212},
            { 10, -1, 31214}, {10,  1, 32124},
            { 14, -1,  1218}, {14,  1,  2128},
            { 23, -1,  1218}, {23,  1,  2128},
            { 24, -3,  1114}, {24, -1,  2114}, {24, 1,  2214}, {24, 3,  2224},
            { 25, -3, 31114}, {25, -1, 32114}, {25, 1, 32214}, {25, 3, 32224},
            { 26, -3,  1112}, {26, -1,  1212}, {26, 1,  2122}, {26, 3,  2222},
            { 27, -3, 11114}, {27, -1, 12114}, {27, 1, 12214}, {27, 3, 12224},
            { 28, -3, 11112}, {28, -1, 11212}, {28, 1, 12122}, {28, 3, 12222},
            { 29, -3,  1116}, {29, -1,  1216}, {29, 1,  2126}, {29, 3,  2226},
            { 30, -3, 21112}, {30, -1, 21212}, {30, 1, 22122}, {30, 3, 22222},
            { 31, -3, 21114}, {31, -1, 22114}, {31, 1, 22214}, {31, 3, 22224},
            { 32, -3, 11116}, {32, -1, 11216}, {32, 1, 12126}, {32, 3, 12226},
            { 33, -3,  1118}, {33, -1,  2118}, {33, 1,  2218}, {33, 3,  2228},
            { 40, -3,  1118}, {40, -1,  2118}, {40, 1,  2218}, {40, 3,  2228},
            { 41,  0,  3122},
            { 42,  0, 13122},   
            { 43,  0,  3124},   
            { 44,  0, 23122},   
            { 45,  0, 33122},
            { 46,  0, 13124},
            { 47,  0, 43122},   
            { 48,  0, 53122},   
            { 49,  0,  3126},   
            { 50,  0, 13126},   
            { 51,  0, 23124},   
            { 52,  0,  3128},   
            { 53,  0, 23126},   
            { 54, -2,  3112}, {54,  0,  3212}, {54,  2,  3222},
            { 55, -2,  3114}, {55,  0,  3214}, {55,  2,  3224},
            { 56, -2, 13112}, {56,  0, 13212}, {56,  2, 13222},
            { 57, -2, 13114}, {57,  0, 13214}, {57,  2, 13224},
            { 58, -2, 23112}, {58,  0, 23212}, {58,  2, 23222},
            { 59, -2,  3116}, {59,  0,  3216}, {59,  2,  3226},
            { 60, -2, 13116}, {60,  0, 13216}, {60,  2, 13226},
            { 61, -2, 23114}, {61,  0, 23214}, {61,  2, 23224},
            { 62, -2,  3118}, {62,  0,  3218}, {62,  2,  3228},
            { 63, -1,  3312}, {63,  1,  3322},
            { 64, -1,  3314}, {64,  1,  3324},
            { 66, -1, 13314}, {66,  1, 13324},
            { 69,  0,  3334},
            { 70,  0,  4122},
            {100,  0,    22}, 
            {101, -2,  -211}, {101,  0,   111}, {101,  2,   211},
            {102,  0,   221},
            {103,  0,   223},
            {104, -2,  -213}, {104,  0,   113}, {104,  2,   213},
            {105,  0, 10221},
            {106, -1,   311}, {106,  1,   321},
            {107,  0,   331},
            {108, -1,   313}, {108,  1,   323},
            {109,  0,   333},
            {110, -1, 10311}, {110,  1, 10321},
            {111, -2,-10211}, {111,  0, 10111}, {111,  2, 10211},
            {112,  0, 20221},
            {113, -1, 10313}, {113,  1, 10323},
            {114, -2,-20213}, {114,  0, 20113}, {114,  2, 20213},
            {115,  0, 20223},
            {116,  0, 40223},
            {117, -1,   315}, {117,  1,   325},
            {118, -2,  -215}, {118,  0,   115}, {118,  2,   215},
            {119,  0,   225},
            {120,  0,   335},
            {121, -1, 20313}, {121,  1, 20323},
            {122, -2,-10213}, {122,  0, 10113}, {122,  2, 10213},
            {123,  0, 10223},
            {125, -1, 30313}, {125,  1, 30323},
            {126, -2,-40213}, {126,  0, 40113}, {126,  2, 40213},
            {127,  0, 50223},
            {128,  0, 10333},
            {129, -1, 40313}, {129,  1, 40323},
            {130, -2,-30213}, {130,  0, 30113}, {130,  2, 30213},
            {131,  0, 60223},
            {132,  0,   337},
            {133,  -1,   421}, {133,   1,   411},
            {134,  -1, 10421}, {134,   1, 10411},
            {135,  0, 443},
            {136,  0, 10441},
            {137,  0, 100443},
            {138,   0,   431},
            {139,   0,   433}
        };

    public:

        int GetPdgID(int ityp, int iso3) {
            /*
                ityp: UrQMD particle type
                iso3: 2 I_3, used to identify particle species in UrQMD
            */
            bool anti = false;
            int aityp = fabs(ityp);
            int norm_iso3 = iso3;
            
            // PYTHIA pdgid's (only need to subtract the offset!)
            if (aityp > 1000) {
                if (ityp > 0) {
                    return ityp - 1000;
                } else {
                    return ityp + 1000; // origin: -1*(abs(ityp) - 1000)
                }
            }
            
            // check for antiparticles
            if (ityp < 0) {
                // it is an antiparticle
                anti = true;
                norm_iso3 = -iso3;
            }
            
            // search for the ITYP in PID_table (vector<vector<int>>)
            int first = 0;    // in original fortran code, this is 1, 'caz it starts with 1
            int last = PID_table.size() - 1; // similar to first
            int next = 0;
            bool found = false;
            
            if (PID_table[first][0] == aityp) {
                next = first;
                found = true;
            } else if (PID_table[last][0] == aityp) {
                next = last;
                found = true;
            }
            
            // binary search in case ITYPE not found at boundaries
            while (!found) {
                if (last <= first + 1) {
                    return 0; // not found
                }
                
                next = (first + last) / 2;
                
                if (PID_table[next][0] < aityp) {
                    first = next;
                } else if (PID_table[next][0] > aityp) {
                    last = next;
                } else {
                    found = true;
                }
            }
            
            // calculate the entry with the wanted ISO3
            next = next - (PID_table[next][1] - norm_iso3) / 2;
            
            // check if we found the correct values in PID_table
            if (PID_table[next][0] == aityp && PID_table[next][1] == norm_iso3) {
                if (anti) {
                    return -PID_table[next][2];
                } else {
                    return PID_table[next][2];
                }
            }
            
            return 0; // not found
        }
};

#endif