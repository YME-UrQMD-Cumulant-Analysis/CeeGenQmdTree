#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cassert>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "PIDConvert.hpp"

#define MAXMUL 5000

using std::cout;
using std::endl;
using std::string;

bool isParticleVector(const string& line) {
    return line.size() >= 3 && 
        line[0] == ' ' &&
        line[1] == ' ' &&
        line[2] != ' ';
}

int main(int argc, char** argv) {
    auto pc = PIDHelper();
    string line;

    bool isRunning = false; // just for the first event

    int jobId = atoi(argv[1]);
    cout << "[LOG] - Convert: The job ID is " << jobId << endl;
    // prepare output file
    auto tf = TFile::Open(Form("%05d.root", jobId), "recreate");
    
    float b = -1;
    int Npart = -1;
    int mul = 0; // Nch
    vector<float> vpx;
    vector<float> vpy;
    vector<float> vpz;
    vector<int> vpid;
    float px[MAXMUL];
    float py[MAXMUL];
    float pz[MAXMUL];
    int pid[MAXMUL];
    TTree tt("urqmd", "urqmd");
    tt.Branch("mul", &mul, "mul/I");
    tt.Branch("b", &b, "b/F");
    tt.Branch("Npart", &Npart, "Npart/I");
    tt.Branch("pid", pid, "pid[mul]/I");
    tt.Branch("px", px, "px[mul]/F");
    tt.Branch("py", py, "py[mul]/F");
    tt.Branch("pz", pz, "pz[mul]/F");

    std::ifstream fin("qmd_output.txt");
    int nEv = 0;
    while (std::getline(fin, line)) {
        if (isParticleVector(line)) {
            std::istringstream iss(line);
            float val1, val2, val3, val4, val5; // r0 rx ry rz p0
            float px_, py_, pz_; // px py pz
            float val6; // m
            int ityp, iso3; // ityp 2i3, and later values are not used

            iss >> val1 >> val2 >> val3 >> val4 >> val5
                >> px_ >> py_ >> pz_
                >> val6 >> ityp >> iso3;
            if (val1 == 0 && val5 == 0) { continue; }
            int pdgId = pc.GetPdgID(ityp, iso3);
            vpx.push_back(px_);
            vpy.push_back(py_);
            vpz.push_back(pz_);
            vpid.push_back(pdgId);
        } else if (line.rfind("UQMD", 0) == 0) {
            if (isRunning) { // if it is NOT the first: last event ends, record it, and clean the branchs
                mul = vpx.size();
                memcpy(px, vpx.data(), mul * sizeof(float));
                memcpy(py, vpy.data(), mul * sizeof(float));
                memcpy(pz, vpz.data(), mul * sizeof(float));
                memcpy(pid, vpid.data(), mul * sizeof(int));
                tt.Fill();
                // report this event
                cout << "[LOG] - Convert: Event report => b = " << b << " fm, Npart = " << Npart << ", multiplicity = " << mul << "." << endl;
                mul = 0; // reset multiplicity
                b = -1;
                Npart = -1;
                vpx.clear();
                vpy.clear();
                vpz.clear();
                vpid.clear();
                nEv += 1;
            } else { isRunning = true; } // do nothing for the first event but set this flag to true
        } else if (line.rfind("impact", 0) == 0) {
            size_t pos = strlen("impact_parameter_real/min/max(fm):");
            std::istringstream iss(line.substr(pos));
            iss >> b;
        } else if (line.rfind("Participants_Glaube", 0) == 0) {
            size_t pos = strlen("Participants_Glauber (Nuc_part,prot_part,neut_part):");
            std::istringstream iss(line.substr(pos));
            double Npart_f;
            iss >> Npart_f;
            Npart = (int)Npart_f;
        }
    }
    // and save the last event
    if (isRunning) {
        mul = vpx.size();
        memcpy(px, vpx.data(), mul * sizeof(float));
        memcpy(py, vpy.data(), mul * sizeof(float));
        memcpy(pz, vpz.data(), mul * sizeof(float));
        memcpy(pid, vpid.data(), mul * sizeof(int));
        cout << "[LOG] - Convert: Event report => b = " << b << " fm, Npart = " << Npart << ", multiplicity = " << mul << "." << endl;
        tt.Fill();
        nEv += 1;
    }
    
    cout << "[LOG] - Convert: Task report: Number of events: " << nEv << "." << endl;
    // save the root file
    tf->cd();
    tt.Write();
    tf->Close();
    cout << "[LOG] - Convert: Programmende. Auf Wieder-Sehen!" << endl; // demonstrate a touch of deutschen Humor
    cout << "               (Data successfully bureaucratized into ROOT format.)" << endl;

    return 0;
}