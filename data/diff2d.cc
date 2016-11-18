static int cnt = 0;

TH2F* Load(const char* filename, const int phi, const int opt, const bool reverse=false)
{
    std::ifstream file(filename);

    if (!file) {
      std::cerr << "Error: file did not exist!" << std::endl;
      return NULL;
    }

    const char* name = Form("CS1-%i-p%i_%i", opt, phi, cnt);
    double zmin = 0.;
    double zmax = 5.;
    int    nz   = 5;
    double rmin = 0.;
    double rmax = 9.;
    int    nr   = 9;

    int    ibuff[3];
    double dbuff[3];
    TH2F* hist = new TH2F(name, name, nz, zmin, zmax, nr, rmin, rmax);

    double min = 9999.;
    double max = -9999.;

    while (true) {

        file >> ibuff[0] >> ibuff[1] >> ibuff[2] >> dbuff[0] >> dbuff[1] >> dbuff[2];
        dbuff[0] /= 3600.*24*365;
        if (!file) break;

        if (ibuff[1]==phi) {
          if (reverse==true)
            hist->Fill(ibuff[0]+0.5, (rmax-1-ibuff[2])+0.5, dbuff[opt]);
          else
            hist->Fill(ibuff[0]+0.5, ibuff[2]+0.5, dbuff[opt]);
        }


        if (dbuff[opt]>max) max = dbuff[opt];
        if (dbuff[opt]<min) min = dbuff[opt];
    }
   
    // opt==0: flux
    if (opt==0)
      hist->SetTitle(Form("#phi = %i; Z; R; Neutron Flux [n/m^{2}/sec]", phi+1));

    // opt==1: heat
    if (opt==1)
      hist->SetTitle(Form("#phi = %i; Z; R; Energy Deposit [Gy/sec]", phi+1));

    //hist->GetZaxis()->SetRangeUser(0., max);
    std::cout << "=========================" << std::endl;
    std::cout << " min.: " << min << std::endl;
    std::cout << " max.: " << max << std::endl;
    std::cout << "=========================" << std::endl;

    cnt ++;

    return hist;
}


void plotdiff2d()
{
  TH2F* track[4];
  TH2F* kerma[4];

  TCanvas* c0 = new TCanvas("c0", "c0", 740., 450.);
  c0->Divide(2, 2);
  gStyle->SetOptStat(0);
  gStyle->SetTitleOffset(1.2, "z");

  for (int i=0; i<4; i++) {
    //track[i] = Load("161029CS1Track.dat", i, 1);
    track[i] = Load("rad_phits.dat", i, 1, true);
    kerma[i] = Load("./phits288/161029CS1Track.dat", i, 1);
    track[i]->Divide(kerma[i]);
    
    c0->cd(i+1);
    gPad->SetTicks(1,1);
    gPad->SetRightMargin(0.16);
    //track[i]->SetTitle(Form("#phi = %i; Z; R; Track/Kerma", i+1));
    track[i]->SetTitle(Form("#phi = %i; Z; R; phits2.73 / phits2.88", i+1));
    track[i]->Draw("colz");
  }

}


void diff2d()
{
  plotdiff2d();
}
