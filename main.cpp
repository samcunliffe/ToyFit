
#include <vector>
#include <iostream>

#include <TCanvas.h>
#include <TString.h>

#include <RooFitResult.h>
#include <RooFit.h>
#include <RooGaussian.h>
#include <RooPlot.h>
#include <RooMinuit.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataSet.h>

const bool DEBUG = 1;

//#include "Model.hpp"
#include "GaussianModel1D.hpp"
#include "Fitter.hpp"



class Plotter1D
{
 public:
  Plotter1D(RooRealVar &x): _x(x) {}
  void plot(GaussianModel1D& model, RooDataSet* data)
  {
    RooPlot* rp = _x.frame();
    data->plotOn(rp);
    model.pdf()->plotOn(rp);
    TCanvas canvas;
    rp->Draw();
    canvas.SaveAs("plot.pdf");
    return;
  }
 private:
  RooRealVar& _x;
};

int main(int argc, char *argv[])
{
  // setup model
  RooRealVar x("x", "x", -1, 1);
  GaussianModel1D model("simple", x);
  RooDataSet *data = model.generate();

  // minimise the likelihood
  Fitter f(&model, data);
  std::cout << "pointer to the model pdf " << model.pdf() << std::endl;
  f.fit();
  f.plotNLLScan(model.getPar("mean")); // make a quick NLL plot
  f.plotNLLScan(model.getPar("sigma"));

  // make a plot of the PDF and data
  Plotter1D p(x);
  p.plot(model, data);

  return 0;
}
