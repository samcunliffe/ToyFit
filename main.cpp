#include <iostream>
#include <TCanvas.h>
#include <RooPlot.h>

#include <RooRealVar.h>
#include <RooDataSet.h>

#include "InvariantMassModel.hpp"
#include "Fitter.hpp"

using std::cout; using std::endl;
const bool scans = false;
const bool profiles = false;

int main(int argc, char *argv[])
{
  // setup model
  RooRealVar x("x", "m", 4800, 5700);
  InvariantMassModel model("simple", x);
  RooDataSet *data = model.generate();

  // minimise the likelihood
  Fitter f(&model, data);
  f.fit();

  // plot things
  f.plot("after_fit", &x);

  if (scans) {
    cout << "Now try making scans" << endl;
    f.plotNLLScan(model.getPar("mean")); // make a quick NLL plot
    f.plotNLLScan(model.getPar("sigma"));
    f.plotNLLScan(model.getPar("lambda"));
    f.plotNLLScan(model.getPar("f_sig"));
    f.plotNLLScan(model.getPar("mean"), model.getPar("sigma"));
  }
  if (profiles) {
    cout << "Now try making profiles" << endl;
    f.plotNLLProfile(model.getPar("sigma")); // proper NLL scan
    f.plotNLLProfile(model.getPar("mean"));
    f.plotNLLProfile(model.getPar("lambda"));
    f.plotNLLProfile(model.getPar("f_sig"));
  }
  // test a 2D profile
  f.plotNLLProfile(model.getPar("mean"), model.getPar("sigma"));
  return 0;
}
