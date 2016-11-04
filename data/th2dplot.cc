TH2F* Load(const char* filename, const int phi, const int opt, const bool reverse=false)
{
    std::ifstream file(filename);

    if (!file) {
      std::cerr << "Error: file did not exist!" << std::endl;
      return NULL;
    }

    const char* name = Form("CS1-%i-p%i", opt, phi);
    double zmin = 0.;
    double zmax = 5.;
    int    nz   = 5;
    double rmin = 0.;
    double rmax = 9.;
    int    nr   = 9;

    int    ibuff[3];
    double dbuff[2];
    TH2F* hist = new TH2F(name, name, nz, zmin, zmax, nr, rmin, rmax);

    double min = 9999.;
    double max = -9999.;

    while (true) {

        file >> ibuff[0] >> ibuff[1] >> ibuff[2] >> dbuff[0] >> dbuff[1];
        dbuff[0] /= 3600.*24*365;
        if (!file) break;

        if (ibuff[1]==phi) {
          if (reverse==false)
            hist->Fill(ibuff[0]+0.5, ibuff[2]+0.5, dbuff[opt]);
          else
            hist->Fill(ibuff[0]+0.5, (rmax-1-ibuff[2])+0.5, dbuff[opt]);
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

    hist->GetZaxis()->SetRangeUser(0., max);
    std::cout << "=========================" << std::endl;
    std::cout << " min.: " << min << std::endl;
    std::cout << " max.: " << max << std::endl;
    std::cout << "=========================" << std::endl;

    return hist;
}


TH2F* LoadCS0(const char* filename, const int opt)
{
    std::ifstream file(filename);

    if (!file) {
      std::cerr << "Error: file did not exist!" << std::endl;
      return NULL;
    }

    const char* name = Form("CS0-%i", opt);
    double pmin = 0.;
    double pmax = 4.;
    int    np   = 4;
    double rmin = 0.;
    double rmax = 9.;
    int    nr   = 9;

    int    ibuff[3];
    double dbuff[2];
    TH2F* hist = new TH2F(name, name, np, pmin, pmax, nr, rmin, rmax);

    int swap = 0;

    double min = 1e+100;
    double max = -9999.;

    while (true) {

        file >> ibuff[0] >> ibuff[1] >> ibuff[2] >> dbuff[0] >> dbuff[1];
        if (!file) break;
        dbuff[0] /= 3600.*24*365;

        if (ibuff[1]==0)
          swap = 1;
        else if (ibuff[1]==1)
          swap = 0;
        else
          swap = ibuff[1];
        //ibuff[1] = swap;

        //hist->Fill(ibuff[1]+0.5, (rmax-1-ibuff[2])+0.5, dbuff[opt]);
        hist->Fill(ibuff[1]+0.5, ibuff[2]+0.5, dbuff[opt]);

        if (dbuff[opt]>max) max = dbuff[opt];
        if (dbuff[opt]<min) min = dbuff[opt];
    }
   
    // opt==0: flux
    if (opt==0)
      hist->SetTitle("; #phi; R; Neutron Flux [n/m^{2}/sec]");

    // opt==1: heat
    if (opt==1)
      hist->SetTitle("; $phi; R; Energy Deposit [Gy/sec]");

    std::cout << " minimum: " << min << std::endl;
    std::cout << " maximum: " << max << std::endl;

    return hist;
}


void th2dplot()
{
  /*
  TCanvas* c0 = new TCanvas("c0", "c0", 550., 400.);
  c0->SetTicks(1,1);
  c0->SetRightMargin(0.16);

  gStyle->SetOptStat(0);
  gStyle->SetTitleOffset(1.2, "z");

  TH2F* hist = LoadCS0("./161019CS0.dat", 1);
  hist->Draw("colz");
  */

  TCanvas* c0 = new TCanvas("c0", "c0", 740., 450.);
  c0->Divide(2,2);

  gStyle->SetOptStat(0);
  gStyle->SetTitleOffset(1.2, "z");

  for (int i=0; i<4; i++) {
    c0->cd(i+1);
    gPad->SetTicks(1,1);
    gPad->SetRightMargin(0.16);
    TH2F* hist = Load("./161029CS1Kerma.dat",i,1);
    //TH2F* hist = Load("./rad_phits.dat",i,1,true);
    hist->Draw("colz");
  }
}
